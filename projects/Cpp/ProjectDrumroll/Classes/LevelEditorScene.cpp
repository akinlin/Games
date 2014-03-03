//
//  LevelEditorScene.cpp
//  ProjectDrumroll
//
//  Created by Alexander Kinlin on 8/15/13.
//
//

#include "LevelEditorScene.h"
#include "TitleScene.h"
#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "ScreenHelper.h"
#include "Store.h"

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
// LevelEditorScene

CCScene* LevelEditorScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    LevelEditorScene *layer = LevelEditorScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool LevelEditorScene::init()
{
    //////////////////////////////
    // 1. super init first
	if (!CCLayerColor::initWithColor(ccc4(255, 0, 0, 255)))
    {
        return false;
    }
   
    // add background for the main screen"
    m_backgroundReference = CCSprite::create("win32/MainScreenEditor.png");
    m_backgroundReference->setScale(VisibleRect::getScale());
    // position the sprite on the center of the screen
    m_backgroundReference->setPosition( ccp(VisibleRect::getVisibleRect().size.width/2, VisibleRect::getVisibleRect().size.height/2) );
	m_backgroundReference->setScale(VisibleRect::getScale());
	// add the sprite as a child to this layer
    addChild(m_backgroundReference, kTagBackgroundReference);
        
    // create and add the grid
    createGrid();
    addChild(m_gridReference, kTagGridReference);
    
    // create and add the interact and eliminate buttons
	m_interactionReference = new GameInteractionMenu();
	addChild(m_interactionReference, kTagClippingButton);
    
    // schedule the update check
    schedule( schedule_selector(LevelEditorScene::updateAll), 0.2f);
    
    return true;
}

void LevelEditorScene::interactionSelected(ccColor3B color, gamePieceInteractionType interactionState)
{
	// stop all layer actions first
	stopAllActions();

	// need to call back to LevelEditorScene reference
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

void LevelEditorScene::updateAll(float dx)
{
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
}

void LevelEditorScene::createGrid()
{
    // create and add the grid
    m_gridReference = new Grid();
    m_gridReference->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width/2 - m_gridReference->getWidth()/2, CCDirector::sharedDirector()->getWinSize().height/2 - m_gridReference->getHeight()/2));
    
	setColor(ccBLACK);
}
