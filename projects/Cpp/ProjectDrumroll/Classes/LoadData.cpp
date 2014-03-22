//
//  LoadData.cpp
//  ProjectDrumroll
//
//  Created by Alexander Kinlin on 8/15/13.
//
//

#include "LoadData.h"
#include "ScreenHelper.h"
#include "FileOperation.h"
#include "LogoScene.h"
#include <sys/types.h>
#include <sys/stat.h>

USING_NS_CC;

/////////////////////////
// TestDataSave methods
CCScene* LoadData::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
	LoadData *layer = LoadData::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool LoadData::init()
{
    // super init
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    // turn on touch events
	setTouchEnabled(true);
    
    // load the app data
    loadData();
    
    return true;
}

void LoadData::loadData()
{
    // load the directory
    string directoryLoadResult = loadAppDirectory();
    CCLabelTTF* StoredFileExistLabel = CCLabelTTF::create(directoryLoadResult.c_str(), "Arial", VisibleRect::getScaledFont(8));
    StoredFileExistLabel->setAnchorPoint(CCPointZero);
    StoredFileExistLabel->setPosition(ccp(10, VisibleRect::getScreenHeight() - 40));
    addChild(StoredFileExistLabel, 0);
    
    // Load the default high scores
    string highScoreLoadResults = loadHighscores();
    CCLabelTTF* StoredHighScoreFileExistLabel = CCLabelTTF::create(highScoreLoadResults.c_str(), "Arial", VisibleRect::getScaledFont(8));
    StoredHighScoreFileExistLabel->setAnchorPoint(CCPointZero);
    StoredHighScoreFileExistLabel->setPosition(ccp(10, StoredFileExistLabel->getPositionY() - 40));
    addChild(StoredHighScoreFileExistLabel, 1);
    
    // load the level files
    string levelLoadResults = loadLevel();
    CCLabelTTF* levelFileExistLabel = CCLabelTTF::create(levelLoadResults.c_str(), "Arial", VisibleRect::getScaledFont(8));
    levelFileExistLabel->setAnchorPoint(CCPointZero);
    levelFileExistLabel->setPosition(ccp(10, StoredHighScoreFileExistLabel->getPositionY() - 40));
    addChild(levelFileExistLabel, 2);
}

// file directory load
string LoadData::loadAppDirectory()
{
    // Check if directory exists
    // if not create it
    
    // Clean up the instance
    CCFileUtils::sharedFileUtils()->purgeCachedEntries();
    
    // check if the project store directory exists
    string storedFilePath = FileOperation::getAppFilePath();
    CCString* StoredFileExistsPathDisplay;
    if (CCFileUtils::sharedFileUtils()->isFileExist(storedFilePath))
    {
        StoredFileExistsPathDisplay = CCString::createWithFormat("Exists: %s", storedFilePath.c_str());
    }
    else
    {
        // create the directory
        // SYSTEM LEVEL METHOD
        mkdir(storedFilePath.c_str(), 0777);
        StoredFileExistsPathDisplay = CCString::createWithFormat("Created directory at path: %s", storedFilePath.c_str());
    }
    
    return StoredFileExistsPathDisplay->getCString();
}

// high score load
string LoadData::loadHighscores()
{
    string defaultHighScoreFilePath = FileOperation::getFilePath() + "test.plist";
    string storedHighScoreFilePath = FileOperation::getAppFilePath() + "/test.plist";
    bool storedHighScoreFileExists = CCFileUtils::sharedFileUtils()->isFileExist(storedHighScoreFilePath);
    CCString* HighScoreFileExistsPathDisplay;
    if (storedHighScoreFileExists)
    {
        HighScoreFileExistsPathDisplay = CCString::createWithFormat("Exists %s", storedHighScoreFilePath.c_str());
        CCLog(storedHighScoreFilePath.c_str());
    }
    else
    {
        if (FileOperation::fileCopy(defaultHighScoreFilePath, storedHighScoreFilePath))
        {
            HighScoreFileExistsPathDisplay = CCString::createWithFormat("Created file: %s", storedHighScoreFilePath.c_str());
        }
        else
        {
            HighScoreFileExistsPathDisplay = CCString::createWithFormat("File was not created");
        }
        
    }
    return HighScoreFileExistsPathDisplay->getCString();
}

// level load
string LoadData::loadLevel()
{
    string levelFilePath = FileOperation::getFilePath() + "levelA.plist";
    string destLevelFilePath = FileOperation::getAppFilePath() + "/levelA.plist";
    bool destLevelFileExists = CCFileUtils::sharedFileUtils()->isFileExist(destLevelFilePath);
    CCString* destLevelFileExistsPathDisplay;
    if (destLevelFileExists)
    {
        destLevelFileExistsPathDisplay = CCString::createWithFormat("Exists %s", destLevelFilePath.c_str());
    }
    else
    {
        if (FileOperation::fileCopy(levelFilePath, destLevelFilePath))
        {
            destLevelFileExistsPathDisplay = CCString::createWithFormat("Created file: %s", levelFilePath.c_str());
        }
        else
        {
            destLevelFileExistsPathDisplay = CCString::createWithFormat("File was not created");
        }
        
    }
    
    // load levelB
    levelFilePath = FileOperation::getFilePath() + "levelB.plist";
    destLevelFilePath = FileOperation::getAppFilePath() + "/levelB.plist";
    destLevelFileExists = CCFileUtils::sharedFileUtils()->isFileExist(destLevelFilePath);
    if (destLevelFileExists)
    {
        destLevelFileExistsPathDisplay = CCString::createWithFormat("Exists %s", destLevelFilePath.c_str());
    }
    else
    {
        if (FileOperation::fileCopy(levelFilePath, destLevelFilePath))
        {
            destLevelFileExistsPathDisplay = CCString::createWithFormat("Created file: %s", levelFilePath.c_str());
        }
        else
        {
            destLevelFileExistsPathDisplay = CCString::createWithFormat("File was not created");
        }
        
    }
    return destLevelFileExistsPathDisplay->getCString();
}

void LoadData::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
	CCDirector::sharedDirector()->replaceScene(LogoScene::scene());
}
