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
    HUD();//GameScene* parentScene);
    ~HUD();
    
    virtual void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    void returnToMenu(CCObject* pSender);
    
    void updateScore(int score);
    void updateLevel(int level);
	void updateBars(int flipbar, int switchbar, int dpadbar);
    
private:
    CCLabelTTF* m_scoreDisplayString;
    CCLabelTTF* m_levelDisplayString;

	CCLabelTTF* m_flipDisplayString;
	CCLabelTTF* m_switchDisplayString;
	CCLabelTTF* m_dpadDisplayString;
    
//    GameScene* m_parentScene;
    
};


#endif /* defined(__ProjectDrumroll__HUD__) */
