//
//  ProjectDrumrollGame.h
//  ProjectDrumroll
//
//  Created by Alexander Kinlin on 11/11/13.
//  Copyright TBDEntertainment 2013. All rights reserved.
//

#include "GameStateMachine.h"

#ifndef  _PROJECTDRUMROLLGAME_H_
#define  _PROJECTDRUMROLLGAME_H_

/**
@brief    The ProjectDrumroll Game Class.

This class manages all high level aspects of the game.
*/
class  ProjectDrumrollGame
{
public:
	ProjectDrumrollGame();
	virtual ~ProjectDrumrollGame();

	// called on applicationDidFinishLaunching
	bool init();

	// set the display of the debug text
	void setDebugDisplay(bool isDebugVisible);

	// called on applicationDidEnterBackground
	void pause();
	// called on applicationWillEnterForeground
	void play();

private:
	GameStateMachine* m_gameStateMachine;

	// this funcation abstracts the initialization process 
	// to create cleaner more readable code and support better debugging
	void setUpGame();
};

#endif // _PROJECTDRUMROLLGAME_H_

