//
//  BaseSceneSelector.cpp
//  ProjectDrumroll
//
//  Created by Alexander Kinlin on 11/19/13.
//
//

#include "BaseSceneSelector.h"
#include "TitleScene.h"

BaseSceneSelector::BaseSceneSelector()
{
	m_sceneStateMachine = new SceneSelectorStateMachine();
}

BaseSceneSelector::~BaseSceneSelector()
{
	delete m_sceneStateMachine;
}

bool BaseSceneSelector::init()
{
	// set scene state to title
	m_sceneStateMachine->setSceneState(sceneState_title);

	CCDirector *pDirector = CCDirector::sharedDirector();
	// create a scene. it's an autorelease object
	CCScene *pScene = TitleScene::scene();

	// run
	pDirector->runWithScene(pScene);

	return true;
}
