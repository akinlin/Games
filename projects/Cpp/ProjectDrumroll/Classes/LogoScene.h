//
//  LogoScene.h
//  ProjectDrumroll
//
//  Created by Alexander Kinlin on 2/11/13.
//
//

#ifndef __ProjectDrumroll__LogoScene__
#define __ProjectDrumroll__LogoScene__

// When you import this file, you import all the cocos2d classes
#include "cocos2d.h"
USING_NS_CC;

class LogoScene : public CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static CCScene* scene();

	void callback1(CCNode* pTarget);
	void callback2(CCNode* pTarget);

	// touch functions
	virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
    
    // implement the "static node()" method manually
	CREATE_FUNC(LogoScene);
    
private:
	// CCSprite object for the title background image
	CCSprite*  m_LogoSprite;

	// creates and initalizes the m_TitleBGSprite
	void createTitleBG();
};
#endif /* defined(__ProjectDrumroll__TitleScene__) */
