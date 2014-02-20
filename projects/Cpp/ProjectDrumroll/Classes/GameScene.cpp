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
	m_interactionNative = false;
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

bool TouchSelectorStateMachine::isNativeTouchInteraction()
{
	return m_interactionNative;
}

void TouchSelectorStateMachine::setNativeTouchInteraction(bool isNative)
{
	m_interactionNative = isNative;
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
    m_currentLevel = 1;
    
    // init score
    m_currentScore = 0;
    m_scoreCache = 0;
    
    // init combo
    m_highestCombo = 0;
    // init interaction count
    m_interactionCount = 0;

	// init the touchselector state machine
	TouchSelectorStateMachine::sharedTouchSelector()->init();

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
	m_interactionReference = new InteractionMenu();
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

//void GameScene::menuCallback(CCObject* pSender)
//{
//	// stop all layer actions first
//	stopAllActions();
//
//    // get the userdata, it's the index of the menu item clicked
//    CCMenuItem* pMenuItem = (CCMenuItemImage *)(pSender);
//    int nIdx = pMenuItem->getZOrder() - 10000;
//    
//    CCLog("Index = %d", nIdx);
//    
//    // create the scene and run it
//    CCScene *pScene = NULL;
//    switch (nIdx)
//    {
//        case 0:
//            pScene = TitleScene::scene();
//            break;
//        case 1:
//            // go to the next level
//            nextLevel();
//            break;
//        case 2:
//            // change the touch state
//            m_gridReference->toggleTouchType();
//            // tint the background for the interaction type
//            if (m_gridReference->getTouchState() == 0)
//            {
//                // hard coded to be the interaction state so set to blue
//                //m_backgroundReference->setColor(ccBLUE);
//				setColor(ccBLUE);
//            }
//            else if(m_gridReference->getTouchState() == 1)
//            {
//                // hard coded to be the elimination state so set to red
//                //m_backgroundReference->setColor(ccRED);
//				setColor(ccRED);
//            }
//            break;
//        default:
//            break;
//    }
//
//	runAction(
//		CCRepeatForever::create(
//		CCSequence::create(
//		CCFadeTo::create(.25, 0),
//		CCFadeTo::create(.25, 255),
//		NULL)));
//    
//    // run
//    if (pScene)
//    {
//        CCDirector::sharedDirector()->replaceScene(pScene);
//    }
//    
//}

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
			// check for new high score
			const char* KEY_HIGH_SCORE = "high_score";
			int currentHighScore = CCUserDefault::sharedUserDefault()->getIntegerForKey(KEY_HIGH_SCORE);
			if (m_currentScore > currentHighScore)
			{
				CCUserDefault::sharedUserDefault()->setIntegerForKey(KEY_HIGH_SCORE, m_currentScore);
				CCLog("NEW HIGH SCORE");
			}
			else
			{
				CCLog("**************");
			}

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
	// update score
	refreshScore();

    // update goals
    updateGoals();

	// check if interactions are done
	// needs a callback from the interaction menu to execute
	TouchSelectorStateMachine* sharedTouchSelector = TouchSelectorStateMachine::sharedTouchSelector();
	if (sharedTouchSelector->getInteractionState() == pieceInteractionEmpty)
	{
		// check to make sure the action didn't get reset
		if (!m_gridReference->wasActionReset())
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

	//m_hudReference->updateBars(m_flipBar, m_switchBar, m_dpadBar, m_slideBar, m_rotaryBar);
    
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
