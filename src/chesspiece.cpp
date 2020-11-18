#include "ChessPiece.h"

ChessPiece::ChessPiece(Player player, int row, int col, ChessPiece* (*chessBoard)[8][8])
:
  player(player),
  row(row),
  col(col),
  previousRow(row),
  previousCol(col),
  potentialCapturedChessPiece(nullptr),
  chessBoard(chessBoard)
{
    (*chessBoard)[row][col] = this;
}

int ChessPiece::getRow() const
{
    return row;
}

int ChessPiece::getCol() const
{
    return col;
}

bool ChessPiece::isEnemyChessPiece(const ChessPiece* const chessPiece) const
{
    if (chessPiece == nullptr || chessPiece->player == player)
        return false;

    return true;
}

void ChessPiece::calculatePotentialCaptures(bool potentialCaptures[8][8]) const
{
    calculatePotentialMoves(potentialCaptures);

    for (int i = 0; i<8; ++i)
    {
        for (int j = 0; j<8; ++j)
        {
            if (potentialCaptures[i][j] && !isEnemyChessPiece((*chessBoard)[i][j]))
                potentialCaptures[i][j] = false;
        }
    }
}

void ChessPiece::potentialMove(int row, int col)
{
    previousRow = this->row;
    previousCol = this->col;
    potentialCapturedChessPiece = (*chessBoard)[row][col];

    this->row = row;
    this->col = col;
    (*chessBoard)[row][col] = this;
    (*chessBoard)[previousRow][previousCol] = nullptr;
}

void ChessPiece::undoPotentialMove()
{
    (*chessBoard)[row][col] = potentialCapturedChessPiece;
    potentialCapturedChessPiece = nullptr;

    row = previousRow;
    col = previousCol;

    (*chessBoard)[row][col] = this;
}

ChessPiece* ChessPiece::move(int row, int col)
{
    ChessPiece* captured = (*chessBoard)[row][col];

    (*chessBoard)[this->row][this->col] = nullptr;

    this->row = row;
    this->col = col;
    (*chessBoard)[row][col] = this;

    return captured;
}

// Helper function
bool ChessPiece::checkTile(int i, int j, bool potentialMoves[8][8]) const
{
    if (i<0 || i>=8 || j<0 || j>=8)
        return true;

    if ((*chessBoard)[i][j] == nullptr)
    {
        potentialMoves[i][j] = true;
        return false;
    }

    if (isEnemyChessPiece((*chessBoard)[i][j]))
        potentialMoves[i][j] = true;

    return true;
}

void ChessPiece::checkGrid(bool potentialMoves[8][8]) const
{
    for (int i = row-1; i<=row+1; ++i)
    {
        if (i>=0 && i<8)
        {
            for (int j = col-1; j<=col+1; ++j)
            {
                if (j>=0 && j<8)
                {
                    checkTile(i, j, potentialMoves);
                }
            }
        }
    }
}

void ChessPiece::checkCross(bool potentialMoves[8][8]) const
{
    bool blocked = false;

    // check forward
    for (int i = row+1; i<8 && !blocked; ++i)
    {
        blocked = checkTile(i, col, potentialMoves);
    }

    // check backward
    blocked = false;

    for (int i = row-1; i>=0 && !blocked; --i)
    {
        blocked = checkTile(i, col, potentialMoves);
    }

    // check left
    blocked = false;

    for (int j = col-1; j>=0 && !blocked; --j)
    {
        blocked = checkTile(row, j, potentialMoves);
    }

    // check right
    blocked = false;

    for (int j = col+1; j<8 && !blocked; ++j)
    {
        blocked = checkTile(row, j, potentialMoves);
    }
}

void ChessPiece::checkDiagonal(bool potentialMoves[8][8]) const
{
    bool blocked = false;

    // check upperright direction
    for (int i = row+1, j = col+1;
         i<8 && j<8 && !blocked;
         ++i, ++j)
    {
        blocked = checkTile(i, j, potentialMoves);
    }

    //check lowerright direction
    blocked = false;

    for (int i = row-1, j = col+1;
         i>=0 && j<8 && !blocked;
         --i, ++j)
    {
        blocked = checkTile(i, j, potentialMoves);
    }

    //check upperleft direction
    blocked = false;

    for (int i = row+1, j = col-1;
         i<8 && j>=0 && !blocked;
         ++i, --j)
    {
        blocked = checkTile(i, j, potentialMoves);
    }

    //check lowerleft direction
    blocked = false;

    for (int i = row-1, j = col-1;
         i >= 0 && j >=0 && !blocked;
         --i, --j)
    {
        blocked = checkTile(i, j, potentialMoves);
    }

}

bool ChessPiece::canCastle(Side) const
{
    return false;
}

bool ChessPiece::hasFirstMoved() const
{
    return false;
}

void ChessPiece::passTurnEnPassant()
{
}

bool ChessPiece::canPromote() const
{
    return false;
}

bool ChessPiece::hasCapturedEnPassant(int&, int&) const
{
    return false;
}




