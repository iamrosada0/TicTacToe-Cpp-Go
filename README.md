

## Tic-Tac-Toe Game (C++ AND Golang (go))

This repository contains a simple Tic-Tac-Toe game implemented in C++ and Go, featuring both a graphical interface and a web-based API to handle game moves and state. The game allows two players to make moves on a 3x3 board, with support for checking for winners, handling ties, and switching turns.

### Features:
- **Graphical Interface (Windows)**: The game includes a GUI that allows players to interact with the game board using mouse clicks. It displays the state of the game and announces the winner or draw.
- **Web API (Go)**: The game server is written in Go, which exposes an HTTP API that allows players to make moves by sending POST requests.
- **Parallelism (TBB)**: The application uses Intel's Threading Building Blocks (TBB) for parallel computation in drawing the grid and handling multiple processes simultaneously.
- **Cross-Technology Integration**: The C++ client communicates with the Go server to update the game state over HTTP requests.

### Technologies Used:
- **C++**: For the graphical client that interacts with the user.
- **Go**: For the web server handling the game logic.
- **WinAPI**: For rendering the graphical interface on Windows.
- **Intel Threading Building Blocks (TBB)**: For optimizing parallel operations during grid drawing.
- **WinINet**: For handling HTTP requests on Windows to communicate with the Go server.

### How to Run:

1. **Compile the C++ Client**:
   - Use `g++` with MinGW to compile the C++ client code. Make sure to link against necessary libraries such as `TBB`, `GDI32`, and `WinINet`.
   
   ```bash
   g++ -I/mingw64/include -L/mingw64/lib tictactoe.cpp -ltbb12 -lgdi32 -lwininet -o tictactoe
   ```

2. **Run the Go Server**:
   - The Go server listens on port 8080. It serves as the backend logic that processes game moves.
   
   ```bash
   go run server.go
   ```
2. **Run the ./tictactoe**:
   - Run this on terminal.
   
   ```bash
   ./tictactoe
   ```

3. **Play the Game**:
   - After starting the server and compiling the client, run the `tictactoe` executable on your Windows machine to launch the graphical interface.
   - You can make moves by clicking on the grid in the GUI, which will communicate with the Go server to update the game state.
<img width="196" alt="image" src="https://github.com/user-attachments/assets/f306e3be-71f8-47b0-a4a4-a268846f2def" />
<img width="188" alt="image" src="https://github.com/user-attachments/assets/315543d3-a97c-4dd8-91f3-8074db84d59e" />


### License:
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

