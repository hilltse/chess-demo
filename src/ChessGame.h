#ifndef CHESSGAME_H_
#define CHESSGAME_H_
#include "Player.h"
#include "ChessPiece.h"
#include "King.h"
#include "Queen.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "Pawn.h"

class ChessGame {
public:
    void ChessGame::calculateValidMoves(ChessPiece* chessPiece, bool validMoves[8][8]) const;
    void selectChessPiece(int row, int col); // Updates selectedChessPiece and selectedChessPieceValidMoves.
    void moveSelectedChessPiece(int row, int col, char promote = 0); // Process movement of selectedChessPiece to the specified position.
    bool hasValidMoves(Player player) const;

    ChessGame();
    ~ChessGame();
private:
    ChessPiece* chessBoard[8][8]; // The internal ChessBoard. Passed to all ChessPieces.
    King* const whiteKing;
    King* const blackKing;
    ChessPiece* pawnFirstMove; // For En Passant Timing Rules.
    ChessPiece* selectedChessPiece;
    bool selectedChessPieceValidMoves[8][8];
    bool whiteCastled;
    bool blackCastled;
    Player currentPlayer;

    King* getPlayerKing(Player player) const;
    void updateCastling(Player p);
    void promotePawn(ChessPiece* pawn, char choice); // Pawn will be promoted into either Queen, Rook, Knight, or Bishop, when it reaches the last row on the opposite side.
    void nextPlayerTurn(); // Will also check if Checkmate/Stalemate.
};

#endif /* CHESSGAME_H_ */
