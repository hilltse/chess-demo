#include "Rook.h"

Rook::Rook(Player player, int row, int col, ChessPiece* (*chessBoard)[8][8])
:
  ChessPiece(player, row, col, chessBoard),
  hasMoved(false)
{}

char Rook::getImage() const
{
    if (player == WHITE)
        return IMAGE_WHITE;

    return IMAGE_BLACK;
}

void Rook::calculatePotentialMoves(bool potentialMoves[8][8]) const
{
    checkCross(potentialMoves);
}

ChessPiece* Rook::move(int row, int col)
{
    ChessPiece* captured = ChessPiece::move(row, col);
    hasMoved = true;

    return captured;
}

bool Rook::canCastle(Side s) const
{   
    if (s == KING_SIDE)
    {
        return !hasMoved &&
               (*chessBoard)[row][5] == nullptr &&
               (*chessBoard)[row][6] == nullptr;
    }

    // castlingSide == QUEEN_SIDE

    return !hasMoved &&
           (*chessBoard)[row][3] == nullptr &&
           (*chessBoard)[row][2] == nullptr &&
           (*chessBoard)[row][1] == nullptr;
}
