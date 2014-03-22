//
//  LogoScene.cpp
//  ProjectDrumroll
//
//  Created by Alexander Kinlin on 2/11/13.
//
//

#include "LogoScene.h"
#include "TitleScene.h"
#include "SimpleAudioEngine.h"
#include "ScreenHelper.h"
#include "FileOperation.h"

USING_NS_CC;
using namespace CocosDenshion;

/////////////////////////
// Children Helper
enum TitleLayerChildTags {
	kTitleLayerChildTagLogo = 0,
	kTitleLayerChildTagClipper
};

/////////////////////////
// LogoScene methods
CCScene* LogoScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
	LogoScene *layer = LogoScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool LogoScene::init()
{
    // super init
    if ( !CCLayer::init() )
    {
        return false;
    }

	// turn on touch events
	setTouchEnabled(true);
    
	// initalize the title background
	createTitleBG();
    
    return true;
}

void LogoScene::createTitleBG()
{
	// TODO: need to fix the resource path
	m_LogoSprite = CCSprite::create("logo.png");
	m_LogoSprite->setScale(3);
	// position the sprite on the center of the screen
	m_LogoSprite->setPosition(ccp(VisibleRect::getScreenWidth() / 2, (VisibleRect::getScreenHeight() / 2)-200));
	// add the sprite as a child to this layer
	addChild(m_LogoSprite, kTitleLayerChildTagLogo, kTitleLayerChildTagLogo);

	// try it out
	CCSprite*  clipper;
	clipper = CCSprite::create("MainScreen.png");
	clipper->setPosition(ccp(VisibleRect::getScreenWidth() / 2, (VisibleRect::getScreenHeight() / 2)-300));
	clipper->setScale(2);
	// add the sprite as a child to this layer
	addChild(clipper, kTitleLayerChildTagClipper, kTitleLayerChildTagClipper);

	// delta point
	CCPoint dxPoint = ccp(VisibleRect::getScreenWidth() / 2, (VisibleRect::getScreenHeight() / 2)-150);
	CCPoint dxPoint1 = ccp(dxPoint.x, dxPoint.y + 50);
	CCPoint dxPoint2 = ccp(dxPoint1.x, dxPoint1.y + 50);
	CCPoint dxPoint3 = ccp(dxPoint2.x, dxPoint2.y + 50);
	CCActionInterval* move_ease_in = CCEaseInOut::create(CCMoveTo::create(1, dxPoint), .5f);
	CCActionInterval* move_ease_in1 = CCEaseInOut::create(CCMoveTo::create(1, dxPoint1), .5f);
	CCActionInterval* move_ease_in2 = CCEaseInOut::create(CCMoveTo::create(1, dxPoint2), .5f);
	CCActionInterval* move_ease_in3 = CCEaseInOut::create(CCMoveTo::create(1, dxPoint3), .5f);
	m_LogoSprite->runAction(CCSequence::create(move_ease_in, move_ease_in1, move_ease_in2, move_ease_in3, CCCallFuncN::create(this, callfuncN_selector(LogoScene::callback1)), NULL));

	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("printer.wav", true);
}

void LogoScene::callback1(CCNode* pTarget)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->stopAllEffects();

	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("paperrip.wav");

	CCSprite*  clipper = (CCSprite*)getChildByTag(kTitleLayerChildTagClipper);
	CCDelayTime* readyDelay = CCDelayTime::create(0.5f);
	clipper->runAction(CCSequence::create(CCFadeOut::create(.5f), readyDelay, CCCallFuncN::create(this, callfuncN_selector(LogoScene::callback2)), NULL));
}

void LogoScene::callback2(CCNode* pTarget)
{
	CCDirector::sharedDirector()->replaceScene(TitleScene::scene());
}

void LogoScene::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->stopAllEffects();
	CCDirector::sharedDirector()->replaceScene(TitleScene::scene());
}