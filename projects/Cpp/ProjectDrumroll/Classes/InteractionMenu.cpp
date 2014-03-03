//
//  InteractionMenu.cpp
//  ProjectDrumroll
//
//  Created by Alexander Kinlin on 8/24/13.
//
//

#include "InteractionMenu.h"
#include "ScreenHelper.h"
#include "GameScene.h"
#include "GamePiece.h"
#include "InteractionMenuItem.h"

const int INTERACTION_BAR_COST = 5;
const int INTERACTION_USE_DEDUCTION = 20;
const int MAX_BAR_LEVEL = 50;
const int BUTTON_TEXTURE_HEIGHT = 250;
const int BUTTON_TEXTURE_WIDTH = 250;

//
// testing variables not meant to stay in
//
bool interactionFlipOn = false;
bool interactionSwitchOn = false;
bool interactionDPadOn = false;
bool interactionSlideOn = false;
bool interactionRotaryOn = false;
bool interactionEliminationOn = false;

enum ButtonMenuTags {
	kTagInteractButtonReference,
	kTagG_InteractButtonReference,
	kTagY_InteractButtonReference,
	kTagP_InteractButtonReference,
	kTagO_InteractButtonReference,
	kTagEliminationButtonReference
};

InteractionMenu::InteractionMenu()
{
	
}

InteractionMenu::~InteractionMenu()
{
	
}

