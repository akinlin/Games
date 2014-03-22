//
//  Store.h
//  ProjectDrumroll
//
//  Created by Alexander Kinlin on 8/15/13.
//
//

#ifndef __ProjectDrumroll__Store__
#define __ProjectDrumroll__Store__

// When you import this file, you import all the cocos2d classes
#include "cocos2d.h"
USING_NS_CC;

class Store
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

	void finalScoreUpdate(int score, int level);
    
	/** returns a shared instance of the Store */
	static Store* sharedStore(void);
    
private:
	Store();
	void loadPlist(const char* plistFile);
	static Store* s_SharedStore;
	
};
#endif /* defined(__ProjectDrumroll__Store__) */
