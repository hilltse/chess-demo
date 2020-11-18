#include "Bishop.h"

Bishop::Bishop(Player player, int row, int col, ChessPiece* (*chessBoard)[8][8])
:
  ChessPiece (player, row, col, chessBoard)
{}

char Bishop::getImage() const
{
    if (player == WHITE)
        return IMAGE_WHITE;

    return IMAGE_BLACK;
}

void Bishop::calculatePotentialMoves(bool potentialMoves[8][8]) const
{
    checkDiagonal(potentialMoves);
}
