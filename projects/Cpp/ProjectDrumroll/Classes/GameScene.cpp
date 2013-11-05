//
//  GameScene.cpp
//  ProjectDrumroll
//
//  Created by Alexander Kinlin on 8/15/13.
//
//

#include "GameScene.h"
#include "TitleScene.h"
#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "ScreenHelper.h"
#include "GamePiece.h"

using namespace CocosDenshion;

//
// testing variables not meant to stay in
//
bool interactionFlipOn = false;
bool interactionSwitchOn = false;
bool interactionDPadOn = false;


const int LINE_SPACE = 50;
const int ITEM_COUNT = 4;
const std::string menuItem[ITEM_COUNT] =
{
    "Game Over",
    "Next Level",
    "Toogle Touch State"
};

enum SceneTags {
    kTagBackgroundReference = 1,
    kTagGridReference = 2,
    kTagHUDReference = 3,
    kTagButtonMenu = 4,
    kTagGoalstab
};

enum ButtonMenuTags {
    kTagInteractButtonReference = 1,
	kTagG_InteractButtonReference = 2,
	kTagY_InteractButtonReference = 3,
    kTagEliminationButtonReference
};

CCScene* GameScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    GameScene *layer = GameScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
	if (!CCLayerColor::initWithColor(ccc4(255, 0, 0, 255)))
    {
        return false;
    }
    
    // init level
    m_currentLevel = 1;
    
    // init score
    m_currentScore = 0;
    m_scoreCache = 0;
    
    // init combo
    m_highestCombo = 0;
    // init interaction count
    m_interactionCount = 0;
    
    // add background for the main screen"
    m_backgroundReference = CCSprite::create("win32/MainScreen.png");
    m_backgroundReference->setScale(VisibleRect::getScale());
    // position the sprite on the center of the screen
    m_backgroundReference->setPosition( ccp(VisibleRect::getVisibleRect().size.width/2, VisibleRect::getVisibleRect().size.height/2) );
	m_backgroundReference->setScale(VisibleRect::getScale());
	// add the sprite as a child to this layer
    addChild(m_backgroundReference, kTagBackgroundReference);
        
    // create and add the grid
    createGrid();
    addChild(m_gridReference, kTagGridReference);
    
    // create the HUD
    // create and add the grid
    m_hudReference = new HUD();
    m_hudReference->setPosition(CCPointZero);
    m_hudReference->updateLevel(m_currentLevel);
    this->addChild(m_hudReference, kTagHUDReference);

    // create and add the interact and eliminate buttons
    // should probably be in a HUD class
	// blue interation button (pieceInteractionFlip)
    m_touchStateMenu = CCMenu::create();
    CCMenuItemImage* interactButton = CCMenuItemImage::create("win32/interactButton.png", "win32/interactButton.png", this, menu_selector(GameScene::menuInteractionCallback));
    interactButton->setAnchorPoint(CCPointZero);
	interactButton->setPosition(ccp(0, (VisibleRect::getScreenHeight() / 2) + interactButton->getContentSize().height));
    m_touchStateMenu->addChild(interactButton, kTagInteractButtonReference);
	m_flipBar = 50;

	// green interation button (pieceInteractionDPadFlip)
	CCMenuItemImage* g_interactButton = CCMenuItemImage::create("win32/g_eliminateButton.png", "win32/g_eliminateButton.png", this, menu_selector(GameScene::menuInteractionCallback));
	g_interactButton->setAnchorPoint(CCPointZero);
	g_interactButton->setPosition(ccp(0, VisibleRect::getScreenHeight() / 2));
	m_touchStateMenu->addChild(g_interactButton, kTagG_InteractButtonReference);
	m_dpadBar = 50;

	// yellow interation button (pieceInteractionSwitch)
	CCMenuItemImage* y_interactButton = CCMenuItemImage::create("win32/y_eliminateButton.png", "win32/y_eliminateButton.png", this, menu_selector(GameScene::menuInteractionCallback));
	y_interactButton->setAnchorPoint(CCPointZero);
	y_interactButton->setPosition(ccp(0, (VisibleRect::getScreenHeight() / 2) - interactButton->getContentSize().height));
	m_touchStateMenu->addChild(y_interactButton, kTagY_InteractButtonReference);
	m_switchBar = 50;
    
	// elimination button
    CCMenuItemImage* eliminateButton = CCMenuItemImage::create("win32/eliminateButton.png", "win32/eliminateButton.png", this, menu_selector(GameScene::menuEliminateCallback));
    //eliminateButton->setAnchorPoint(CCPointZero);
    eliminateButton->setPosition(ccp(VisibleRect::getScreenWidth() / 2, eliminateButton->getContentSize().height / 2));
    m_touchStateMenu->addChild(eliminateButton, kTagEliminationButtonReference);
    
	// clean up the menu position and such
    m_touchStateMenu->setContentSize(CCSizeMake(CCDirector::sharedDirector()->getWinSize().width, VisibleRect::getScreenHeight()));
    m_touchStateMenu->setAnchorPoint(CCPointZero);
    m_touchStateMenu->setPosition(CCPointZero);
    addChild(m_touchStateMenu, kTagButtonMenu);
    
    // Add the goals tab
    m_goalsTab = new Goals();
    //addChild(m_goalsTab, kTagGoalstab);
    
    // schedule the update check
    schedule( schedule_selector(GameScene::updateAll), 0.2f);
    
    return true;
}

