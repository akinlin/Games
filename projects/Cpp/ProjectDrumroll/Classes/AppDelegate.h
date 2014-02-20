//
//  ProjectDrumrollAppDelegate.h
//  ProjectDrumroll
//
//  Created by Alexander Kinlin on 8/13/13.
//  Copyright __MyCompanyName__ 2013. All rights reserved.
//

#ifndef  _APP_DELEGATE_H_
#define  _APP_DELEGATE_H_

#include "CCApplication.h"
#include "ProjectDrumrollGame.h"

/**
@brief    The cocos2d Application.

The reason for implement as private inheritance is to hide some interface call by CCDirector.
*/
class  AppDelegate : private cocos2d::CCApplication
{
public:
    AppDelegate();
    virtual ~AppDelegate();

    /**
    @brief    Implement CCDirector and CCScene init code here.
    @return true    Initialize success, app continue.
    @return false   Initialize failed, app terminate.
    */
    virtual bool applicationDidFinishLaunching();

    /**
    @brief  The function be called when the application enter background
    @param  the pointer of the application
    */
    virtual void applicationDidEnterBackground();

    /**
    @brief  The function be called when the application enter foreground
    @param  the pointer of the application
    */
    virtual void applicationWillEnterForeground();

private:
	// techdemo hack to allow for easier across app development
	// production code will subcalss AppDelagate but this requires updating main in all platforms
	ProjectDrumrollGame* m_game;
};

class Win32App : private AppDelegate
{
public:
	int getANumber();
};

#endif // _APP_DELEGATE_H_