GameInteractionMenu::GameInteractionMenu()
{
	CCSize s = VisibleRect::getScreenSize();

	// turn on touch events
	setTouchEnabled(true);

	//// blue interation button (pieceInteractionFlip)
	flipButtonClipper = new InteractionMenuItem(20, "win32/1_eliminateButton.png", "win32/flip_overlay_large.png");
	if (flipButtonClipper)
	{
		flipButtonClipper->setPosition(ccp(0, (s.height / 2) + BUTTON_TEXTURE_HEIGHT));
		addChild(flipButtonClipper);
	}

	//// green interation button (pieceInteractionSwitch)
	switchButtonClipper = new InteractionMenuItem(20, "win32/g_eliminateButton.png", "win32/switch_overlay_large.png");
	if (switchButtonClipper)
	{
		switchButtonClipper->setPosition(ccp(0, (s.height / 2)));
		addChild(switchButtonClipper);
	}

	//// yellow interation button (pieceInteractionDPad)
	dpadButtonClipper = new InteractionMenuItem(20, "win32/y_1_eliminateButton.png", "win32/dpad_overlay_large.png");
	if (dpadButtonClipper)
	{
		dpadButtonClipper->setPosition(ccp(0, (s.height / 2) - BUTTON_TEXTURE_HEIGHT));
		addChild(dpadButtonClipper);
	}

	//// purple interation button (pieceInteractionSlide)
	slideButtonClipper = new InteractionMenuItem(20, "win32/p_1_eliminateButton.png", "win32/slide_overlay_large.png");
	if (slideButtonClipper)
	{
		slideButtonClipper->setPosition(ccp(0, (s.height / 2) - (BUTTON_TEXTURE_HEIGHT * 2)));
		addChild(slideButtonClipper);
	}

	//// green interation button (pieceInteractionSwitch)
	// set the stencil
	//m_switchBar = 20;
	//percentage = m_switchBar / MAX_BAR_LEVEL;
	//barLevelHeight = BUTTON_TEXTURE_HEIGHT * percentage;
	//switchStencil = shape(m_switchBar);
	//switchStencil->setPosition(ccp(0, barLevelHeight - BUTTON_TEXTURE_HEIGHT));
	//switchStencil->setAnchorPoint(CCPointZero);
	////// set the clipper
	//switchButtonClipper = CCClippingNode::create();
	//switchButtonClipper->setAnchorPoint(CCPointZero);
	//switchButtonClipper->setPosition(ccp(0, (s.height / 2)));
	//switchButtonClipper->setStencil(switchStencil);
	//addChild(switchButtonClipper, 0);
	//// set the content
	//CCSprite* switchButton = CCSprite::create("win32/g_eliminateButton.png");
	//switchButton->setAnchorPoint(CCPointZero);
	//switchButton->setPosition(ccp(0, 0));
	//switchButtonClipper->addChild(switchButton);
	//// overlay
	//CCSprite* switchOverlay = CCSprite::create("win32/switch_overlay_large.png");
	//switchOverlay->setPosition(switchButtonClipper->getPosition());
	//switchOverlay->setAnchorPoint(CCPointZero);
	//addChild(switchOverlay, 1);

	//// yellow interation button (pieceInteractionDPad)
	// set the stencil
	//m_dpadBar = 20;
	//percentage = m_dpadBar / MAX_BAR_LEVEL;
	//barLevelHeight = BUTTON_TEXTURE_HEIGHT * percentage;
	//dpadStencil = shape(m_dpadBar);
	//dpadStencil->setPosition(ccp(0, barLevelHeight - BUTTON_TEXTURE_HEIGHT));
	//dpadStencil->setAnchorPoint(CCPointZero);
	////// set the clipper
	//dpadButtonClipper = CCClippingNode::create();
	//dpadButtonClipper->setAnchorPoint(CCPointZero);
	//dpadButtonClipper->setPosition(ccp(0, (s.height / 2) - BUTTON_TEXTURE_HEIGHT));
	//dpadButtonClipper->setStencil(dpadStencil);
	//addChild(dpadButtonClipper, 0);
	//// set the content
	//CCSprite* dpadButton = CCSprite::create("win32/y_1_eliminateButton.png");
	//dpadButton->setAnchorPoint(CCPointZero);
	//dpadButton->setPosition(ccp(0, 0));
	//dpadButtonClipper->addChild(dpadButton);
	//// overlay
	//CCSprite* dpadOverlay = CCSprite::create("win32/dpad_overlay_large.png");
	//dpadOverlay->setPosition(dpadButtonClipper->getPosition());
	//dpadOverlay->setAnchorPoint(CCPointZero);
	//addChild(dpadOverlay, 1);

	//// purple interation button (pieceInteractionSlide)
	// set the stencil
	//m_slideBar = 20;
	//percentage = m_slideBar / MAX_BAR_LEVEL;
	//barLevelHeight = BUTTON_TEXTURE_HEIGHT * percentage;
	//slideStencil = shape(m_slideBar);
	//slideStencil->setPosition(ccp(0, barLevelHeight - BUTTON_TEXTURE_HEIGHT));
	//slideStencil->setAnchorPoint(CCPointZero);
	////// set the clipper
	//slideButtonClipper = CCClippingNode::create();
	//slideButtonClipper->setAnchorPoint(CCPointZero);
	//slideButtonClipper->setPosition(ccp(0, (s.height / 2) - (BUTTON_TEXTURE_HEIGHT*2)));
	//slideButtonClipper->setStencil(slideStencil);
	//addChild(slideButtonClipper, 0);
	//// set the content
	//CCSprite* slideButton = CCSprite::create("win32/p_1_eliminateButton.png");
	//slideButton->setAnchorPoint(CCPointZero);
	//slideButton->setPosition(ccp(0, 0));
	//slideButtonClipper->addChild(slideButton);
	//// overlay
	//CCSprite* slideOverlay = CCSprite::create("win32/slide_overlay_large.png");
	//slideOverlay->setPosition(slideButtonClipper->getPosition());
	//slideOverlay->setAnchorPoint(CCPointZero);
	//addChild(slideOverlay, 1);

	//// elimination button
	eliminateButton = CCSprite::create("win32/eliminateButton.png");
	eliminateButton->setAnchorPoint(CCPointZero);
	eliminateButton->setPosition(ccp((s.width / 2) - (BUTTON_TEXTURE_WIDTH / 2), 0));
	addChild(eliminateButton);
}

