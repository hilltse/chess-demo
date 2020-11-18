#include "Pawn.h"

Pawn::Pawn(Player player, int row, int col, ChessPiece* (*chessBoard)[8][8])
:
  ChessPiece(player, row, col, chessBoard),
  hasMoved(false),
  canBeEnPassant(false),
  capturedEnPassant(false),
  rowEnPassant(0),
  colEnPassant(0)
{
    canBeEnPassant = false;
}

char Pawn::getImage() const
{
    if (player == WHITE)
        return IMAGE_WHITE;

    return IMAGE_BLACK;
}

bool Pawn::canEnPassant(Direction d) const
{
    if (row != 3 && row != 4) return false;

    if (d == Direction::LEFT)
        return col-1 >= 0 &&
               (*chessBoard)[row][col-1] != nullptr &&
               (*chessBoard)[row][col-1]->hasFirstMoved();

    // assume d == Direction::RIGHT
    return col+1 < 8 &&
           (*chessBoard)[row][col+1] != nullptr &&
           (*chessBoard)[row][col+1]->hasFirstMoved();
}

void Pawn::calculatePotentialMoves(bool potentialMoves[8][8]) const
{
    if (player == WHITE)
    {
        if (row+1<8 && (*chessBoard)[row+1][col] == nullptr)
        {
            potentialMoves[row+1][col] = true;

            if (!hasMoved && (*chessBoard)[row+2][col] == nullptr)
                potentialMoves[row+2][col] = true;
        }

        if (row+1<8 && col-1>=0 && isEnemyChessPiece((*chessBoard)[row+1][col-1]))
            potentialMoves[row+1][col-1] = true;

        if (row+1<8 && col+1<8 && isEnemyChessPiece((*chessBoard)[row+1][col+1]))
            potentialMoves[row+1][col+1] = true;

        if (canEnPassant(Direction::LEFT))
            potentialMoves[row+1][col-1] = true;

        if (canEnPassant(Direction::RIGHT))
            potentialMoves[row+1][col+1] = true;
    }
    else // player == BLACK
    {
        if (row-1>=0 && (*chessBoard)[row-1][col] == nullptr)
        {
            potentialMoves[row-1][col] = true;

            if (!hasMoved && (*chessBoard)[row-2][col] == nullptr)
                potentialMoves[row-2][col] = true;
        }

        if (row-1>=0 && col-1>=0 && isEnemyChessPiece((*chessBoard)[row-1][col-1]))
            potentialMoves[row-1][col-1] = true;

        if (row-1>=0 && col+1<8 && isEnemyChessPiece((*chessBoard)[row-1][col+1]))
            potentialMoves[row-1][col+1] = true;

        if (canEnPassant(Direction::LEFT))
            potentialMoves[row-1][col-1] = true;

        if (canEnPassant(Direction::RIGHT))
            potentialMoves[row-1][col+1] = true;
    }
}

void Pawn::calculatePotentialCaptures(bool potentialMoves[8][8]) const
{
    ChessPiece::calculatePotentialCaptures(potentialMoves);

    if (canEnPassant(Direction::LEFT))
        potentialMoves[row][col-1] = true;

    if (canEnPassant(Direction::RIGHT))
        potentialMoves[row][col+1] = true;
}

ChessPiece* Pawn::move(int row, int col)
{
    ChessPiece* captured = nullptr;

    // handle en passant
    if (canEnPassant(Direction::LEFT) && col == this->col-1)
    {
        capturedEnPassant = true;
        rowEnPassant = row;
        colEnPassant = col-1;

        captured = (*chessBoard)[this->row][this->col-1];
        (*chessBoard)[this->row][this->col-1] = nullptr;
    }
    else if (canEnPassant(Direction::RIGHT) && col == this->col+1)
    {
        capturedEnPassant = true;
        rowEnPassant = row;
        colEnPassant = col+1;

        captured = (*chessBoard)[this->row][this->col+1];
        (*chessBoard)[this->row][this->col+1] = nullptr;
    }
    else
    {
        captured = (*chessBoard)[row][col];
    }

    ChessPiece::move(row, col);

    if (!hasMoved)
    {
        hasMoved = true;
        canBeEnPassant = true;
    }
    else
        canBeEnPassant = false;

    return captured;
}

bool Pawn::hasFirstMoved() const
{
    return canBeEnPassant;
}

bool Pawn::hasCapturedEnPassant(int& rowEnPassant, int& colEnPassant) const
{
    if (capturedEnPassant)
    {
        rowEnPassant = this->rowEnPassant;
        colEnPassant = this->colEnPassant;

        return true;
    }

    return false;
}

void Pawn::passTurnEnPassant()
{
    canBeEnPassant = false;
}

bool Pawn::canPromote() const
{
    if (player == WHITE)
        return row == 7;
    else
        return row == 0;

}
