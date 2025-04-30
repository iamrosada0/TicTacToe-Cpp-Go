
# Tic-Tac-Toe Game (GO (GOLANG) AND C++)

This project is a simple implementation of a Tic-Tac-Toe game, featuring both a web-based server-side component (built with Go) and a Windows desktop client (using WinAPI and TBB for parallelization). The server provides an API for handling game moves, while the client interacts with the game through a graphical user interface (GUI).

## Requirements

Before running the project, ensure you have the following tools and libraries installed:

### For the Web Server (Go):
- [Go](https://golang.org/dl/) version 1.16 or later

### For the Windows Client (C++):
- [TBB (Threading Building Blocks)](https://www.intel.com/content/www/us/en/programmable/tbb.html)
- A C++ compiler that supports WinAPI (such as Microsoft Visual Studio)
Certainly! Below is an updated section for the README that explains how to compile the C++ program using the `g++` command and the necessary libraries.

---

## Compilation Instructions

To compile the C++ Tic-Tac-Toe game, you need to use the `g++` compiler with specific libraries and paths. Below is the command to compile the program:

### Steps:

1. Ensure you have MinGW-w64 and the necessary libraries installed, such as TBB, GDI32, and WinINet. These are required for compiling the project.

2. Run the following `g++` command to compile the source code:

   ```bash
   g++ -I/mingw64/include -L/mingw64/lib tictactoe.cpp -ltbb12 -lgdi32 -lwininet -o tictactoe
   ```

   ### Explanation of the command:

   - **`g++`**: The GNU C++ compiler used for compiling C++ source files.
   
   - **`-I/mingw64/include`**: Specifies the directory for header files (`/mingw64/include`). This is necessary for finding external libraries such as TBB and WinINet.
   
   - **`-L/mingw64/lib`**: Specifies the directory for libraries (`/mingw64/lib`). This tells the linker where to look for `.lib` or `.dll` files when linking.
   
   - **`tictactoe.cpp`**: The C++ source code file that contains the implementation of the Tic-Tac-Toe game.
   
   - **`-ltbb12`**: Links the Intel Threading Building Blocks (TBB) library. This is used for parallel computing tasks in the application.
   
   - **`-lgdi32`**: Links the GDI32 library. It is part of the Windows API used for rendering graphics (e.g., drawing the Tic-Tac-Toe grid).
   
   - **`-lwininet`**: Links the WinINet library, used for making HTTP requests, which is essential for the communication between the client and server in the game.
   
   - **`-o tictactoe`**: Specifies the output filename. The compiled executable will be named `tictactoe`.

### Example Command:

```bash
g++ -I/mingw64/include -L/mingw64/lib tictactoe.cpp -ltbb12 -lgdi32 -lwininet -o tictactoe
```

After running the above command, the `tictactoe` executable will be generated. You can now run the compiled program.

---
## Running the Project

### 1. Run the Web Server

The server is built using Go and provides a REST API to handle game moves. It listens on `localhost:8080`.

#### To run the web server:
1. Navigate to the `server` folder (where the Go code is located).
2. Open a terminal or command prompt and run the following command:

   ```bash
   go run main.go
   ```

   The server will start, and you should see the message `Server running on :8080`.

#### Web API
- **POST /move**: This endpoint handles game moves. You can make a `POST` request with a JSON body containing the row and column where the player wants to place their mark. Example:

  ```json
  {
    "row": 0,
    "col": 1
  }
  ```

  The server will respond with the current state of the game, including the board, current turn, and winner (if any).

### 2. Run the Windows Client

The Windows client is a C++ application that communicates with the Go server to send moves and receive updates. It features a graphical interface using WinAPI for rendering the Tic-Tac-Toe board.

#### To run the Windows client:
1. Open the C++ source code in a development environment such as Microsoft Visual Studio.
2. Build and run the project. Ensure that the server is running on `localhost:8080` before launching the client.
3. Once the client is running, you can click on the board to make moves. The board will be updated, and the winner (or draw) will be displayed after the game ends.

### 3. Game Rules

- The game alternates turns between two players: **X** and **O**.
- The game ends when one player gets three marks in a row (horizontally, vertically, or diagonally), or when all cells are filled and there is no winner, resulting in a draw.
- After each move, the game state (board, current turn, and winner) is updated and sent back to the client.

## Project Structure

### Web Server (Go)

- `main.go`: The main file that contains the server logic and handles game moves.

### Windows Client (C++)

- `game.cpp`: The main C++ file for the Windows client that handles the rendering of the Tic-Tac-Toe board and user input.
- `WinAPI` and `TBB` libraries are used to manage the GUI and parallelization for rendering.

## Dependencies

- **Go**: The Go server-side component uses standard Go libraries (`net/http`, `encoding/json`).
- **C++ Libraries**:
  - **TBB**: Used for parallelism in the `WM_PAINT` section of the Windows client.
  - **WinAPI**: Used to create the window, handle user input, and render the Tic-Tac-Toe board.

## License

This project is open-source under the [MIT License](LICENSE).
