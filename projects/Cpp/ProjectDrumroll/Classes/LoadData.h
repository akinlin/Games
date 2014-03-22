//
//  LoadData.h
//  ProjectDrumroll
//
//  Created by Alexander Kinlin on 8/15/13.
//
//

#ifndef __ProjectDrumroll__LoadData__
#define __ProjectDrumroll__LoadData__

// When you import this file, you import all the cocos2d classes
#include "cocos2d.h"
USING_NS_CC;
using namespace std;

class LoadData : public CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static CCScene* scene();
    
    // touch functions
	virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
    
    // implement the "static node()" method manually
	CREATE_FUNC(LoadData);
    
private:
	// loads game data
	void loadData();
    
    // file directory load
	string loadAppDirectory();
    
    // high score load
	string loadHighscores();
    
    // level load
	string loadLevel();
};
#endif /* defined(__ProjectDrumroll__LoadData__) */
