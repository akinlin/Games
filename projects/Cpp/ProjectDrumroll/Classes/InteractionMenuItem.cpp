//
//  InteractionMenuItem.cpp
//  ProjectDrumroll
//
//  Created by Alexander Kinlin on 8/17/13.
//
//

#include "InteractionMenuItem.h"
#include "ScreenHelper.h"

using namespace cocos2d;

const int INTERACTION_BAR_COST = 5;
const int MAX_BAR_LEVEL = 50;
const int STARTING_BAR_LEVEL = 20;
const int BUTTON_TEXTURE_HEIGHT = 250;
const int BUTTON_TEXTURE_WIDTH = 250;

enum MenuChildTags {
	kTagStencil,
	kTagFillBar,
	kTagOverlay
};

InteractionMenuItem::InteractionMenuItem(float  barLevel, const char* clipImage, const char* overlayImage)
{ 
	// create the menu item
	m_interactionMenuItem = CCClippingNode::create();
	m_interactionMenuItem->setAnchorPoint(CCPointZero);
	
	// create the stencil
	m_fillBarValue = barLevel;
	float percentage = m_fillBarValue / MAX_BAR_LEVEL;
	float barLevelHeight = BUTTON_TEXTURE_HEIGHT * percentage;
	m_stencil = shape(m_fillBarValue);
	m_stencil->setPosition(ccp(0, barLevelHeight - BUTTON_TEXTURE_HEIGHT));
	m_stencil->setAnchorPoint(CCPointZero);
	// set the stencil
	m_interactionMenuItem->setStencil(m_stencil);
	addChild(m_interactionMenuItem, kTagStencil, kTagStencil);

	// create the content
	m_fillBar = CCSprite::create(clipImage);
	m_fillBar->setAnchorPoint(CCPointZero);
	m_interactionMenuItem->addChild(m_fillBar, kTagFillBar, kTagFillBar);

	// creat the overlay
	m_overlay = CCSprite::create(overlayImage);
	m_overlay->setAnchorPoint(CCPointZero);
	addChild(m_overlay, kTagOverlay, kTagOverlay);
}

InteractionMenuItem::~InteractionMenuItem()
{

}

CCDrawNode* InteractionMenuItem::shape(int barLevel)
{
	float percentage = barLevel / MAX_BAR_LEVEL;
	float barLevelHeight = BUTTON_TEXTURE_HEIGHT * percentage;

	CCDrawNode *shape = CCDrawNode::create();
	static CCPoint rect[4];
	rect[0] = ccp(-250, -250);
	rect[1] = ccp(250, -250);
	rect[2] = ccp(250, 250);
	rect[3] = ccp(-250, 250);

	static ccColor4F green = { 0, 1, 0, 1 };
	shape->drawPolygon(rect, 4, green, 0, green);
	return shape;
}

float InteractionMenuItem::getBarLevel()
{
	return m_fillBarValue;
}

void InteractionMenuItem::setBarLevel(float barLevel)
{
	m_fillBarValue = barLevel;
	float percentage = m_fillBarValue / MAX_BAR_LEVEL;
	float barLevelHeight = BUTTON_TEXTURE_HEIGHT * percentage;
	m_stencil->setPosition(ccp(0, barLevelHeight - BUTTON_TEXTURE_HEIGHT));
}

void InteractionMenuItem::setStencil(float barLevel)
{
	// set the stencil
	m_fillBarValue = barLevel;
	float percentage = m_fillBarValue / MAX_BAR_LEVEL;
	float barLevelHeight = BUTTON_TEXTURE_HEIGHT * percentage;
	m_stencil = shape(m_fillBarValue);
	m_stencil->setPosition(ccp(0, barLevelHeight - BUTTON_TEXTURE_HEIGHT));
	m_stencil->setAnchorPoint(CCPointZero);
}

void InteractionMenuItem::setClippingImage(const char* clipImage)
{

}
void InteractionMenuItem::setOverlayImage(const char* overlayImage)
{

}