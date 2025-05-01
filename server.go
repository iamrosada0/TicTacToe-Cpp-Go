package main

import (
	"encoding/json"
	"fmt"
	"net/http"
)

type GameState struct {
	Board       [][]string `json:"board"`
	Size        int        `json:"size"`
	CurrentTurn string     `json:"currentTurn"`
	Winner      string     `json:"winner"`
}

func NewGame(size int) *GameState {
	board := make([][]string, size)
	for i := range board {
		board[i] = make([]string, size)
	}
	return &GameState{
		Board:       board,
		Size:        size,
		CurrentTurn: "X",
	}
}

func (g *GameState) MakeMove(row, col int) bool {
	if row < 0 || row >= g.Size || col < 0 || col >= g.Size || g.Board[row][col] != "" || g.Winner != "" {
		return false
	}
	g.Board[row][col] = g.CurrentTurn
	g.checkWinner()
	if g.Winner == "" {
		g.CurrentTurn = map[string]string{"X": "O", "O": "X"}[g.CurrentTurn]
	}
	return true
}

func (g *GameState) checkWinner() {
	for i := 0; i < g.Size; i++ {
		if g.Board[i][0] != "" && allEqual(g.Board[i]) {
			g.Winner = g.Board[i][0]
			return
		}
	}
	for j := 0; j < g.Size; j++ {
		col := make([]string, g.Size)
		for i := 0; i < g.Size; i++ {
			col[i] = g.Board[i][j]
		}
		if col[0] != "" && allEqual(col) {
			g.Winner = col[0]
			return
		}
	}
	
	diag := make([]string, g.Size)
	for i := 0; i < g.Size; i++ {
		diag[i] = g.Board[i][i]
	}
	
	if diag[0] != "" && allEqual(diag) {
		g.Winner = diag[0]
		return
	}
	
	diag = make([]string, g.Size)
	for i := 0; i < g.Size; i++ {
		diag[i] = g.Board[i][g.Size-1-i]
	}
	if diag[0] != "" && allEqual(diag) {
		g.Winner = diag[0]
		return
	}

	
	for i := 0; i < g.Size; i++ {
		for j := 0; j < g.Size; j++ {
			if g.Board[i][j] == "" {
				return
			}
		}
	}
	g.Winner = "Draw"
}


func allEqual(slice []string) bool {
	for i := 1; i < len(slice); i++ {
		if slice[i] != slice[0] {
			return false
		}
	}
	return true
}

func main() {
	game := NewGame(3) 
	http.HandleFunc("/move", func(w http.ResponseWriter, r *http.Request) {
		var move struct {
			Row int `json:"row"`
			Col int `json:"col"`
		}
		if err := json.NewDecoder(r.Body).Decode(&move); err != nil {
			http.Error(w, "Invalid request", 400)
			return
		}
		success := game.MakeMove(move.Row, move.Col)
		if !success {
			http.Error(w, "Invalid move", 400)
			return
		}
		json.NewEncoder(w).Encode(game)
	})
	fmt.Println("Server running on :8080")
	http.ListenAndServe(":8080", nil)
}
