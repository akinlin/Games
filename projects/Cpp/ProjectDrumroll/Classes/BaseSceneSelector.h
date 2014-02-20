//
//  BaseSceneSelector.h
//  ProjectDrumroll
//
//  Created by Alexander Kinlin on 11/19/13.
//
//

#ifndef __ProjectDrumroll__BaseSceneSelector__
#define __ProjectDrumroll__BaseSceneSelector__

#include "cocos2d.h"
#include "SceneSelectorStateMachine.h"
USING_NS_CC;

class BaseSceneSelector
{
public:
	BaseSceneSelector();
	~BaseSceneSelector();

	bool init();

protected:

private:
	SceneSelectorStateMachine* m_sceneStateMachine;

};

#endif /* defined(__ProjectDrumroll__BaseSceneSelector__) */
