//
//  SceneSelectorStateMachine.cpp
//  ProjectDrumroll
//
//  Created by Alexander Kinlin on 11/19/13.
//
//

#include "SceneSelectorStateMachine.h"

SceneSelectorStateMachine::SceneSelectorStateMachine()
{

}

SceneSelectorStateMachine::~SceneSelectorStateMachine()
{

}

void SceneSelectorStateMachine::init()
{
	// set the state to null to signify it has never been set
	m_state = sceneState_null;
}

SceneStates SceneSelectorStateMachine::getSceneState()
{
	return m_state;
}

void SceneSelectorStateMachine::setSceneState(SceneStates gameState)
{
	m_state = gameState;
}
