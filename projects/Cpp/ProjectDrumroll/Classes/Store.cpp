//
//  Store.cpp
//  ProjectDrumroll
//
//  Created by Alexander Kinlin on 8/15/13.
//
//

#include "Store.h"
#include "FileOperation.h"

USING_NS_CC;

/////////////////////////
// Store methods

// singleton stuff
static Store *s_SharedStore = NULL;

Store* Store::sharedStore(void)
{
	if (!s_SharedStore)
	{
		s_SharedStore = new Store();
		s_SharedStore->init();
	}

	return s_SharedStore;
}

// on "init" you need to initialize your instance
bool Store::init()
{
	//FileOperation::saveFile();
	//FileOperation::savePlistFile();
	//FileOperation::readFile();

    return true;
}

void Store::finalScoreUpdate(int score, int level)
{
	// open the highscores file and add them to a CCArray
	std::string highScoreFile = FileOperation::getFilePath() + "test.plist";
	CCArray* scores = CCArray::createWithContentsOfFile(highScoreFile.c_str());

	CCObject* arrayElement;
	int index = 0;
	// check if each score is lower than the new score
	CCARRAY_FOREACH(scores, arrayElement)
	{
		CCDictionary* scoreDict = (CCDictionary *)arrayElement;
		int highScore;

		// Get score data.
		highScore = scoreDict->valueForKey("SCORE")->intValue();

		// check if score is greater then the highscore
		if (score > highScore)
		{
			// add the score to the high score list
			CCDictionary* newHighScore = CCDictionary::create();
			newHighScore->setObject(ccs(std::to_string(index + 1)), "RANK");
			newHighScore->setObject(ccs(std::to_string(score)), "SCORE");
			newHighScore->setObject(ccs(std::to_string(level)), "LEVEL");
			scores->insertObject(newHighScore, index);
			CCLog("Got High Score");

			break;
		}
		index++;
	}

	FileOperation::saveHighScoreFile(scores);
}