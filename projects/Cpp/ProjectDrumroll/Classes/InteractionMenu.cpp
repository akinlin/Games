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

const int MAX_BAR_LEVEL = 50;
const int INTERACTION_BAR_COST = 5;
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
	CCSize s = VisibleRect::getScreenSize();

	// turn on touch events
	//setTouchEnabled(true);

	// blue interation button (pieceInteractionFlip)
	// set the stencil
	m_flipBar = 20;
	float percentage = m_flipBar / MAX_BAR_LEVEL;
	float barLevelHeight = BUTTON_TEXTURE_HEIGHT * percentage;
	flipStencil = shape(m_flipBar);
	flipStencil->setPosition(ccp(0, barLevelHeight - BUTTON_TEXTURE_HEIGHT));
	flipStencil->setAnchorPoint(CCPointZero);
	//// set the clipper
	flipButtonClipper = CCClippingNode::create();
	flipButtonClipper->setAnchorPoint(ccp(0, 0));
	flipButtonClipper->setPosition(ccp(0, (s.height / 2) + BUTTON_TEXTURE_HEIGHT));
	flipButtonClipper->setStencil(flipStencil);
	addChild(flipButtonClipper, 0);
	// set the content
	CCSprite* interactButton = CCSprite::create("win32/1_eliminateButton.png");
	interactButton->setAnchorPoint(CCPointZero);
	interactButton->setPosition(ccp(0, 0));
	flipButtonClipper->addChild(interactButton);
	// overlay
	CCSprite* flipOverlay = CCSprite::create("win32/flip_overlay_large.png");
	flipOverlay->setPosition(flipButtonClipper->getPosition());
	flipOverlay->setAnchorPoint(CCPointZero);
	addChild(flipOverlay, 1);

	//// green interation button (pieceInteractionSwitch)
	// set the stencil
	m_switchBar = 20;
	percentage = m_switchBar / MAX_BAR_LEVEL;
	barLevelHeight = BUTTON_TEXTURE_HEIGHT * percentage;
	switchStencil = shape(m_switchBar);
	switchStencil->setPosition(ccp(0, barLevelHeight - BUTTON_TEXTURE_HEIGHT));
	switchStencil->setAnchorPoint(CCPointZero);
	//// set the clipper
	switchButtonClipper = CCClippingNode::create();
	switchButtonClipper->setAnchorPoint(CCPointZero);
	switchButtonClipper->setPosition(ccp(0, (s.height / 2)));
	switchButtonClipper->setStencil(switchStencil);
	addChild(switchButtonClipper, 0);
	// set the content
	CCSprite* switchButton = CCSprite::create("win32/g_eliminateButton.png");
	switchButton->setAnchorPoint(CCPointZero);
	switchButton->setPosition(ccp(0, 0));
	switchButtonClipper->addChild(switchButton);
	// overlay
	CCSprite* switchOverlay = CCSprite::create("win32/switch_overlay_large.png");
	switchOverlay->setPosition(switchButtonClipper->getPosition());
	switchOverlay->setAnchorPoint(CCPointZero);
	addChild(switchOverlay, 1);

	//// yellow interation button (pieceInteractionDPad)
	// set the stencil
	m_dpadBar = 20;
	percentage = m_dpadBar / MAX_BAR_LEVEL;
	barLevelHeight = BUTTON_TEXTURE_HEIGHT * percentage;
	dpadStencil = shape(m_dpadBar);
	dpadStencil->setPosition(ccp(0, barLevelHeight - BUTTON_TEXTURE_HEIGHT));
	dpadStencil->setAnchorPoint(CCPointZero);
	//// set the clipper
	dpadButtonClipper = CCClippingNode::create();
	dpadButtonClipper->setAnchorPoint(CCPointZero);
	dpadButtonClipper->setPosition(ccp(0, (s.height / 2) - BUTTON_TEXTURE_HEIGHT));
	dpadButtonClipper->setStencil(dpadStencil);
	addChild(dpadButtonClipper, 0);
	// set the content
	CCSprite* dpadButton = CCSprite::create("win32/y_1_eliminateButton.png");
	dpadButton->setAnchorPoint(CCPointZero);
	dpadButton->setPosition(ccp(0, 0));
	dpadButtonClipper->addChild(dpadButton);
	// overlay
	CCSprite* dpadOverlay = CCSprite::create("win32/dpad_overlay_large.png");
	dpadOverlay->setPosition(dpadButtonClipper->getPosition());
	dpadOverlay->setAnchorPoint(CCPointZero);
	addChild(dpadOverlay, 1);

	//// purple interation button (pieceInteractionSlide)
	// set the stencil
	m_slideBar = 20;
	percentage = m_slideBar / MAX_BAR_LEVEL;
	barLevelHeight = BUTTON_TEXTURE_HEIGHT * percentage;
	slideStencil = shape(m_slideBar);
	slideStencil->setPosition(ccp(0, barLevelHeight - BUTTON_TEXTURE_HEIGHT));
	slideStencil->setAnchorPoint(CCPointZero);
	//// set the clipper
	slideButtonClipper = CCClippingNode::create();
	slideButtonClipper->setAnchorPoint(CCPointZero);
	slideButtonClipper->setPosition(ccp(0, (s.height / 2) - (BUTTON_TEXTURE_HEIGHT*2)));
	slideButtonClipper->setStencil(slideStencil);
	addChild(slideButtonClipper, 0);
	// set the content
	CCSprite* slideButton = CCSprite::create("win32/p_1_eliminateButton.png");
	slideButton->setAnchorPoint(CCPointZero);
	slideButton->setPosition(ccp(0, 0));
	slideButtonClipper->addChild(slideButton);
	// overlay
	CCSprite* slideOverlay = CCSprite::create("win32/slide_overlay_large.png");
	slideOverlay->setPosition(slideButtonClipper->getPosition());
	slideOverlay->setAnchorPoint(CCPointZero);
	addChild(slideOverlay, 1);

	//// elimination button
	eliminateButton = CCSprite::create("win32/eliminateButton.png");
	eliminateButton->setAnchorPoint(CCPointZero);
	eliminateButton->setPosition(ccp((s.width / 2) - (BUTTON_TEXTURE_WIDTH / 2), 0));
	addChild(eliminateButton);

	//m_touchStateMenu = CCMenu::create();
	////m_touchStateMenu->addChild(interactButton, kTagInteractButtonRefer ence);

	//// green interation button (pieceInteractionSwitch)
	//CCMenuItemImage* g_interactButton = CCMenuItemImage::create("win32/g_eliminateButton.png", "win32/g_eliminateButton.png", this, menu_selector(InteractionMenu::menuInteractionCallback));
	//g_interactButton->setAnchorPoint(CCPointZero);
	//g_interactButton->setPosition(ccp(0, VisibleRect::getScreenHeight() / 2));
	//m_touchStateMenu->addChild(g_interactButton, kTagG_InteractButtonReference);
	//m_dpadBar = 20;

	//// yellow interation button (pieceInteractionDPad)
	//CCMenuItemImage* y_interactButton = CCMenuItemImage::create("win32/y_eliminateButton.png", "win32/y_eliminateButton.png", this, menu_selector(InteractionMenu::menuInteractionCallback));
	//y_interactButton->setAnchorPoint(CCPointZero);
	//y_interactButton->setPosition(ccp(0, (VisibleRect::getScreenHeight() / 2) - BUTTON_TEXTURE_HEIGHT));
	//m_touchStateMenu->addChild(y_interactButton, kTagY_InteractButtonReference);
	//m_switchBar = 20;

	//// purple interation button (pieceInteractionSlide)
	//CCMenuItemImage* p_interactButton = CCMenuItemImage::create("win32/p_eliminateButton.png", "win32/p_eliminateButton.png", this, menu_selector(InteractionMenu::menuInteractionCallback));
	//p_interactButton->setAnchorPoint(CCPointZero);
	//p_interactButton->setPosition(ccp(0, (VisibleRect::getScreenHeight() / 2) - (BUTTON_TEXTURE_HEIGHT * 2)));
	//m_touchStateMenu->addChild(p_interactButton, kTagP_InteractButtonReference);
	//m_slideBar = 20;

	//// orange interation button (pieceInteractionRotary)
	//CCMenuItemImage* o_interactButton = CCMenuItemImage::create("win32/o_eliminateButton.png", "win32/o_eliminateButton.png", this, menu_selector(InteractionMenu::menuInteractionCallback));
	//o_interactButton->setAnchorPoint(CCPointZero);
	//o_interactButton->setPosition(ccp(0, (VisibleRect::getScreenHeight() / 2) - (BUTTON_TEXTURE_HEIGHT * 3)));
	////m_touchStateMenu->addChild(o_interactButton, kTagO_InteractButtonReference);
	//m_rotaryBar = 20;

	//// elimination button
	//CCMenuItemImage* eliminateButton = CCMenuItemImage::create("win32/eliminateButton.png", "win32/eliminateButton.png", this, menu_selector(InteractionMenu::menuEliminateCallback));
	////eliminateButton->setAnchorPoint(CCPointZero);
	//eliminateButton->setPosition(ccp(VisibleRect::getScreenWidth() / 2, eliminateButton->getContentSize().height / 2));
	//m_touchStateMenu->addChild(eliminateButton, kTagEliminationButtonReference);

	//// clean up the menu position and such
	//m_touchStateMenu->setContentSize(CCSizeMake(CCDirector::sharedDirector()->getWinSize().width, VisibleRect::getScreenHeight()));
	//m_touchStateMenu->setAnchorPoint(CCPointZero);
	//m_touchStateMenu->setPosition(CCPointZero);
	//addChild(m_touchStateMenu, 1);
}

