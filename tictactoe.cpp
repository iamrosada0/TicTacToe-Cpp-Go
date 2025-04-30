#include <windows.h>
#include <wininet.h>
#include <string>
#include <vector>
#include <tbb/tbb.h>
#include <sstream>

struct GameState {
    std::vector<std::vector<std::string>> board;
    int size;
    std::string currentTurn;
    std::string winner;
};

// Função para enviar jogada ao backend Go
bool sendMove(int row, int col, GameState& state) {
    HINTERNET hInternet = InternetOpen(L"WinAPI", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    HINTERNET hConnect = InternetConnect(hInternet, L"localhost", 8080, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
    HINTERNET hRequest = HttpOpenRequest(hConnect, L"POST", L"/move", NULL, NULL, NULL, 0, 0);

    std::string body = "{\"row\":" + std::to_string(row) + ",\"col\":" + std::to_string(col) + "}";
    HttpSendRequest(hRequest, L"Content-Type: application/json", -1, (LPVOID)body.c_str(), body.length());

    char buffer[4096];
    DWORD bytesRead;
    std::string response;
    while (InternetReadFile(hRequest, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
        response.append(buffer, bytesRead);
    }

    // Parsear resposta JSON manualmente (simplificado)
    if (response.find("\"winner\":\"\"") == std::string::npos) {
        if (response.find("\"Draw\"") != std::string::npos) {
            state.winner = "Draw";
        } else {
            size_t pos = response.find("\"winner\":\"") + 10;
            state.winner = response.substr(pos, 1);
        }
    } else {
        state.winner = "";
    }
    state.currentTurn = (state.currentTurn == "X") ? "O" : "X";
    state.board[row][col] = (state.currentTurn == "O") ? "X" : "O"; // Atualiza localmente

    InternetCloseHandle(hRequest);
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);
    return true;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static GameState state = { std::vector<std::vector<std::string>>(3, std::vector<std::string>(3, "")), 3, "X", "" };
    static int cellSize = 100;

    switch (msg) {
    case WM_LBUTTONDOWN: {
        if (state.winner != "") break;
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        int row = y / cellSize;
        int col = x / cellSize;
        if (row < state.size && col < state.size && state.board[row][col].empty()) {
            sendMove(row, col, state);
            InvalidateRect(hwnd, NULL, TRUE);
        }
        break;
    }
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // Desenhar grade em paralelo com TBB
        tbb::parallel_for(tbb::blocked_range<int>(0, state.size + 1), [&](const tbb::blocked_range<int>& r) {
            for (int i = r.begin(); i != r.end(); ++i) {
                MoveToEx(hdc, i * cellSize, 0, NULL);
                LineTo(hdc, i * cellSize, state.size * cellSize);
                MoveToEx(hdc, 0, i * cellSize, NULL);
                LineTo(hdc, state.size * cellSize, i * cellSize);
            }
        });

        // Desenhar X e O
        for (int i = 0; i < state.size; ++i) {
            for (int j = 0; j < state.size; ++j) {
                if (state.board[i][j] == "X") {
                    TextOutA(hdc, j * cellSize + 40, i * cellSize + 40, "X", 1);
                } else if (state.board[i][j] == "O") {
                    TextOutA(hdc, j * cellSize + 40, i * cellSize + 40, "O", 1);
                }
            }
        }

        // Exibir vencedor
        if (!state.winner.empty()) {
            std::string msg = state.winner == "Draw" ? "Empate!" : "Vencedor: " + state.winner;
            TextOutA(hdc, 10, state.size * cellSize + 10, msg.c_str(), msg.length());
        }

        EndPaint(hwnd, &ps);
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"TicTacToe";
    RegisterClass(&wc);

    HWND hwnd = CreateWindow(L"TicTacToe", L"Jogo da Velha", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 400, NULL, NULL, hInstance, NULL);
    ShowWindow(hwnd, nCmdShow);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}