void GameScene::menuCallback(CCObject* pSender)
{
	// stop all layer actions first
	stopAllActions();

    // get the userdata, it's the index of the menu item clicked
    CCMenuItem* pMenuItem = (CCMenuItemImage *)(pSender);
    int nIdx = pMenuItem->getZOrder() - 10000;
    
    CCLog("Index = %d", nIdx);
    
    // create the scene and run it
    CCScene *pScene = NULL;
    switch (nIdx)
    {
        case 0:
            pScene = TitleScene::scene();
            break;
        case 1:
            // go to the next level
            nextLevel();
            break;
        case 2:
            // change the touch state
            m_gridReference->toggleTouchType();
            // tint the background for the interaction type
            if (m_gridReference->getTouchState() == 0)
            {
                // hard coded to be the interaction state so set to blue
                //m_backgroundReference->setColor(ccBLUE);
				setColor(ccBLUE);
            }
            else if(m_gridReference->getTouchState() == 1)
            {
                // hard coded to be the elimination state so set to red
                //m_backgroundReference->setColor(ccRED);
				setColor(ccRED);
            }
            break;
        default:
            break;
    }

	runAction(
		CCRepeatForever::create(
		CCSequence::create(
		CCFadeTo::create(.25, 0),
		CCFadeTo::create(.25, 255),
		NULL)));
    
    // run
    if (pScene)
    {
        CCDirector::sharedDirector()->replaceScene(pScene);
    }
    
}

void GameScene::menuInteractionCallback(CCObject* pSender)
{
	// stop all layer actions first
	stopAllActions();

    // change the touch state
    m_gridReference->setInteractTouchType();

	// get the selected button
	CCMenuItemImage* pMenuItem = (CCMenuItemImage *)(pSender);

	// set the background color
	switch (pMenuItem->getZOrder())
	{
	case kTagInteractButtonReference:
		if (m_flipBar > 0)
		{
			interactionFlipOn = true;
			setColor(ccBLUE);
			m_gridReference->setInteractionState(is_flip);
		}
		break;
	case kTagG_InteractButtonReference:
		if (m_switchBar > 0)
		{
			interactionSwitchOn = true;
			setColor(ccGREEN);
			m_gridReference->setInteractionState(is_switch);
		}
		
		break;
	case kTagY_InteractButtonReference:
		if (m_dpadBar > 0)
		{
			interactionDPadOn = true;
			setColor(ccYELLOW);
			m_gridReference->setInteractionState(is_dpadflip);
		}
		break;
	default:
		setColor(ccBLACK);
		break;
	}

	runAction(
		CCRepeatForever::create(
		CCSequence::create(
		CCFadeTo::create(.25, 0),
		CCFadeTo::create(.25, 255),
		NULL)));
}

void GameScene::menuEliminateCallback(CCObject* pSender)
{
	// stop all layer actions first
	stopAllActions();

    // change the touch state
    m_gridReference->setEliminateTouchType();
    // hard coded to be the elimination state so set to red
    //m_backgroundReference->setColor(ccRED);
	setColor(ccRED);

	runAction(
		CCRepeatForever::create(
		CCSequence::create(
		CCFadeTo::create(.25, 0),
		CCFadeTo::create(.25, 255),
		NULL)));
}

void GameScene::addToFlipBar(int addition)
{
	m_flipBar += addition;
}

void GameScene::addToSwitchBar(int addition)
{
	m_switchBar += addition;
}

void GameScene::addToDPadBar(int addition)
{
	m_dpadBar += addition;
}

void GameScene::checkForEndOfLevel()
{
    if (m_gridReference->isLevelComplete())
    {
        // check to see if all the goals are complete
        if (m_goalsTab->areGoalsComplete())
        {
            // go to the next level
            nextLevel();
        }
        else
        {
            // end the game (go back to main menu)
            CCScene *pScene = TitleScene::scene();
            CCDirector::sharedDirector()->replaceScene(pScene);
        }
    }
}