GameInteractionMenu::~GameInteractionMenu()
{
	CC_SAFE_DELETE(flipButtonClipper);
}

void GameInteractionMenu::interactionComplete()
{
	if (interactionFlipOn)
	{
		interactionFlipOn = false;
		flipButtonClipper->setBarLevel(flipButtonClipper->getBarLevel() - INTERACTION_BAR_COST);
		subPoints(INTERACTION_USE_DEDUCTION);
	}
	else if (interactionSwitchOn)
	{
		interactionSwitchOn = false;
		switchButtonClipper->setBarLevel(switchButtonClipper->getBarLevel() - INTERACTION_BAR_COST);
		subPoints(INTERACTION_USE_DEDUCTION);
	}
	else if (interactionDPadOn)
	{
		interactionDPadOn = false;
		dpadButtonClipper->setBarLevel(dpadButtonClipper->getBarLevel() - INTERACTION_BAR_COST);
		subPoints(INTERACTION_USE_DEDUCTION);
	}
	else if (interactionSlideOn)
	{
		interactionSlideOn = false;
		slideButtonClipper->setBarLevel(slideButtonClipper->getBarLevel() - INTERACTION_BAR_COST);
		subPoints(INTERACTION_USE_DEDUCTION);
	}
	else if (interactionRotaryOn)
	{
	}
	else if (interactionEliminationOn)
	{
		interactionEliminationOn = false;
	}
}

void GameInteractionMenu::subPoints(int points)
{
	CCUserDefault::sharedUserDefault()->setIntegerForKey("current_score", CCUserDefault::sharedUserDefault()->getIntegerForKey("current_score") - points);
	CCUserDefault::sharedUserDefault()->setIntegerForKey("current_sub_score", CCUserDefault::sharedUserDefault()->getIntegerForKey("current_sub_score") + points);
}

void GameInteractionMenu::cancelInteraction()
{
	interactionFlipOn = false;
	interactionSwitchOn = false;
	interactionDPadOn = false;
	interactionSlideOn = false;
	interactionRotaryOn = false;
	interactionEliminationOn = false;
}

void GameInteractionMenu::onEnter()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
	CCLayer::onEnter();
}

void GameInteractionMenu::onExit()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->removeDelegate(this);
	CCLayer::onExit();
}

