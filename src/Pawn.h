#ifndef PAWN_H_
#define PAWN_H_

#include "ChessPiece.h"

class Pawn : public ChessPiece {
public:
    const static char IMAGE_WHITE = 'P';
	const static char IMAGE_BLACK = 'p';
    enum class Direction {LEFT, RIGHT};

    Pawn(Player player, int row, int col, ChessPiece* (*chessBoard)[8][8]);
	virtual char getImage() const override;

    virtual void calculatePotentialMoves(bool potentialMoves[8][8]) const override;
    virtual void calculatePotentialCaptures(bool potentialCaptures[8][8]) const override;
    virtual ChessPiece* move(int row, int col) override; // First Move and En Passant.

    virtual bool hasFirstMoved() const override;
    virtual bool hasCapturedEnPassant(int& rowEnPassant, int& colEnPassant) const override;
    virtual void passTurnEnPassant() override;
    virtual bool canPromote() const override;

private:
    bool hasMoved; // Pawn has First Move if still have not moved the whole game.
    bool canBeEnPassant; // En Passant only valid for the immediate next turn.
    bool capturedEnPassant; // Can be captured by enemy En Passant
    int rowEnPassant; // Row of enemy Pawn captured En Passant.
    int colEnPassant; // Col of enemy Pawn captured En Passant.

    bool canEnPassant(Direction d) const;
};

#endif /* PAWN_H_ */
