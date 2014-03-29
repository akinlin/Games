// to enable CCLOG()
#define COCOS2D_DEBUG 1

#include "FileOperation.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

using namespace std;
using namespace cocos2d;

const char* gameSaveDirecotry = "com.tbd.projectdrumroll";

void FileOperation::saveFile()
{
	string path = getFilePath() + "savetest.txt";
	FILE *fp = fopen(path.c_str(), "w");

	if (!fp)
	{
		CCLOG("can not create file %s", path.c_str());
		return;
	}
	cocos2d::CCUserDefault::sharedUserDefault()->flush();

	fputs("something", fp);
	fclose(fp);
    
    CCLOG("Saved file %s", path.c_str());
}

void FileOperation::savePlistFile()
{
	string path = getFilePath() + "test.plist";
	// create the XML Document
	CCLOG("Creating xml file %s", path.c_str());
	tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();
	if (NULL == pDoc)
	{
		CCLOG("tinyxml2::XMLDocument failed to create");
		return;
	}
	// Add the XML Declaration
	tinyxml2::XMLDeclaration *pDeclaration = pDoc->NewDeclaration();
	if (NULL == pDeclaration)
	{
		CCLOG("tinyxml2::XMLDeclaration failed to create");
		return;
	}
	pDoc->LinkEndChild(pDeclaration);
	// Start the Root Element
	tinyxml2::XMLElement *pRootEle = addNewElement(pDoc, "plist");
	pRootEle->SetAttribute("version", "1.0");

	// array Element
	tinyxml2::XMLElement *pArrayEle = addNewElement(pDoc, pRootEle, "array");

	// Start the Dict1 Element
	tinyxml2::XMLElement *pDictEle = addNewElement(pDoc, pArrayEle, "dict");

	// save the array of Dicts to file
	// RANK
	tinyxml2::XMLElement *pKey = addNewElement(pDoc, pDictEle, "key", "RANK");
	tinyxml2::XMLElement *pInteger = addNewElement(pDoc, pDictEle, "integer", "111");
	// SCORE
	pKey = addNewElement(pDoc, pDictEle, "key", "SCORE");
	pInteger = addNewElement(pDoc, pDictEle, "integer", "10000");
	// LEVEL
	pKey = addNewElement(pDoc, pDictEle, "key", "LEVEL");
	pInteger = addNewElement(pDoc, pDictEle, "integer", "8");

	// Start the Dict2 Element
	pDictEle = addNewElement(pDoc, pArrayEle, "dict");
	// RANK
	pKey = addNewElement(pDoc, pDictEle, "key", "RANK");
	pInteger = addNewElement(pDoc, pDictEle, "integer", "2");
	// SCORE
	pKey = addNewElement(pDoc, pDictEle, "key", "SCORE");
	pInteger = addNewElement(pDoc, pDictEle, "integer", "7500");
	// LEVEL
	pKey = addNewElement(pDoc, pDictEle, "key", "LEVEL");
	pInteger = addNewElement(pDoc, pDictEle, "integer", "6");

	// Start the Dict3 Element
	pDictEle = addNewElement(pDoc, pArrayEle, "dict");
	// RANK
	pKey = addNewElement(pDoc, pDictEle, "key", "RANK");
	pInteger = addNewElement(pDoc, pDictEle, "integer", "3");
	// SCORE
	pKey = addNewElement(pDoc, pDictEle, "key", "SCORE");
	pInteger = addNewElement(pDoc, pDictEle, "integer", "5000");
	// LEVEL
	pKey = addNewElement(pDoc, pDictEle, "key", "LEVEL");
	pInteger = addNewElement(pDoc, pDictEle, "integer", "4");
    
    cocos2d::CCUserDefault::sharedUserDefault()->flush();

	// Save file
	pDoc->SaveFile(path.c_str());

	if (pDoc)
	{
		delete pDoc;
	}
}

void FileOperation::saveHighScoreFile(CCArray* highscores)
{
	string path = getAppFilePath() + "/test.plist";
	// create the XML Document
	CCLOG("Creating xml file %s", path.c_str());
	tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();
	if (NULL == pDoc)
	{
		CCLOG("tinyxml2::XMLDocument failed to create");
		return;
	}
	// Add the XML Declaration
	tinyxml2::XMLDeclaration *pDeclaration = pDoc->NewDeclaration();
	if (NULL == pDeclaration)
	{
		CCLOG("tinyxml2::XMLDeclaration failed to create");
		return;
	}
	pDoc->LinkEndChild(pDeclaration);
	// Start the Root Element
	tinyxml2::XMLElement *pRootEle = addNewElement(pDoc, "plist");
	pRootEle->SetAttribute("version", "1.0");

	// array Element
	tinyxml2::XMLElement *pArrayEle = addNewElement(pDoc, pRootEle, "array");

	// save the array of Dicts to file
	CCObject* arrayElement;
	int index = 0;
	int currentHighScore = 0;
	tinyxml2::XMLElement *pDictEle;
	tinyxml2::XMLElement *pKey;
	tinyxml2::XMLElement *pInteger;
	// check if each score is lower than the new score
	CCARRAY_FOREACH(highscores, arrayElement)
	{
		CCDictionary* scoreDict = (CCDictionary *)arrayElement;
	
		// add the score to the high score list
		pDictEle = addNewElement(pDoc, pArrayEle, "dict");
		// RANK
		pKey = addNewElement(pDoc, pDictEle, "key", "RANK");
		pInteger = addNewElement(pDoc, pDictEle, "integer", CCString::createWithFormat("%d", index + 1)->getCString());
        //CCLog("Saved Rank %s", std::to_string(index + 1).c_str());
		// SCORE
		pKey = addNewElement(pDoc, pDictEle, "key", "SCORE");
		pInteger = addNewElement(pDoc, pDictEle, "integer", scoreDict->valueForKey("SCORE")->getCString());

		// if this is the first score save it to display later
		if (index == 0)
		{
			currentHighScore = scoreDict->valueForKey("SCORE")->intValue();
		}

		// LEVEL
		pKey = addNewElement(pDoc, pDictEle, "key", "LEVEL");
		pInteger = addNewElement(pDoc, pDictEle, "integer", scoreDict->valueForKey("LEVEL")->getCString());

		index++;

		// dont write more than 10 scores
		if (index > 9)
		{
			break;
		}
	}
    
    cocos2d::CCUserDefault::sharedUserDefault()->flush();

	// Save file
	pDoc->SaveFile(path.c_str(), false);

	if (pDoc)
	{
		delete pDoc;
	}

	// Save the highest score
	const char* KEY_HIGH_SCORE = "high_score";
	CCUserDefault::sharedUserDefault()->setIntegerForKey(KEY_HIGH_SCORE, currentHighScore);
}

