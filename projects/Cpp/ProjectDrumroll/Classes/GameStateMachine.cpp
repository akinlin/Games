//
//  GameStateMachine.cpp
//  ProjectDrumroll
//
//  Created by Alexander Kinlin on 11/19/13.
//
//

#include "GameStateMachine.h"
#include "TitleScene.h"

GameStateMachine::GameStateMachine()
{

}

GameStateMachine::~GameStateMachine()
{

}

void GameStateMachine::init()
{
	// set the state to null to signify it has never been set
	m_state = gameState_null;
}

GameStates GameStateMachine::getGameState()
{
	return m_state;
}

void GameStateMachine::setGameState(GameStates gameState)
{
	m_state = gameState;
}
