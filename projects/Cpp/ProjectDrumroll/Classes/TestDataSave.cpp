//
//  TitleScene.cpp
//  ProjectDrumroll
//
//  Created by Alexander Kinlin on 8/15/13.
//
//

#include "TestDataSave.h"
#include "GameScene.h"
#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "ScreenHelper.h"
#include "FileOperation.h"

USING_NS_CC;
using namespace CocosDenshion;

/////////////////////////
// TestDataSave methods
CCScene* TestDataSave::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
	TestDataSave *layer = TestDataSave::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool TestDataSave::init()
{
    // super init
    if ( !CCLayer::init() )
    {
        return false;
    }

	//FileOperation::saveFile();
	FileOperation::savePlistFile();
	//FileOperation::readFile();

	CCLabelTTF* highscoretitle = CCLabelTTF::create("Rank      Score      Level", "Arial", VisibleRect::getScaledFont(15));
	highscoretitle->setAnchorPoint(CCPointZero);
	// yeah yeah its hardcoded, i just didn't want to do that math right now
	highscoretitle->setPosition(ccp(50, VisibleRect::getScaledFont(450)));
	addChild(highscoretitle, 0);

	loadPlist("test.plist");

	// set an action to pop scene
	CCDelayTime* timeToLive = CCDelayTime::create(7.0f);
	CCCallFunc* CCCallFuncpopScene = CCCallFunc::create(this, callfunc_selector(TestDataSave::popScene));
	CCSequence* newSeq = (CCSequence*)CCSequence::create(timeToLive, CCCallFuncpopScene, NULL);
	this->runAction(newSeq);

    return true;
}

void TestDataSave::loadPlist(const char* plistFile)
{
	std::string highScoreFile = FileOperation::getFilePath() + plistFile;//CCFileUtils::sharedFileUtils()->fullPathForFilename(plistFile);
	CCLog("highScoreFile %s", highScoreFile.c_str());
	CCArray* scores = CCArray::createWithContentsOfFile(highScoreFile.c_str());

	if (scores != NULL)
	{
		// try writing to the data array and saving it back
		CCDictionary* newScore = CCDictionary::create();
		newScore->setObject(ccs("4"), "RANK");
		newScore->setObject(ccs("750"), "SCORE");
		newScore->setObject(ccs("1"), "LEVEL");
		scores->addObject(newScore);

		CCObject* arrayElement;
		int index = 0;
		CCARRAY_FOREACH(scores, arrayElement)
		{
			CCDictionary* scoreDict = (CCDictionary *)arrayElement;
			int rank, score, level;

			// Get data for the given key. As you can see below, you can get this data within the format you expect (string, int, float....)
			rank = scoreDict->valueForKey("RANK")->intValue();
			score = scoreDict->valueForKey("SCORE")->intValue();
			level = scoreDict->valueForKey("LEVEL")->intValue();

			CCLog("rank: %d score: %d level: %d", rank, score, level);

			char highScoreString[100];
			sprintf(highScoreString, "%d      %d      %d", rank, score, level);
			m_highScoreDisplayString = CCLabelTTF::create(highScoreString, "Arial", VisibleRect::getScaledFont(15));
			m_highScoreDisplayString->setAnchorPoint(CCPointZero);
			// yeah yeah its hardcoded, i just didn't want to do that math right now
			m_highScoreDisplayString->setPosition(ccp(50, VisibleRect::getScaledFont(400) - (50 * index)));
			addChild(m_highScoreDisplayString, index+1);

			index++;
		}
	}
	else
	{
		CCLog("scores NULL");
	}

}

void TestDataSave::popScene()
{
	CCDirector::sharedDirector()->popScene();
}