bool GameInteractionMenu::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
	CCPoint location = touch->getLocationInView();

	location = CCDirector::sharedDirector()->convertToGL(location);

	// check the flip button
	CCRect *ButtonRect = new CCRect(flipButtonClipper->getPositionX()
		, flipButtonClipper->getPositionY()
		, BUTTON_TEXTURE_WIDTH
		, BUTTON_TEXTURE_HEIGHT);

	GameScene* m_parent = (GameScene*)this->getParent();
	if (ButtonRect->containsPoint(location))
	{
		if (flipButtonClipper->getBarLevel() >= INTERACTION_BAR_COST)
		{
			CCLog("InteractionMenu TouchBegan - true");
			interactionFlipOn = true;
			m_parent->interactionSelected(ccBLUE, pieceInteractionFlip);
			return true;
		}
	}

	// check the switch button
	ButtonRect = new CCRect(switchButtonClipper->getPositionX()
		, switchButtonClipper->getPositionY()
		, BUTTON_TEXTURE_WIDTH
		, BUTTON_TEXTURE_HEIGHT);

	if (ButtonRect->containsPoint(location))
	{
		if (switchButtonClipper->getBarLevel() >= INTERACTION_BAR_COST)
		{
			CCLog("InteractionMenu TouchBegan - true");
			interactionSwitchOn = true;
			m_parent->interactionSelected(ccGREEN, pieceInteractionSwitch);
			return true;
		}
	}

	// check the dpad button
	ButtonRect = new CCRect(dpadButtonClipper->getPositionX()
		, dpadButtonClipper->getPositionY()
		, BUTTON_TEXTURE_WIDTH
		, BUTTON_TEXTURE_HEIGHT);

	if (ButtonRect->containsPoint(location))
	{
		if (dpadButtonClipper->getBarLevel() >= INTERACTION_BAR_COST)
		{
			CCLog("InteractionMenu TouchBegan - true");
			interactionDPadOn = true;
			m_parent->interactionSelected(ccYELLOW, pieceInteractionDPadFlip);
			return true;
		}
	}

	// check the slide button
	ButtonRect = new CCRect(slideButtonClipper->getPositionX()
		, slideButtonClipper->getPositionY()
		, BUTTON_TEXTURE_WIDTH
		, BUTTON_TEXTURE_HEIGHT);

	if (ButtonRect->containsPoint(location))
	{
		if (slideButtonClipper->getBarLevel() >= INTERACTION_BAR_COST)
		{
			CCLog("InteractionMenu TouchBegan - true");
			interactionSlideOn = true;
			m_parent->interactionSelected(ccc3(255, 0, 255), pieceInteractionSlide);
			return true;
		}
	}

	// check the elimination button
	ButtonRect = new CCRect(eliminateButton->getPositionX()
		, eliminateButton->getPositionY()
		, BUTTON_TEXTURE_WIDTH
		, BUTTON_TEXTURE_HEIGHT);

	if (ButtonRect->containsPoint(location))
	{
		CCLog("InteractionMenu TouchBegan - true");
		interactionEliminationOn = true;
		m_parent->interactionSelected(ccRED, pieceInteractionElimination);
		return true;
	}

	CCLog("InteractionMenu TouchBegan - false");

	return false;
}

void GameInteractionMenu::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
	CCLog("InteractionMenu TouchEnded");
}

void GameInteractionMenu::menuEliminateCallback(CCObject* pSender)
{
	GameScene* m_parent = (GameScene*)this->getParent();
	m_parent->interactionSelected(ccRED, pieceInteractionElimination);
}

void GameInteractionMenu::addToBarLevel(int pieceColor, int valueToAdd)
{
	if (pieceColor == pieceColorYellow)
	{
		addToDPadBar(valueToAdd);
	}
	else if (pieceColor == pieceColorPurple)
	{
		addToFlipBar(valueToAdd);
	}
	else if (pieceColor == pieceColorGreen)
	{
		addToSwitchBar(valueToAdd);
	}
	else if (pieceColor == pieceColorRed)
	{
		addToSlideBar(valueToAdd);
	}
}

void GameInteractionMenu::addToFlipBar(int addition)
{
	int flipBar = flipButtonClipper->getBarLevel() + addition;
	if (flipBar > MAX_BAR_LEVEL)
	{
		flipBar = MAX_BAR_LEVEL;
	}
	flipButtonClipper->setBarLevel(flipBar);
}

void GameInteractionMenu::addToSwitchBar(int addition)
{
	int switchBar = switchButtonClipper->getBarLevel() + addition;
	if (switchBar > MAX_BAR_LEVEL)
	{
		switchBar = MAX_BAR_LEVEL;
	}
	switchButtonClipper->setBarLevel(switchBar);
}

void GameInteractionMenu::addToDPadBar(int addition)
{
	int dpadBar = dpadButtonClipper->getBarLevel() + addition;
	if (dpadBar > MAX_BAR_LEVEL)
	{
		dpadBar = MAX_BAR_LEVEL;
	}
	dpadButtonClipper->setBarLevel(dpadBar);
}

void GameInteractionMenu::addToSlideBar(int addition)
{
	int slideBar = slideButtonClipper->getBarLevel() + addition;
	if (slideBar > MAX_BAR_LEVEL)
	{
		slideBar = MAX_BAR_LEVEL;
	}
	slideButtonClipper->setBarLevel(slideBar);
}