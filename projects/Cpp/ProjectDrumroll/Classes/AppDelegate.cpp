//
//  ProjectDrumrollAppDelegate.cpp
//  ProjectDrumroll
//
//  Created by Alexander Kinlin on 8/13/13.
//  Copyright __MyCompanyName__ 2013. All rights reserved.
//

#include "AppDelegate.h"

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "TitleScene.h"

USING_NS_CC;
using namespace CocosDenshion;

AppDelegate::AppDelegate()
{
	// create the game class
	m_game = new ProjectDrumrollGame();
}

AppDelegate::~AppDelegate()
{
	// delete the game class
	delete m_game;
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // init the game
	if (m_game)
	{
		if (!m_game->init())
		{
			CCLog("AppDelegate::applicationDidFinishLaunching - Error Game failed on init");
			return false;
		}
	}
	else
	{
		CCLog("AppDelegate::applicationDidFinishLaunching - Error Game is NULL");
		return false;
	}
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
	// game interupted pause execution
	if (m_game)
	{
		m_game->pause();
	}
	else
	{
		CCLog("AppDelegate::applicationDidEnterBackground - Error Game is NULL");
	}
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
	// game is told to resume execution
	if (m_game)
	{
		m_game->play();
	}
	else
	{
		CCLog("AppDelegate::applicationWillEnterForeground - Error Game is NULL");
	}
}

int Win32App::getANumber()
{
	return 32;
}