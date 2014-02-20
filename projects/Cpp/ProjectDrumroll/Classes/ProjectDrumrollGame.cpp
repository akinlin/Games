//
//  ProjectDrumrollGame.cpp
//  ProjectDrumroll
//
//  Created by Alexander Kinlin on 11/11/13.
//  Copyright TBDEntertainment 2013. All rights reserved.
//

#include "ProjectDrumrollGame.h"

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "LogoScene.h"
#include "TitleScene.h"
#include "TestDataSave.h"

USING_NS_CC;
using namespace CocosDenshion;

ProjectDrumrollGame::ProjectDrumrollGame()
{
	// create the GameStateMachine
	m_gameStateMachine = new GameStateMachine();
}

ProjectDrumrollGame::~ProjectDrumrollGame()
{
	delete m_gameStateMachine;
	//delete m_sceneSelector;
}

bool ProjectDrumrollGame::init()
{
	// Ugly but helpful for debugging
	if (m_gameStateMachine)
	{
		setUpGame();
	}
	else
	{
		CCLog("ProjectDrumrollGame::init - Error running m_gameStateMachine is NULL");
		return false;
	}

	return true;
}

void ProjectDrumrollGame::setUpGame()
{
	// initialize and set the GameStateMachine to initalizing
	m_gameStateMachine->init();
	m_gameStateMachine->setGameState(gameState_initializing);

	// initialize director
	CCDirector *pDirector = CCDirector::sharedDirector();
	pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());

	// set FPS. the default value is 1.0/60 if you don't call this
	pDirector->setAnimationInterval(1.0 / 60);

	// create a scene. it's an autorelease object
	CCScene *pScene = LogoScene::scene();

	// run
	pDirector->runWithScene(pScene);

	// set the game state to running once the initialization process is complete
	m_gameStateMachine->setGameState(gameState_running);
}

void setDebugDisplay(bool isDebugVisible)
{
	// turn on display FPS
	CCDirector *pDirector = CCDirector::sharedDirector();
	pDirector->setDisplayStats(isDebugVisible);
}

// called on applicationDidEnterBackground
void ProjectDrumrollGame::pause()
{
	// pause the scene
	CCDirector::sharedDirector()->stopAnimation();

	// pause the sound
	SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
	SimpleAudioEngine::sharedEngine()->pauseAllEffects();

	// set the game state to paused
	m_gameStateMachine->setGameState(gameState_paused);
}

// called on applicationWillEnterForeground
void ProjectDrumrollGame::play()
{
	// play the scene
	CCDirector::sharedDirector()->startAnimation();

	// play the sound
	SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
	SimpleAudioEngine::sharedEngine()->resumeAllEffects();

	// set the game state to running
	m_gameStateMachine->setGameState(gameState_running);
}
