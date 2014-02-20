//
//  SceneSelectorStateMachine.h
//  ProjectDrumroll
//
//  Created by Alexander Kinlin on 11/19/13.
//
//

#ifndef __ProjectDrumroll__SceneSelectorStateMachine__
#define __ProjectDrumroll__SceneSelectorStateMachine__

#include "cocos2d.h"
USING_NS_CC;

enum SceneStates
{
	sceneState_null,
	sceneState_title,
	sceneState_game
};

class SceneSelectorStateMachine
{
public:
	SceneSelectorStateMachine();
	~SceneSelectorStateMachine();

	void init();

	SceneStates getSceneState();
	void setSceneState(SceneStates sceneState);

private:
	SceneStates m_state;
};

#endif /* defined(__ProjectDrumroll__SceneSelectorStateMachine__) */
