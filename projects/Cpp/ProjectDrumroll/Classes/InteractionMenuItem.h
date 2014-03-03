//
//  InteractionMenuItem.h
//  ProjectDrumroll
//
//  Created by Alexander Kinlin on 8/17/13.
//
//

#ifndef __ProjectDrumroll__InteractionMenuItem__
#define __ProjectDrumroll__InteractionMenuItem__


#include "cocos2d.h"
#include "SpritHelper.h"
USING_NS_CC;

class InteractionMenuItem : public CCLayer
{
public:
	InteractionMenuItem(float  barLevel, const char* clipImage, const char* overlayImage);
	~InteractionMenuItem();

	void setBarLevel(float barLevel);
	float getBarLevel();

	CCDrawNode* shape(int barLevel);
   
private:
	// interaction button values
	float m_fillBarValue;
	CCNode* m_stencil;
	CCClippingNode* m_interactionMenuItem;
	CCSprite* m_fillBar;
	CCSprite* m_overlay;

	void setStencil(float barLevel);
	void setClippingImage(const char* clipImage);
	void setOverlayImage(const char* overlayImage);
	void addToBar(int addition);
};

#endif /* defined(__ProjectDrumroll__GamePiece__) */