void GameScene::refreshScore()
{
    // get the score from the grid
    int levelScore = m_gridReference->getCurrentScore();
    // add the scoreCache variable, value stored from the previous levels
    m_currentScore = m_scoreCache + levelScore;
    
    // give it to the HUD
    m_hudReference->updateScore(m_currentScore);
}

void GameScene::updateGoals()
{
    // check each of the goals and update state
    // hard coded checks
	// TAKEING OUT GOAL CHECKS TO TEST NEW GAMEPLAY
    //if (m_goalsTab->getGoalStatus(0))
    //{
    //    //CCLog("Interaction Count: %d", m_gridReference->getInteractionCount());
    //    // icon is green
    //    if (m_gridReference->getInteractionCount() > 40)
    //    {
    //        // strong coupling with the knowledge that this is the '0' index should change
    //        m_goalsTab->setGoalStatus(false, 0);
    //    }
    //}
    //else
    //{
    //    // icon is red
    //    if (m_gridReference->getInteractionCount() <= 40)
    //    {
    //        // strong coupling with the knowledge that this is the '0' index should change
    //        m_goalsTab->setGoalStatus(true, 0);
    //    }
    //}
    //
    //if (m_goalsTab->getGoalStatus(1))
    //{
    //    // icon is green
    //    if (m_gridReference->getCurrentScore() < 1000)
    //    {
    //        // strong coupling with the knowledge that this is the '0' index should change
    //        m_goalsTab->setGoalStatus(false, 1);
    //    }
    //}
    //else
    //{
    //    // icon is red
    //    if (m_gridReference->getCurrentScore() >= 1000)
    //    {
    //        // strong coupling with the knowledge that this is the '0' index should change
    //        m_goalsTab->setGoalStatus(true, 1);
    //    }
    //}
    //
    //if (m_goalsTab->getGoalStatus(2))
    //{
    //    // icon is green
    //    if (m_gridReference->getComboCount() < 5)
    //    {
    //        // strong coupling with the knowledge that this is the '0' index should change
    //        m_goalsTab->setGoalStatus(false, 2);
    //    }
    //}
    //else
    //{
    //    // icon is red
    //    if (m_gridReference->getComboCount() > 5)
    //    {
    //        // strong coupling with the knowledge that this is the '0' index should change
    //        m_goalsTab->setGoalStatus(true, 2);
    //    }
    //}

	// only goal is score 1000 pts per level
	if (m_currentScore >= m_currentLevel * 1000)
	{
		m_goalsTab->setGoalStatus(true, 0);
	}
	else
	{
		// score is not high enough to pass this level
		m_goalsTab->setGoalStatus(false, 0);
	}
	// always set the other two goals to true
	m_goalsTab->setGoalStatus(true, 1);
	m_goalsTab->setGoalStatus(true, 2);
}

void GameScene::updateAll(float dx)
{
    // update goals
    updateGoals();
    
    // update score
    refreshScore();

	// check if interactions are done
	if (m_gridReference->getInteractionState() == InteractionState::is_empty && m_gridReference->getTouchState() != TouchState::eliminate)
	{
		if (interactionFlipOn)
		{
			interactionFlipOn = false;
			m_flipBar -= 5;
		}
		if (interactionSwitchOn)
		{
			interactionSwitchOn = false;
			m_switchBar -= 5;
		}
		if (interactionDPadOn)
		{
			interactionDPadOn = false;
			m_dpadBar -= 5;
		}
		stopAllActions();
		setColor(ccBLACK);
	}

	m_hudReference->updateBars(m_flipBar, m_switchBar, m_dpadBar);
    
    // check for the end of the level (grid returns complete when all pieces are gone)
    checkForEndOfLevel();
}

void GameScene::createGrid()
{
    // create and add the grid
    m_gridReference = new Grid();
    m_gridReference->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width/2 - m_gridReference->getWidth()/2, CCDirector::sharedDirector()->getWinSize().height/2 - m_gridReference->getHeight()/2));
    
	setColor(ccBLACK);
}

void GameScene::nextLevel()
{
    // remove the grid and create a new one
    createGrid();
    removeChildByTag(kTagGridReference);
    addChild(m_gridReference, kTagGridReference);
    
    // increment the level
    m_currentLevel++;
    m_hudReference->updateLevel(m_currentLevel);
    
    // store the score cache since the score will get reset in the next level
    storeScoreCache();
}

void GameScene::storeScoreCache()
{
    m_scoreCache = m_currentScore;
}
