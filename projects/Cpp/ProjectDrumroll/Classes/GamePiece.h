//
//  GamePiece.h
//  ProjectDrumroll
//
//  Created by Alexander Kinlin on 8/17/13.
//
//

#ifndef __ProjectDrumroll__GamePiece__
#define __ProjectDrumroll__GamePiece__


#include "cocos2d.h"
#include "SpritHelper.h"
USING_NS_CC;

class GamePiece : public CCSprite
{
public:
    GamePiece();
    //GamePiece(GamePiece& gamePiece);
    
    int getPieceColor();
    int getInteractionType();
	void setInteractionType(int interactionIndex);
    
    int getTextureWidth();
    int getTextureHeight();
    
    void switchToRandomPiece();
    void switchToNextPiece();
    
    bool isInElinationCheck();
    void setElinationCheck(bool isInEliminationCheck);
    
    bool isActive();
    void setActive(bool isActive);
    
private:
    int s_color;
    int s_interactionType;
    Blocks* m_BlocksSprite;
    CCLabelTTF* m_interactionSprite;
    
    bool m_isInElinationCheck;
    bool m_isActive;
};

enum gamePieceInteractionType
{
	pieceInteractionEmpty = 0,
	pieceInteractionFlip = 1,
	pieceInteractionDPadFlip = 2,
	pieceInteractionSwitch = 3,
	pieceInteractionCount = 4
};

// TODO: review if this should be part of the spritehelper class
enum gamePieceColor
{
	pieceColorYellow = 0,
	pieceColorPurple = 1,
	pieceColorRed = 2,
	pieceColorGreen = 3,
	gamePieceColorCount = 4
};

#endif /* defined(__ProjectDrumroll__GamePiece__) */
