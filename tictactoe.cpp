#include <windows.h>
#include <wininet.h>
#include <string>
#include <vector>
#include <tbb/tbb.h>

struct GameState {
    std::vector<std::vector<std::string>> board;
    int size;
    std::string currentTurn;
    std::string winner;
};

bool sendMove(int row, int col, GameState& state) {
    HINTERNET hInternet = InternetOpen("WinAPI", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    HINTERNET hConnect = InternetConnect(hInternet, "localhost", 8080, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
    HINTERNET hRequest = HttpOpenRequest(hConnect, "POST", "/move", NULL, NULL, NULL, 0, 0);

    std::string body = "{\"row\":" + std::to_string(row) + ",\"col\":" + std::to_string(col) + "}";
    HttpSendRequest(hRequest, "Content-Type: application/json", -1, (LPVOID)body.c_str(), body.length());

    char buffer[4096];
    DWORD bytesRead;
    std::string response;
    while (InternetReadFile(hRequest, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
        response.append(buffer, bytesRead);
    }

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
    state.board[row][col] = (state.currentTurn == "O") ? "X" : "O";

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
      tbb::parallel_for(tbb::blocked_range<int>(0, state.size + 1), [&](const tbb::blocked_range<int>& r) {
          for (int i = r.begin(); i != r.end(); ++i) {
              MoveToEx(hdc, i * cellSize, 0, NULL);
              LineTo(hdc, i * cellSize, state.size * cellSize);
              MoveToEx(hdc, 0, i * cellSize, NULL);
              LineTo(hdc, state.size * cellSize, i * cellSize);
          }
      });
      for (int i = 0; i < state.size; ++i) {
          for (int j = 0; j < state.size; ++j) {
              if (state.board[i][j] == "X") {
                  TextOutW(hdc, j * cellSize + 40, i * cellSize + 40, L"X", 1);  // Using TextOutW for wide-char output
              } else if (state.board[i][j] == "O") {
                  TextOutW(hdc, j * cellSize + 40, i * cellSize + 40, L"O", 1);  // Using TextOutW for wide-char output
              }
          }
      }
      if (!state.winner.empty()) {
          std::wstring msg = (state.winner == "Draw" ? L"Empate!" : L"Vencedor: " + std::wstring(state.winner.begin(), state.winner.end()));
          TextOutW(hdc, 10, state.size * cellSize + 10, msg.c_str(), msg.length());
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
  wc.lpszClassName = "TicTacToe";  // Use narrow string
  RegisterClass(&wc);

  HWND hwnd = CreateWindow("TicTacToe", "Jogo da Velha", WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, CW_USEDEFAULT, 400, 400, NULL, NULL, hInstance, NULL);
  ShowWindow(hwnd, nCmdShow);

  MSG msg;
  while (GetMessage(&msg, NULL, 0, 0)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
  }
  return 0;
}

