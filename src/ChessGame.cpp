#include <string>

#include "ChessGame.h"
#include "helper.h"

ChessGame::ChessGame()
:
  chessBoard{{nullptr}},
  whiteKing(new King(WHITE, 0, 4, &chessBoard)),
  blackKing(new King(BLACK, 7, 4, &chessBoard)),
  pawnFirstMove(nullptr),
  selectedChessPiece(nullptr),
  selectedChessPieceValidMoves{{false}},
  whiteCastled(false),
  blackCastled(false),
  currentPlayer(WHITE)
{
    // set up the chess board
    for (int j = 0; j<8; ++j) //j == column
    {
        new Pawn(WHITE, 1, j, &chessBoard);
        new Pawn(BLACK, 6, j, &chessBoard);

        switch(j)
        {
        case 0: case 7:
            new Rook(WHITE, 0, j, &chessBoard);
            new Rook(BLACK, 7, j, &chessBoard);
            break;
        case 1: case 6:
            new Knight(WHITE, 0, j, &chessBoard);
            new Knight(BLACK, 7, j, &chessBoard);
            break;
        case 2: case 5:
            new Bishop(WHITE, 0, j, &chessBoard);
            new Bishop(BLACK, 7, j, &chessBoard);
            break;
        case 3:
            new Queen(WHITE, 0, j, &chessBoard);
            new Queen(BLACK, 7, j, &chessBoard);
            break;
        }
    }
}
ChessGame::~ChessGame()
{
    for (int i = 0; i<8; ++i)
        for (int j = 0; j<8; ++j)
            if (chessBoard[i][j] != nullptr)
                delete chessBoard[i][j];
}

King* ChessGame::getPlayerKing(Player player) const
{
    return player == WHITE ? whiteKing : blackKing;
}

bool ChessGame::hasValidMoves(Player player) const
{
    bool existsMove = false;
    bool validMoves[8][8] = {{false}};
    ChessPiece* curr = nullptr;

    // We traverse through the whole chess board
    for (int i = 0; i<8 && !existsMove; ++i)
        for (int j = 0; j<8 && !existsMove; ++j)
        {
            curr = chessBoard[i][j];
            if (curr != nullptr && curr->player == player)
            {
                calculateValidMoves(curr, validMoves);

                for (int k = 0; k<8 && !existsMove; ++k)
                    for (int l = 0; l<8 && !existsMove; ++l)
                        if (validMoves[k][l])
                            existsMove = true;
            }
        }

    return existsMove;
}

void ChessGame::calculateValidMoves(ChessPiece* chessPiece, bool validMoves[8][8]) const
{
    if (chessPiece == nullptr) return;

    clear_grid(validMoves);

    chessPiece->calculatePotentialMoves(validMoves);

    for (int i = 0; i<8; ++i)
        for (int j = 0; j<8; ++j)
        {
            if (validMoves[i][j])
            {
                chessPiece->potentialMove(i, j);

                if (chessPiece->player == WHITE)
                {
                    // if the move leaves its king in check, then it isn't valid
                    validMoves[i][j] = !(whiteKing->calculatePotentialCheck());
                }
                else
                {
                    validMoves[i][j] = !(blackKing->calculatePotentialCheck());
                }

                chessPiece->undoPotentialMove();
            }
        }

    // eliminate invalid castling
    if (dynamic_cast<King*>(chessPiece) == whiteKing)
    {
        if (whiteKing->canCastle(ChessPiece::KING_SIDE))
        {
            whiteKing->potentialMove(0, 5);
            validMoves[0][6] = !(whiteKing->calculatePotentialCheck());
            whiteKing->undoPotentialMove();
        }

        if (whiteKing->canCastle(ChessPiece::QUEEN_SIDE))
        {
            whiteKing->potentialMove(0, 3);
            validMoves[0][2] = !(whiteKing->calculatePotentialCheck());
            whiteKing->undoPotentialMove();
        }
    }
    else if(dynamic_cast<King*>(chessPiece) == blackKing)
    {
        if (blackKing->canCastle(ChessPiece::KING_SIDE))
        {
            blackKing->potentialMove(7, 5);
            validMoves[7][6] = !(blackKing->calculatePotentialCheck());
            blackKing->undoPotentialMove();
        }

        if (blackKing->canCastle(ChessPiece::QUEEN_SIDE))
        {
            blackKing->potentialMove(7, 3);
            validMoves[7][2] = !(blackKing->calculatePotentialCheck());
            blackKing->undoPotentialMove();
        }
    }
}

void ChessGame::promotePawn(ChessPiece* pawn, char choice)
{
    if (pawn == nullptr) return;

    int row = pawn->getRow();
    int col = pawn->getCol();
    Player player = pawn->player;

    delete pawn;

    switch(choice)
    {
    case 'Q':
        selectedChessPiece = new Queen(player, row, col, &chessBoard);
        break;
    case 'B':
        selectedChessPiece = new Bishop(player, row, col, &chessBoard);
        break;
    case 'N':
        selectedChessPiece = new Knight(player, row, col, &chessBoard);
        break;
    case 'R':
        selectedChessPiece = new Rook(player, row, col, &chessBoard);
        break;
    }
}

void ChessGame::selectChessPiece(int row, int col)
{
    if (chessBoard[row][col] == nullptr)
    {
        selectedChessPiece = nullptr;
    }
    else
    {
        selectedChessPiece = chessBoard[row][col]; // can be enemy
        calculateValidMoves(selectedChessPiece, selectedChessPieceValidMoves);
    }
}

void ChessGame::moveSelectedChessPiece(int row, int col, char promote = 0)
{
    if (selectedChessPiece == nullptr) return;
    ChessPiece* capture = selectedChessPiece->move(row, col);

    if (capture != nullptr)
    {
        if (capture == pawnFirstMove)
            pawnFirstMove = nullptr;

        delete capture;
    }

    // handle pawn first move
    if (selectedChessPiece->hasFirstMoved())
        pawnFirstMove = selectedChessPiece;

    // handle pawn promotion
    if (selectedChessPiece->canPromote())
        promotePawn(selectedChessPiece, promote);

    nextPlayerTurn();
}

void ChessGame::nextPlayerTurn()
{
    // reset highlighting and selection
    selectedChessPiece = nullptr;
    clear_grid(selectedChessPieceValidMoves);

    // handle pass turn en passant
    if (pawnFirstMove != nullptr && pawnFirstMove->player != currentPlayer)
    {
        pawnFirstMove->passTurnEnPassant();
        pawnFirstMove = nullptr;
    }

    // handle check
    if (currentPlayer == WHITE)
    {
        blackKing->updateCheck();
        currentPlayer = BLACK;
    }
    else
    {
        whiteKing->updateCheck();
        currentPlayer = WHITE;
    }
}


void ChessGame::updateCastling(Player p)
{
    if (p == WHITE)
    {
        if (whiteCastled) return;
        whiteCastled = true;
    }
    else
    {
        if (blackCastled) return;
        blackCastled = true;
    }
}
