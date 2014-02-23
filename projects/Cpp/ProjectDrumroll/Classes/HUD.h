//
//  HUD.h
//  ProjectDrumroll
//
//  Created by Alexander Kinlin on 8/24/13.
//
//

#ifndef __ProjectDrumroll__HUD__
#define __ProjectDrumroll__HUD__

#include "cocos2d.h"

USING_NS_CC;

class HUD : public CCLayer
{
public:
    HUD();
    ~HUD();
    
    void returnToMenu(CCObject* pSender);
    
	void updateScore();
    void updateLevel(int level);
    
private:
    CCLabelTTF* m_scoreDisplayString;
	CCLabelTTF* m_scoreSubCountDisplayString;
	CCLabelTTF* m_scoreAddCountDisplayString;
    CCLabelTTF* m_levelDisplayString;
    
};


#endif /* defined(__ProjectDrumroll__HUD__) */
