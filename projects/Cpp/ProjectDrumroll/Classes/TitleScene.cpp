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
#include "TestDataSave.h"
#include "LevelEditorScene.h"

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
	kMenuTagLevelEditor,
	kMenuTagHiddenLevel,
	kMenuTagExit
};

/////////////////////////
// Local methos
CCMoveBy* getRandMove();

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
    
	// initalize the title background
	createTitleBG();

	// play bg music
	//CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("bg_music.wav", true);

	// schedule a high score scene push
	//CCScheduler::scheduleSelector(schedule_selector(TitleScene::pushHighScoreScene), this, 3.0f, false);
	schedule(schedule_selector(TitleScene::pushHighScoreScene), 7.0f);
	    
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


	CCLabelTTF* label = CCLabelTTF::create("Click To Start", "Arial", VisibleRect::getScaledFont(35));
	CCMenuItemLabel* pMenuItem = CCMenuItemLabel::create(label, this, menu_selector(TitleScene::menuCallback));
	// pulse action
	CCScaleTo* scaleUp = CCScaleTo::create(.75f, 1.1);
	CCScaleTo* scaleDown = CCScaleTo::create(.75f, 1);
	CCSequence* pulseSequence = CCSequence::create(scaleUp, scaleDown, NULL);
	CCRepeatForever* repeat = CCRepeatForever::create(pulseSequence);
	pMenuItem->runAction(repeat);

	CCLabelTTF* labelLevelEditor = CCLabelTTF::create("Level Editor", "Arial", VisibleRect::getScaledFont(27));
	CCMenuItemLabel* pEditorMenuItem = CCMenuItemLabel::create(labelLevelEditor, this, menu_selector(TitleScene::menuLevelEditorCallback));
	pEditorMenuItem->setPosition(ccp(0, -75));
    
    CCLabelTTF* labelExit = CCLabelTTF::create("Exit", "Arial", VisibleRect::getScaledFont(27));
	CCMenuItemLabel* pExitMenuItem = CCMenuItemLabel::create(labelExit, this, menu_selector(TitleScene::menuExitCallback));
	pExitMenuItem->setPosition(ccp(0, -150));

	m_TitleMenu = CCMenu::create();
	m_TitleMenu->setAnchorPoint(CCPointZero);
	m_TitleMenu->addChild(pMenuItem, kMenuTagStartGame);
	m_TitleMenu->addChild(pEditorMenuItem, kMenuTagLevelEditor);
    m_TitleMenu->addChild(pExitMenuItem, kMenuTagExit);
	m_TitleMenu->setContentSize(VisibleRect::getScreenSize());
	m_TitleMenu->setPosition(ccp((VisibleRect::getScreenWidth() / 2) + 50, VisibleRect::getScreenHeight() / 2));
	// add the m_TitleMenu initalized by createTitleMenu
	addChild(m_TitleMenu, kTitleLayerChildTagTitleMenu, kTitleLayerChildTagTitleMenu);

	// create the label
	m_TitleLabel = CCLabelTTF::create("Project Drumroll", "Arial", VisibleRect::getScaledFont(55));
	m_TitleLabel->setAnchorPoint(CCPointZero);
	m_TitleLabel->setPosition(ccp(VisibleRect::getScreenWidth() - m_TitleLabel->getContentSize().width, VisibleRect::getScreenHeight() - m_TitleLabel->getContentSize().height));
	addChild(m_TitleLabel, kTitleLayerChildTagTitleLabel, kTitleLayerChildTagTitleLabel);

	// high score
	// Init the high scores
	const char* KEY_HIGH_SCORE = "high_score";
	char highScoreString[100];
	sprintf(highScoreString, "High Score: %d", CCUserDefault::sharedUserDefault()->getIntegerForKey(KEY_HIGH_SCORE, 100));
	m_highScoreDisplayString = CCLabelTTF::create(highScoreString, "Arial", VisibleRect::getScaledFont(15));
	m_highScoreDisplayString->setAnchorPoint(CCPointZero);
	// yeah yeah its hardcoded, i just didn't want to do that math right now
	m_highScoreDisplayString->setPosition(ccp(50, VisibleRect::getScaledFont(450)));
	addChild(m_highScoreDisplayString, 5);
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

void TitleScene::menuLevelEditorCallback(CCObject* pSender)
{
	// create the scene and run it
	CCScene *pScene = NULL;
	pScene = LevelEditorScene::scene();

	// run
	if (pScene)
	{
		CCDirector::sharedDirector()->replaceScene(pScene);
	}
}

void TitleScene::menuExitCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();
}

void TitleScene::createTitleBG()
{
	// create the layer for the BG
	m_BGLayer = CCLayer::create();

	// TODO: need to fix the resource path
	m_TitleBGSprite = CCSprite::create("Default.png");
	// position the sprite on the center of the screen
	m_TitleBGSprite->setPosition(ccp(VisibleRect::getScreenWidth() / 2, VisibleRect::getScreenHeight() / 2));

	// add the sprite as a child to m_BGLayer
	m_BGLayer->addChild(m_TitleBGSprite, kTitleLayerChildTagTitleBG);
	CCSequence* moveSequence = CCSequence::create(getRandMove(), CCCallFuncN::create(this, callfuncN_selector(TitleScene::callback1)), NULL);
	//CCRepeatForever* repeat = CCRepeatForever::create(moveSequence);
	m_BGLayer->runAction(moveSequence);
	// add layer as child to TitleScene
	addChild(m_BGLayer, kTitleLayerChildTagTitleBG, kTitleLayerChildTagTitleBG);
}

CCMoveBy* getRandMove()
{
	int x = (rand() % 100) - 50;
	int y = (rand() % 100) - 50;

	return CCMoveBy::create(1.0f, ccp(x, y));
}

void TitleScene::callback1(CCNode* pTarget)
{
	CCSequence* moveSequence = CCSequence::create(getRandMove(), CCCallFuncN::create(this, callfuncN_selector(TitleScene::callback1)), NULL);
	m_BGLayer->runAction(moveSequence);
}

void TitleScene::pushHighScoreScene(float ct)
{
	CCDirector::sharedDirector()->pushScene(TestDataSave::scene());
}

