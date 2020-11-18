#include "Knight.h"

Knight::Knight(Player player, int row, int col, ChessPiece* (*chessBoard)[8][8])
:
  ChessPiece (player, row, col, chessBoard)
{}

char Knight::getImage() const
{
    if (player == WHITE)
        return IMAGE_WHITE;

    return IMAGE_BLACK;
}

void Knight::checkInside(int j, bool potentialMoves[8][8]) const
{
    if (row-2 >= 0 && row-2 < 8)
        checkTile(row-2, j, potentialMoves);

    if (row+2 >= 0 && row+2 < 8)
        checkTile(row+2, j, potentialMoves);
}

void Knight::checkOutside(int j, bool potentialMoves[8][8]) const
{
    if (row-1 >= 0 && row-1 < 8)
        checkTile(row-1, j, potentialMoves);

    if (row+1 >= 0 && row+1 < 8)
        checkTile(row+1, j, potentialMoves);
}

void Knight::calculatePotentialMoves(bool potentialMoves[8][8]) const
{
    // brute force
    for (int j = col-2; j <= col+2; ++j)
    {
        if (j>=0 && j<8)
        {
            switch(j-col)
            {
            case -2:
                checkOutside(j, potentialMoves);
                break;
            case -1:
                checkInside(j, potentialMoves);
                break;
            case 1:
                checkInside(j, potentialMoves);
                break;
            case 2:
                checkOutside(j, potentialMoves);
                break;
            }
        }
    }
}
