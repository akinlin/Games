//
//  InteractionMenu.h
//  ProjectDrumroll
//
//  Created by Alexander Kinlin on 11/10/13.
//
//

#ifndef __ProjectDrumroll__INTERACTIONMENU__
#define __ProjectDrumroll__INTERACTIONMENU__

#include "cocos2d.h"
#include "InteractionMenuItem.h"

USING_NS_CC;

class InteractionMenu : public CCLayer
{
public:
	InteractionMenu();
	~InteractionMenu();

private:
	
};

class GameInteractionMenu : public InteractionMenu
{
public:
	GameInteractionMenu();
	~GameInteractionMenu();

	virtual void onEnter();
	virtual void onExit();
	virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
	virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);

	void menuInteractionCallback(CCObject* pSender);
	void menuEliminateCallback(CCObject* pSender);

	void interactionComplete();
	void cancelInteraction();
	void addToBarLevel(int pieceColor, int valueToAdd);

private:
	// Eliminate and Interact Buttons (should be in the HUD)
	CCMenu* m_touchStateMenu;
	InteractionMenuItem *flipButtonClipper;
	InteractionMenuItem *switchButtonClipper;
	InteractionMenuItem *dpadButtonClipper;
	InteractionMenuItem *slideButtonClipper;

	CCSprite* eliminateButton;

	// interaction button values
	float m_flipBar;
	float m_dpadBar;
	float m_switchBar;
	float m_slideBar;
	float m_rotaryBar;

	CCNode* flipStencil;
	CCNode* switchStencil;
	CCNode* dpadStencil;
	CCNode* slideStencil;

	void addToFlipBar(int addition);
	void addToSwitchBar(int addition);
	void addToDPadBar(int addition);
	void addToSlideBar(int addition);
	void addToRotaryBar(int addition);

	void subPoints(int points);
};

class EditorInteractionMenu : public InteractionMenu
{
public:
	EditorInteractionMenu();
	~EditorInteractionMenu();

private:
	
};

#endif /* defined(__ProjectDrumroll__INTERACTIONMENU__) */