InteractionMenu::~InteractionMenu()
{
	// may not need a destrcutor
}

void InteractionMenu::interactionComplete()
{
	if (interactionFlipOn)
	{
		interactionFlipOn = false;
		m_flipBar -= INTERACTION_BAR_COST;

		float percentage = m_flipBar / MAX_BAR_LEVEL;
		float barLevelHeight = BUTTON_TEXTURE_HEIGHT * percentage;
		flipStencil->setPosition(ccp(0, barLevelHeight - BUTTON_TEXTURE_HEIGHT));
	}
	else if (interactionSwitchOn)
	{
		interactionSwitchOn = false;
		m_switchBar -= INTERACTION_BAR_COST;

		float percentage = m_switchBar / MAX_BAR_LEVEL;
		float barLevelHeight = BUTTON_TEXTURE_HEIGHT * percentage;
		switchStencil->setPosition(ccp(0, barLevelHeight - BUTTON_TEXTURE_HEIGHT));
	}
	else if (interactionDPadOn)
	{
		interactionDPadOn = false;
		m_dpadBar -= INTERACTION_BAR_COST;

		float percentage = m_dpadBar / MAX_BAR_LEVEL;
		float barLevelHeight = BUTTON_TEXTURE_HEIGHT * percentage;
		dpadStencil->setPosition(ccp(0, barLevelHeight - BUTTON_TEXTURE_HEIGHT));
	}
	else if (interactionSlideOn)
	{
		interactionSlideOn = false;
		m_slideBar -= INTERACTION_BAR_COST;

		float percentage = m_slideBar / MAX_BAR_LEVEL;
		float barLevelHeight = BUTTON_TEXTURE_HEIGHT * percentage;
		slideStencil->setPosition(ccp(0, barLevelHeight - BUTTON_TEXTURE_HEIGHT));
	}
	else if (interactionRotaryOn)
	{
		interactionRotaryOn = false;
		m_rotaryBar -= INTERACTION_BAR_COST;
	}
	else if (interactionEliminationOn)
	{
		interactionEliminationOn = false;
	}
}

