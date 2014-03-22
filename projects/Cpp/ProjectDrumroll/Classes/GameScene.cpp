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

using namespace CocosDenshion;

const char* KEY_CURRENT_LEVEL = "current_level";

const char* KEY_CURRENT_SCORE = "current_score";
const char* KEY_CURRENT_ADD_SCORE = "current_add_score";
const char* KEY_CURRENT_SUB_SCORE = "current_sub_score";

const int LINE_SPACE = 50;
const int ITEM_COUNT = 4;
const std::string menuItem[ITEM_COUNT] =
{
    "Game Over",
    "Next Level",
    "Toogle Touch State"
};

enum SceneTags {
    kTagBackgroundReference,
    kTagHUDReference,
    kTagButtonMenu,
	kTagClippingButton,
    kTagGoalstab,
	kTagGridReference
};

/////////////////////////
// TouchStateMachine

// singleton stuff
static TouchSelectorStateMachine *s_SharedTouchSelector = NULL;

TouchSelectorStateMachine* TouchSelectorStateMachine::sharedTouchSelector(void)
{
	if (!s_SharedTouchSelector)
	{
		s_SharedTouchSelector = new TouchSelectorStateMachine();
		s_SharedTouchSelector->init();
	}

	return s_SharedTouchSelector;
}

TouchSelectorStateMachine::TouchSelectorStateMachine()
{

}

TouchSelectorStateMachine::~TouchSelectorStateMachine()
{

}

void TouchSelectorStateMachine::init()
{
	// set the state to null to signify it has never been set
	m_touchState = interact;
	m_interactionState = pieceInteractionEmpty;

	m_switchGamePieceFirstSelection = false;
	m_actionReset = false;
}

gamePieceInteractionType TouchSelectorStateMachine::getInteractionState()
{
	return m_interactionState;
}
void TouchSelectorStateMachine::setInteractionState(gamePieceInteractionType interactionState)
{
	m_interactionState = interactionState;
}

TouchState TouchSelectorStateMachine::getTouchState()
{
	return m_touchState;
}
void TouchSelectorStateMachine::setTouchState(TouchState touchState)
{
	m_touchState = touchState;
}

bool TouchSelectorStateMachine::activeMultiTouchInteraction()
{
	return m_switchGamePieceFirstSelection;
}

void TouchSelectorStateMachine::setActiveMultiTouchInteraction(bool isActive)
{
	m_switchGamePieceFirstSelection = isActive;
}

bool TouchSelectorStateMachine::wasActionReset()
{
	return m_actionReset;
}

void TouchSelectorStateMachine::setActionReset(bool isReset)
{
	m_actionReset = isReset;
}


/////////////////////////
// GameScene

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
	CCUserDefault::sharedUserDefault()->setIntegerForKey(KEY_CURRENT_LEVEL, 1);
    
    // init score
	CCUserDefault::sharedUserDefault()->setIntegerForKey(KEY_CURRENT_SCORE, 0);
	CCUserDefault::sharedUserDefault()->setIntegerForKey(KEY_CURRENT_ADD_SCORE, 0);
	CCUserDefault::sharedUserDefault()->setIntegerForKey(KEY_CURRENT_SUB_SCORE, 0);
    
    
    // save the score to the high score list
    // need to create a general save class and send the score
    Store::sharedStore()->finalScoreUpdate(
                                           CCUserDefault::sharedUserDefault()->getIntegerForKey(KEY_CURRENT_SCORE),
                                           CCUserDefault::sharedUserDefault()->getIntegerForKey(KEY_CURRENT_LEVEL));

    
    // init combo
    m_highestCombo = 0;
    // init interaction count
    m_interactionCount = 0;

    // add background for the main screen"
    m_backgroundReference = CCSprite::create("MainScreen.png");
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
	m_hudReference->updateLevel(CCUserDefault::sharedUserDefault()->getIntegerForKey(KEY_CURRENT_LEVEL));
    this->addChild(m_hudReference, kTagHUDReference);

    // create and add the interact and eliminate buttons
	m_interactionReference = new GameInteractionMenu();
	addChild(m_interactionReference, kTagClippingButton);
    
    // Add the goals tab
    m_goalsTab = new Goals();
    //addChild(m_goalsTab, kTagGoalstab);
    
	// background music
	//CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("bg_music.wav", true);

    // schedule the update check
    schedule( schedule_selector(GameScene::updateAll), 0.2f);
    
    return true;
}

void GameScene::interactionSelected(ccColor3B color, gamePieceInteractionType interactionState)
{
	// stop all layer actions first
	stopAllActions();

	// need to call back to gamescene reference
	setColor(color);

	TouchSelectorStateMachine* sharedTouchSelector = TouchSelectorStateMachine::sharedTouchSelector();
	if (interactionState == pieceInteractionElimination)
	{
		sharedTouchSelector->setTouchState(eliminate);
		sharedTouchSelector->setInteractionState(pieceInteractionElimination);
	}
	else
	{
		sharedTouchSelector->setTouchState(interact);
		sharedTouchSelector->setInteractionState(interactionState);
	}

	runAction(
	CCRepeatForever::create(
	CCSequence::create(
	CCFadeTo::create(.25, 0),
	CCFadeTo::create(.25, 255),
	NULL)));
}

void GameScene::interactionCancelled()
{
	
}

void GameScene::addToBar(int pieceColor, int valueToAdd)
{
	m_interactionReference->addToBarLevel(pieceColor, valueToAdd);
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
			// save the score to the high score list
			// need to create a general save class and send the score
			Store::sharedStore()->finalScoreUpdate(
				CCUserDefault::sharedUserDefault()->getIntegerForKey(KEY_CURRENT_SCORE), 
				CCUserDefault::sharedUserDefault()->getIntegerForKey(KEY_CURRENT_LEVEL));

            // end the game (go back to main menu)
            CCScene *pScene = TitleScene::scene();
            CCDirector::sharedDirector()->replaceScene(pScene);
        }
    }
}

void GameScene::refreshScore()
{
    // give it to the HUD
	m_hudReference->updateScore();
}

void GameScene::updateGoals()
{
	// only goal is score 1000 pts per level
	if (CCUserDefault::sharedUserDefault()->getIntegerForKey(KEY_CURRENT_SCORE) >= CCUserDefault::sharedUserDefault()->getIntegerForKey(KEY_CURRENT_LEVEL) *1000)
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
	// update score
	refreshScore();

    // update goals
    updateGoals();

	// check if interactions are done
	// needs a callback to the interaction menu to execute
	TouchSelectorStateMachine* sharedTouchSelector = TouchSelectorStateMachine::sharedTouchSelector();
	if (sharedTouchSelector->getInteractionState() == pieceInteractionEmpty)
	{
		// check to make sure the action didn't get reset
		if (!sharedTouchSelector->wasActionReset())
		{
			// tell the interactionmenu the interaction is compelte
			m_interactionReference->interactionComplete();
		}
		else
		{
			m_interactionReference->cancelInteraction();
		}
		// hack to stop the elimination checks
		sharedTouchSelector->setTouchState(interact);

		stopAllActions();
		setColor(ccBLACK);
	}
    
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
	// increment the level
	int currentLevel = CCUserDefault::sharedUserDefault()->getIntegerForKey(KEY_CURRENT_LEVEL) + 1;
	CCUserDefault::sharedUserDefault()->setIntegerForKey(KEY_CURRENT_LEVEL, currentLevel);
	m_hudReference->updateLevel(currentLevel);

    // remove the grid and create a new one
    createGrid();
    removeChildByTag(kTagGridReference);
    addChild(m_gridReference, kTagGridReference);
}
