//
//  GameStateMachine.h
//  ProjectDrumroll
//
//  Created by Alexander Kinlin on 11/19/13.
//
//

#ifndef __ProjectDrumroll__GameStateMachine__
#define __ProjectDrumroll__GameStateMachine__

#include "cocos2d.h"
USING_NS_CC;

enum GameStates
{
	gameState_null,
	gameState_initializing,
	gameState_running,
	gameState_paused
};

class GameStateMachine
{
public:
	GameStateMachine();
	~GameStateMachine();

	void init();

	GameStates getGameState();
	void setGameState(GameStates gameState);

private:
	GameStates m_state;
};

#endif /* defined(__ProjectDrumroll__GameStateMachine__) */
