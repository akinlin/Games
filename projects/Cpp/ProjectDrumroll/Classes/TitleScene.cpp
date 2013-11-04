//
//  TitleScene.cpp
//  ProjectDrumroll
//
//  Created by Alexander Kinlin on 8/15/13.
//
//

#include "TitleScene.h"
#include "GameScene.h"
#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "ScreenHelper.h"

USING_NS_CC;
using namespace CocosDenshion;

/////////////////////////
// Children Helper
enum TitleLayerChildTags {
	kTitleLayerChildTagTitleBG = 0,
	kTitleLayerChildTagTitleLabel,
	kTitleLayerChildTagTitleMenu
};

/////////////////////////
// Menu Helper
const int LINE_SPACE = 50;
const int ITEM_COUNT = 3;
const std::string menuItem[ITEM_COUNT] =
{
	"Start Game",
	"Hidden Level",
	"Exit"
};
enum TitleMenuItemTags {
	kMenuTagStartGame = 10000,
	kMenuTagHiddenLevel,
	kMenuTagExit
};

/////////////////////////
// TitleScene methods
CCScene* TitleScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    TitleScene *layer = TitleScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool TitleScene::init()
{
    // super init
    if ( !CCLayer::init() )
    {
        return false;
    }

    // initalize the m_TitleMenu
	createTitleMenu();
	// add the m_TitleMenu initalized by createTitleMenu
	addChild(m_TitleMenu, kTitleLayerChildTagTitleMenu);
    
	// initalize the title background
	createTitleBG();
	// add the sprite as a child to this layer
	addChild(m_TitleBGSprite, kTitleLayerChildTagTitleBG);
	addChild(m_TitleLabel, kTitleLayerChildTagTitleLabel);
	    
    return true;
}

void TitleScene::createTitleMenu()
{
	//int contentHeight = 0;
	//int contentWidth = 0;
	//m_TitleMenu = CCMenu::create();
	//for (int i = 0; i < ITEM_COUNT; ++i)
	//{
	//	CCLabelTTF* label = CCLabelTTF::create(menuItem[i].c_str(), "Arial", VisibleRect::getScaledFont(20));
	//	CCMenuItemLabel* pMenuItem = CCMenuItemLabel::create(label, this, menu_selector(TitleScene::menuCallback));
	//	pMenuItem->setAnchorPoint(CCPointZero);

	//	contentHeight = MAX(contentHeight, pMenuItem->getContentSize().height);
	//	contentWidth = MAX(contentWidth, pMenuItem->getContentSize().width);
	//	CCLog("Height = %d", contentHeight);
	//	CCLog("Width = %d", contentWidth);

	//	m_TitleMenu->addChild(pMenuItem, i + kMenuTagStartGame);
	//}

	//m_TitleMenu->setContentSize(CCSizeMake(contentWidth, (ITEM_COUNT - 1) * contentHeight));
	////m_TitleMenu->setPosition(ccp(VisibleRect::getScreenWidth() - m_TitleMenu->getContentSize().width, m_TitleMenu->getContentSize().height));
	//m_TitleMenu->setPosition(0, 0);
	//m_TitleMenu->alignItemsVertically();


	CCLabelTTF* label = CCLabelTTF::create("Click To Start", "Arial", 150);
	CCMenuItemLabel* pMenuItem = CCMenuItemLabel::create(label, this, menu_selector(TitleScene::menuCallback));

	m_TitleMenu = CCMenu::create();
	m_TitleMenu->setAnchorPoint(CCPointZero);
	m_TitleMenu->addChild(pMenuItem, kMenuTagStartGame);
	m_TitleMenu->setContentSize(VisibleRect::getScreenSize());
	m_TitleMenu->setPosition(ccp((VisibleRect::getScreenWidth() / 2) + 50, VisibleRect::getScreenHeight() / 2));
}

void TitleScene::menuCallback(CCObject* pSender)
{
//    // get the userdata, it's the index of the menu item clicked
//    CCMenuItem* pMenuItem = (CCMenuItem *)(pSender);
//    int nIdx = pMenuItem->getZOrder() - 10000;
//    
//    CCLog("Index = %d", nIdx);
//    
//    if (nIdx == ITEM_COUNT - 1)
//    {
//        // last item in the list is exit
//        CCDirector::sharedDirector()->end();
//        
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//        exit(0);
//#endif
//    }
//    else
//    {
//        // create the scene and run it
//        CCScene *pScene = NULL;
//        switch (nIdx)
//        {
//        case 0:
//            pScene = GameScene::scene(); break;
//        case 1:
//            pScene = HelloWorld::scene(); break;
//        default:
//            break;
//        }
//        
//        // run
//        if (pScene)
//        {
//            CCDirector::sharedDirector()->replaceScene(pScene);
//        }
//    }

	// create the scene and run it
	CCScene *pScene = NULL;
	pScene = GameScene::scene();
	      
	// run
	if (pScene)
	{
		CCDirector::sharedDirector()->replaceScene(pScene);
	}
}

void TitleScene::createTitleBG()
{
	// TODO: need to fix the resource path
	m_TitleBGSprite = CCSprite::create("win32/Default.png");
	// position the sprite on the center of the screen
	m_TitleBGSprite->setPosition(ccp(VisibleRect::getScreenWidth() / 2, VisibleRect::getScreenHeight() / 2));

	// create the label
	m_TitleLabel = CCLabelTTF::create("Project Drumroll", "Arial", 200);
	m_TitleLabel->setAnchorPoint(CCPointZero);
	CCLog("Label Height = %d", m_TitleLabel->getContentSize().height);
	CCLog("Label Width = %d", m_TitleLabel->getContentSize().width);
	m_TitleLabel->setPosition(ccp(VisibleRect::getScreenWidth() - m_TitleLabel->getContentSize().width, VisibleRect::getScreenHeight() - m_TitleLabel->getContentSize().height));
}