void FileOperation::readFile()
{
	string path = getFilePath() + "savetest.txt";
	FILE *fp = fopen(path.c_str(), "r");
	char buf[50] = { 0 };

	if (!fp)
	{
		CCLOG("can not open file %s", path.c_str());
		return;
	}

	fgets(buf, 50, fp);
	CCLOG("read content %s", buf);

	fclose(fp);
}

string FileOperation::getFilePath()
{
	string path("");

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	// In android, every programe has a director under /data/data.
	// The path is /data/data/ + start activity package name.
	// You can save application specific data here.
	path.append("/data/data/org.cocos2dx.application/tmpfile");
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	// You can save file in anywhere if you have the permision.
	path = cocos2d::CCFileUtils::sharedFileUtils()->getWritablePath();
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WOPHONE)
	path = cocos2d::CCApplication::sharedApplication().getAppDataPath();

#ifdef _TRANZDA_VM_
	// If runs on WoPhone simulator, you should insert "D:/Work7" at the
	// begin. We will fix the bug in no far future.
	path = "D:/Work7" + path;
	path.append("tmpfile");
#endif

#endif

	return path;
}

bool FileOperation::saveAppFile(string fileName, char* fileContent)
{
    // check if a file exists
    //string fileDirPath = CCFileUtils::sharedFileUtils()->getWritablePath() + gameSaveDirecotry;
    string filePath = fileName;//fileDirPath + "/" + fileName;
        
    FILE *fp = fopen(filePath.c_str(), "w");
        
    if (fp)
    {
        cocos2d::CCUserDefault::sharedUserDefault()->flush();
        fputs(fileContent, fp);
        fclose(fp);
        CCLog("%s file created", filePath.c_str());
        return true;
    }
    else
    {
        CCLog("can not create file %s", filePath.c_str());
        return false;
    }
}

char* FileOperation::loadAppFile(string fileName)
{
    // check if a file exists
    //string fileDirPath = CCFileUtils::sharedFileUtils()->getWritablePath() + gameSaveDirecotry;
    string filePath = fileName;//fileDirPath + "/" + fileName;
    
    FILE *fp = fopen(filePath.c_str(), "r");
    char buf[500] = { 0 };
    char* content;
    
	if (fp)
	{
		fgets(buf, 500, fp);
        content = buf;
        fclose(fp);
	}
    
    return content;
}

string FileOperation::getAppFilePath()
{
    return CCFileUtils::sharedFileUtils()->getWritablePath() + gameSaveDirecotry;
}

bool FileOperation::fileCopy(string sourceFilePath, string destFilePath)
{
    // buffer from source file
    unsigned char* buf;
    // buffer size
    unsigned long size;
    
    // get the source file data
    buf = CCFileUtils::sharedFileUtils()->getFileData(sourceFilePath.c_str(), "r", &size);
    if (!buf)
    {
        // source file could not be copied
        CCLog("FileOperation::fileCopy - source file could not be copied: %s", buf);
        return false;
    }
    
    // open the dest file for writing
    FILE* dest = fopen(destFilePath.c_str(), "wb");
    if (!dest)
    {
        // dest file could not be opened
        CCLog("FileOperation::fileCopy - dest file could not be opened: %s", destFilePath.c_str());
        return false;
    }

    // write the dest file with the source file buffer
    fwrite(buf, 1, size, dest);
    // close the dest file
    fclose(dest);
    
    return true;
}

tinyxml2::XMLElement* FileOperation::addNewElement(tinyxml2::XMLDocument *pDoc, const char* name)
{
	XMLElement *pElem = pDoc->NewElement(name);
	pDoc->LinkEndChild(pElem);
	return pElem;
}

tinyxml2::XMLElement* FileOperation::addNewElement(tinyxml2::XMLDocument *pDoc, XMLElement *addToEle, const char* name)
{
	XMLElement *pElem = pDoc->NewElement(name);
	addToEle->LinkEndChild(pElem);
	return pElem;
}

tinyxml2::XMLElement* FileOperation::addNewElement(tinyxml2::XMLDocument *pDoc, XMLElement *addToEle, const char* name, const char* value)
{
	tinyxml2::XMLElement* pElem = pDoc->NewElement(name);
	tinyxml2::XMLText* pValue = pDoc->NewText(value);
	pElem->LinkEndChild(pValue);
	addToEle->LinkEndChild(pElem);
	return pElem;
}
