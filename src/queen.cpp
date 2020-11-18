#include "Queen.h"

Queen::Queen(Player player, int row, int col, ChessPiece* (*chessBoard)[8][8])
:
  ChessPiece(player, row, col, chessBoard)
{}

char Queen::getImage() const
{
    if (player == WHITE)
        return IMAGE_WHITE;

    return IMAGE_BLACK;
}

void Queen::calculatePotentialMoves(bool potentialMoves[8][8]) const
{
    checkCross(potentialMoves);
    checkDiagonal(potentialMoves);
}
