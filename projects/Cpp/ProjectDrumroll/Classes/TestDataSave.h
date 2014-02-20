//
//  TestDataSave.h
//  ProjectDrumroll
//
//  Created by Alexander Kinlin on 8/15/13.
//
//

#ifndef __ProjectDrumroll__TestDataSave__
#define __ProjectDrumroll__TestDataSave__

// When you import this file, you import all the cocos2d classes
#include "cocos2d.h"
USING_NS_CC;

class TestDataSave : public CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static CCScene* scene();
    
    // implement the "static node()" method manually
	CREATE_FUNC(TestDataSave);
    
private:
	// CCLabelTTF object for high score display
	CCLabelTTF* m_highScoreDisplayString;

	// loads a plist file
	void loadPlist(const char* plistFile);
	void popScene();
};
#endif /* defined(__ProjectDrumroll__TestDataSave__) */
