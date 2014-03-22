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
#include "ScreenHelper.h"
#include <sys/types.h>
#include <sys/stat.h>

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

    // Test file paths
//    string utilsFilePath = CCFileUtils::sharedFileUtils()->getWritablePath();
//    CCString* utilsFileDisplay = CCString::createWithFormat("Shared File Utils Path: %s", utilsFilePath.c_str());
//    CCLog(utilsFileDisplay->getCString());
//
//    CCLabelTTF* fileUtilsPathLabel = CCLabelTTF::create(utilsFileDisplay->getCString(), "Arial", VisibleRect::getScaledFont(10));
//    fileUtilsPathLabel->setAnchorPoint(CCPointZero);
//    fileUtilsPathLabel->setPosition(ccp(0, VisibleRect::getScaledFont(420)));
//	addChild(fileUtilsPathLabel, 1);
//
//    // Resource File reading
//    
//    // Clean up the instance
//    CCFileUtils::sharedFileUtils()->purgeCachedEntries();
//    // check if a file exists
//    string fileExistsPath = CCFileUtils::sharedFileUtils()->fullPathForFilename("levelA.plist");
//    bool fileExists = CCFileUtils::sharedFileUtils()->isFileExist(fileExistsPath);
//    CCString* fileExistsPathDisplay;
//    if (fileExists)
//    {
//        fileExistsPathDisplay = CCString::createWithFormat("Exists at path: %s", fileExistsPath.c_str());
//        CCLog(fileExistsPath.c_str());
//    }
//    else
//    {
//        fileExistsPathDisplay = CCString::createWithFormat("File does not exist");
//    }
//    CCLabelTTF* fileExistLabel = CCLabelTTF::create(fileExistsPathDisplay->getCString(), "Arial", VisibleRect::getScaledFont(10));
//    fileExistLabel->setAnchorPoint(CCPointZero);
//    fileExistLabel->setPosition(ccp(0, VisibleRect::getScaledFont(390)));
//	addChild(fileExistLabel, 2);
//
//    // File saving and access
//    
//    // Clean up the instance
//    CCFileUtils::sharedFileUtils()->purgeCachedEntries();
//    // check if a file exists
//    string storedFilePath = FileOperation::getAppFilePath() + "/levelA.plist";
//    bool storedFileExists = CCFileUtils::sharedFileUtils()->isFileExist(storedFilePath);
//    CCString* StoredFileExistsPathDisplay;
//    if (storedFileExists)
//    {
//        StoredFileExistsPathDisplay = CCString::createWithFormat("Exists at path: %s", storedFilePath.c_str());
//        CCLog(storedFilePath.c_str());
//    }
//    else
//    {
//        StoredFileExistsPathDisplay = CCString::createWithFormat("File does not exist");
//        FileOperation::saveAppFile(storedFilePath);
//    }
//    CCLabelTTF* StoredFileExistLabel = CCLabelTTF::create(StoredFileExistsPathDisplay->getCString(), "Arial", VisibleRect::getScaledFont(10));
//    StoredFileExistLabel->setAnchorPoint(CCPointZero);
//    StoredFileExistLabel->setPosition(ccp(0, VisibleRect::getScaledFont(360)));
//	addChild(StoredFileExistLabel, 3);

    // high score display
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
	std::string highScoreFile = FileOperation::getAppFilePath() + "/" + plistFile;//CCFileUtils::sharedFileUtils()->fullPathForFilename(plistFile);
	CCArray* scores = NULL;
    if (CCFileUtils::sharedFileUtils()->isFileExist(highScoreFile))
    {
        scores = CCArray::createWithContentsOfFile(highScoreFile.c_str());
    }
    else
    {
        // copy the file to the app storage if it does not exist
        string copyPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(plistFile);
        FILE *fp = fopen(copyPath.c_str(), "r");
        char buf[5000] = { 0 };
        
        if (!fp)
        {
            CCLOG("can not open file %s", copyPath.c_str());
            return;
        }
        
        fgets(buf, 5000, fp);
        
        FILE *wfp = fopen(copyPath.c_str(), "w");
        
        if (wfp)
        {
            cocos2d::CCUserDefault::sharedUserDefault()->flush();
            fputs(buf, wfp);
            fclose(wfp);
            CCLog("%s file created", copyPath.c_str());
        }
        else
        {
            CCLog("can not create file %s", copyPath.c_str());
        }
        
        CCLOG("read content %s", buf);
        
        fclose(fp);

    }
    CCLog("highScoreFile %s", highScoreFile.c_str());
	

	if (scores != NULL)
	{
		// try writing to the data array and saving it back
		/*CCDictionary* newScore = CCDictionary::create();
		newScore->setObject(ccs("4"), "RANK");
		newScore->setObject(ccs("750"), "SCORE");
		newScore->setObject(ccs("1"), "LEVEL");
		scores->addObject(newScore);*/

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
