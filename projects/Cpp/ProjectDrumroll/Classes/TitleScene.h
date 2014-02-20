//
//  TitleScene.h
//  ProjectDrumroll
//
//  Created by Alexander Kinlin on 8/15/13.
//
//

#ifndef __ProjectDrumroll__TitleScene__
#define __ProjectDrumroll__TitleScene__

// When you import this file, you import all the cocos2d classes
#include "cocos2d.h"
USING_NS_CC;

class TitleScene : public CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static CCScene* scene();
    
    // a selector callback
    void menuCallback(CCObject* pSender);
	void callback1(CCNode* pTarget);
    
    // implement the "static node()" method manually
    CREATE_FUNC(TitleScene);
    
private:
	// CCMenu object for the title menu
    CCMenu* m_TitleMenu;
	// CCSprite object for the title background image
	CCSprite*  m_TitleBGSprite;
	// CCSprite object for the title background lable
	CCLabelTTF*  m_TitleLabel;
	// CCLayer object for BG sprites
	CCLayer* m_BGLayer;
	// CCLabelTTF object for high score display
	CCLabelTTF* m_highScoreDisplayString;

	// creates and initalizes the m_TitleMenu
	void createTitleMenu();
	// creates and initalizes the m_TitleBGSprite
	void createTitleBG();
	// pushes the high score screen
	void pushHighScoreScene(float ct);
};
#endif /* defined(__ProjectDrumroll__TitleScene__) */