void InteractionMenu::cancelInteraction()
{
	interactionFlipOn = false;
	interactionSwitchOn = false;
	interactionDPadOn = false;
	interactionSlideOn = false;
	interactionRotaryOn = false;
	interactionEliminationOn = false;
}

CCDrawNode* InteractionMenu::shape(int barLevel)
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

void InteractionMenu::onEnter()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
	CCLayer::onEnter();
}

void InteractionMenu::onExit()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->removeDelegate(this);
	CCLayer::onExit();
}

bool InteractionMenu::ccTouchBegan(CCTouch* touch, CCEvent* event)
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
		if (m_flipBar >= INTERACTION_BAR_COST)
		{
			CCLog("InteractionMenu TouchBegan - true");
			interactionFlipOn = true;
			m_parent->interactionSelected(ccBLUE, is_flip);
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
		if (m_switchBar >= INTERACTION_BAR_COST)
		{
			CCLog("InteractionMenu TouchBegan - true");
			interactionSwitchOn = true;
			m_parent->interactionSelected(ccGREEN, is_switch);
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
		if (m_dpadBar >= INTERACTION_BAR_COST)
		{
			CCLog("InteractionMenu TouchBegan - true");
			interactionDPadOn = true;
			m_parent->interactionSelected(ccYELLOW, is_dpadflip);
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
		if (m_slideBar >= INTERACTION_BAR_COST)
		{
			CCLog("InteractionMenu TouchBegan - true");
			interactionSlideOn = true;
			m_parent->interactionSelected(ccc3(255, 0, 255), is_slide);
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
		m_parent->interactionSelected(ccRED, is_elimination);
		return true;
	}

	CCLog("InteractionMenu TouchBegan - false");

	return false;
}

//void InteractionMenu::ccTouchEnded(CCTouch* touch, CCEvent* event)
void InteractionMenu::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
	/*CCPoint location = touch->getLocationInView();

	location = CCDirector::sharedDirector()->convertToGL(location);

	GameScene* m_parent = (GameScene*)this->getParent();
	if (interactionFlipOn)
	{
		m_parent->interactionSelected(ccBLUE, is_flip);
	}

	if (interactionSwitchOn)
	{
		m_parent->interactionSelected(ccGREEN, is_switch);
	}

	if (interactionDPadOn)
	{
		m_parent->interactionSelected(ccYELLOW, is_dpadflip);
	}

	if (interactionSlideOn)
	{
		m_parent->interactionSelected(ccc3(255, 0, 255), is_slide);
	}

	if (interactionEliminationOn)
	{
		m_parent->interactionSelected(ccRED, is_elimination);
	}*/

	CCLog("InteractionMenu TouchEnded");
}

//void InteractionMenu::ccTouchEnded(CCSet* touch, CCEvent* event)
//{
//	// check for button selection
//	CCSetIterator it;
//	CCTouch* touch;
//
//	for (it = touches->begin(); it != touches->end(); it++)
//	{
//		touch = (CCTouch*)(*it);
//
//		if (!touch)
//			break;
//
//		CCPoint location = touch->getLocationInView();
//
//		location = CCDirector::sharedDirector()->convertToGL(location);
//
//		// check the flip button
//		CCRect *ButtonRect = new CCRect(flipButtonClipper->getPositionX()
//			, flipButtonClipper->getPositionY()
//			, BUTTON_TEXTURE_WIDTH
//			, BUTTON_TEXTURE_HEIGHT);
//
//		GameScene* m_parent = (GameScene*)this->getParent();
//		if (ButtonRect->containsPoint(location))
//		{
//			if (m_flipBar >= INTERACTION_BAR_COST)
//			{
//				interactionFlipOn = true;
//				// need to call back to gamescene reference
//				m_parent->interactionSelected(ccBLUE, is_flip);
//			}
//		}
//
//		// check the switch button
//		ButtonRect = new CCRect(switchButtonClipper->getPositionX()
//			, switchButtonClipper->getPositionY()
//			, BUTTON_TEXTURE_WIDTH
//			, BUTTON_TEXTURE_HEIGHT);
//
//		if (ButtonRect->containsPoint(location))
//		{
//			if (m_switchBar >= INTERACTION_BAR_COST)
//			{
//				interactionSwitchOn = true;
//				// need to call back to gamescene reference
//				m_parent->interactionSelected(ccGREEN, is_switch);
//			}
//		}
//
//		// check the dpad button
//		ButtonRect = new CCRect(dpadButtonClipper->getPositionX()
//			, dpadButtonClipper->getPositionY()
//			, BUTTON_TEXTURE_WIDTH
//			, BUTTON_TEXTURE_HEIGHT);
//
//		if (ButtonRect->containsPoint(location))
//		{
//			if (m_dpadBar >= INTERACTION_BAR_COST)
//			{
//				interactionDPadOn = true;
//				// need to call back to gamescene reference
//				m_parent->interactionSelected(ccYELLOW, is_dpadflip);
//			}
//		}
//
//		// check the slide button
//		ButtonRect = new CCRect(slideButtonClipper->getPositionX()
//			, slideButtonClipper->getPositionY()
//			, BUTTON_TEXTURE_WIDTH
//			, BUTTON_TEXTURE_HEIGHT);
//
//		if (ButtonRect->containsPoint(location))
//		{
//			if (m_slideBar >= INTERACTION_BAR_COST)
//			{
//				interactionSlideOn = true;
//				// need to call back to gamescene reference
//				m_parent->interactionSelected(ccc3(255, 0, 255), is_slide);
//			}
//		}
//
//		// check the elimination button
//		ButtonRect = new CCRect(eliminateButton->getPositionX()
//			, eliminateButton->getPositionY()
//			, BUTTON_TEXTURE_WIDTH
//			, BUTTON_TEXTURE_HEIGHT);
//
//		if (ButtonRect->containsPoint(location))
//		{
//			GameScene* m_parent = (GameScene*)this->getParent();
//			m_parent->interactionSelected(ccRED, is_elimination);
//		}
//	}
//}

void InteractionMenu::menuInteractionCallback(CCObject* pSender)
{
	// get the selected button
	CCMenuItemImage* pMenuItem = (CCMenuItemImage *)(pSender);

	GameScene* m_parent = (GameScene*)this->getParent();

	// set the background color
	switch (pMenuItem->getZOrder())
	{
	case kTagInteractButtonReference:
		if (m_flipBar >= INTERACTION_BAR_COST)
		{
			interactionFlipOn = true;
			// need to call back to gamescene reference
			m_parent->interactionSelected(ccBLUE, is_flip);
		}
		break;
	case kTagG_InteractButtonReference:
		if (m_switchBar >= INTERACTION_BAR_COST)
		{
			interactionSwitchOn = true;
			// need to call back to gamescene reference
			m_parent->interactionSelected(ccGREEN, is_switch);
		}

		break;
	case kTagY_InteractButtonReference:
		if (m_dpadBar >= INTERACTION_BAR_COST)
		{
			interactionDPadOn = true;
			// need to call back to gamescene reference
			m_parent->interactionSelected(ccYELLOW, is_dpadflip);
		}
		break;
	case kTagP_InteractButtonReference:
		if (m_slideBar >= INTERACTION_BAR_COST)
		{
			interactionSlideOn = true;
			// need to call back to gamescene reference
			m_parent->interactionSelected(ccc3(255, 0, 255), is_slide);
		}
		break;
	case kTagO_InteractButtonReference:
		if (m_rotaryBar >= INTERACTION_BAR_COST)
		{
			interactionRotaryOn = true;
			// need to call back to gamescene reference
			m_parent->interactionSelected(ccc3(255, 165, 0), is_rotary);
		}
		break;
	default:
		// need to call back to gamescene reference
		m_parent->interactionSelected(ccBLACK, is_empty);
		break;
	}
}

void InteractionMenu::menuEliminateCallback(CCObject* pSender)
{
	GameScene* m_parent = (GameScene*)this->getParent();
	m_parent->interactionSelected(ccRED, is_elimination);
}

void InteractionMenu::addToBarLevel(int pieceColor, int valueToAdd)
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

void InteractionMenu::addToFlipBar(int addition)
{
	m_flipBar += addition;
	if (m_flipBar > MAX_BAR_LEVEL)
	{
		m_flipBar = MAX_BAR_LEVEL;
	}

	float percentage = m_flipBar / MAX_BAR_LEVEL;
	float barLevelHeight = BUTTON_TEXTURE_HEIGHT * percentage;
	flipStencil->setPosition(ccp(0, barLevelHeight - BUTTON_TEXTURE_HEIGHT));
}

void InteractionMenu::addToSwitchBar(int addition)
{
	m_switchBar += addition;
	if (m_switchBar > MAX_BAR_LEVEL)
	{
		m_switchBar = MAX_BAR_LEVEL;
	}

	float percentage = m_switchBar / MAX_BAR_LEVEL;
	float barLevelHeight = BUTTON_TEXTURE_HEIGHT * percentage;
	switchStencil->setPosition(ccp(0, barLevelHeight - BUTTON_TEXTURE_HEIGHT));
}

void InteractionMenu::addToDPadBar(int addition)
{
	m_dpadBar += addition;
	if (m_dpadBar > MAX_BAR_LEVEL)
	{
		m_dpadBar = MAX_BAR_LEVEL;
	}

	float percentage = m_dpadBar / MAX_BAR_LEVEL;
	float barLevelHeight = BUTTON_TEXTURE_HEIGHT * percentage;
	dpadStencil->setPosition(ccp(0, barLevelHeight - BUTTON_TEXTURE_HEIGHT));
}

void InteractionMenu::addToSlideBar(int addition)
{
	m_slideBar += addition;
	if (m_slideBar > MAX_BAR_LEVEL)
	{
		m_slideBar = MAX_BAR_LEVEL;
	}

	float percentage = m_slideBar / MAX_BAR_LEVEL;
	float barLevelHeight = BUTTON_TEXTURE_HEIGHT * percentage;
	slideStencil->setPosition(ccp(0, barLevelHeight - BUTTON_TEXTURE_HEIGHT));
}