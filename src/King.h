#ifndef KING_H_
#define KING_H_

#include "ChessPiece.h"
#include "Rook.h"

class King : public ChessPiece {
public:
    const static char IMAGE_WHITE = 'K';
    const static char IMAGE_BLACK = 'k';

    King(Player player, int row, int col, ChessPiece* (*chessBoard)[8][8]);
    virtual char getImage() const override;
    virtual void calculatePotentialMoves(bool potentialMoves[8][8]) const override;
    virtual void calculatePotentialCaptures(bool potentialCaptures[8][8]) const override;
    virtual ChessPiece* move(int row, int col) override;

    bool calculatePotentialCheck() const;
    void updateCheck();
    bool isCheck() const;

    bool hasCastled() const;
    bool hasCastledKingSide() const;
    bool hasCastledQueenSide() const;

    virtual bool canCastle(Side s) const override;
private:
    bool check;
    bool hasMoved;
    bool castledKingSide, castledQueenSide;
};

#endif /* KING_H_ */
