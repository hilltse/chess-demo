#include "King.h"
#include "helper.h"

King::King(Player player, int row, int col, ChessPiece* (*chessBoard)[8][8])
:
  ChessPiece (player, row, col, chessBoard),
  check(false),
  hasMoved(false),
  castledKingSide(false),
  castledQueenSide(false)
{}

char King::getImage() const
{
    if (player == WHITE)
        return IMAGE_WHITE;

    return IMAGE_BLACK;
}

void King::calculatePotentialMoves(bool potentialMoves[8][8]) const
{
    // check the 3x3 grid around the king
    checkGrid(potentialMoves);

    if (canCastle(KING_SIDE) && (*chessBoard)[row][7] != nullptr && (*chessBoard)[row][7]->canCastle(KING_SIDE))
        potentialMoves[row][6] = true;

    if (canCastle(QUEEN_SIDE) && (*chessBoard)[row][0] != nullptr && (*chessBoard)[row][0]->canCastle(QUEEN_SIDE))
        potentialMoves[row][2] = true;
}

void King::calculatePotentialCaptures(bool potentialMoves[8][8]) const
{
    ChessPiece::calculatePotentialCaptures(potentialMoves);
}

ChessPiece* King::move(int row, int col)
{
    // handle castling
    if (col == 2 && canCastle(QUEEN_SIDE))
    {
        // move rook
        (*chessBoard)[row][0]->move(row, col+1);
        castledQueenSide = true;
    }
    else if (col == 6 && canCastle(KING_SIDE))
    {
        (*chessBoard)[row][7]->move(row, col-1);
        castledKingSide = true;
    }

    ChessPiece* captured = ChessPiece::move(row, col);
    hasMoved = true;

    return captured;
}

bool King::calculatePotentialCheck() const
{
    bool canCapture[8][8] = {{false}};

    // brute force search
    for (int i = 0; i<8; ++i)
    {
        for (int j = 0; j<8; ++j)
        {
            if (isEnemyChessPiece((*chessBoard)[i][j]))
            {
                if ((*chessBoard)[i][j] != nullptr)
                    (*chessBoard)[i][j]->calculatePotentialCaptures(canCapture);

                if (canCapture[row][col] == true)
                    return true;

                clear_grid(canCapture);
            }
        }
    }

    return false;
}

void King::updateCheck()
{
    check = calculatePotentialCheck();
}

bool King::isCheck() const
{
    return check;
}

bool King::hasCastled() const
{
    return castledKingSide || castledQueenSide;
}

bool King::hasCastledKingSide() const
{
    return castledKingSide;
}

bool King::hasCastledQueenSide() const
{
    return castledQueenSide;
}

bool King::canCastle(Side castlingSide) const
{
    /*  There are many preconditions before Castling can be performed:
     *  1.  It must be the King's First Move in the whole game, i.e. the King has never moved yet.
     *  2.  It must be chosen Rook's First Move in the whole game, i.e. the chosen Rook has never moved yet.
     *  3.  There cannot be any other Chess Pieces between the King and the chosen Rook.
     *  4.  The King is not currently in Check, does not pass through a Square where he would have been in Check,
     *      and does not end up in a Square where he is in Check.
     */

    if (castlingSide == KING_SIDE)
    {
        if ( hasMoved ||
               check ||
               (*chessBoard)[row][5] != nullptr ||
               (*chessBoard)[row][6] != nullptr )
            return false;


        return true;
    }

    // castlingSide == QUEEN_SIDE

    if ( hasMoved ||
           check ||
           (*chessBoard)[row][3] != nullptr ||
           (*chessBoard)[row][2] != nullptr ||
           (*chessBoard)[row][1] != nullptr )
        return false;



    return true;
}
