// to enable CCLOG()
#define COCOS2D_DEBUG 1

#include "cocos2d.h"
#include "FileOperation.h"
#include <stdio.h>

using namespace std;

void FileOperation::saveFile()
{
	string path = getFilePath();
	FILE *fp = fopen(path.c_str(), "w");

	if (!fp)
	{
		CCLOG("can not create file %s", path.c_str());
		return;
	}
	cocos2d::CCUserDefault::sharedUserDefault()->flush();

	fputs("<?xml version=\"1.0\" encoding=\"UTF - 8\"?> <!DOCTYPE plist PUBLIC \" -//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\"> <plist version=\"1.0\">     <array>         <dict>             <key>RANK</key>             <integer>1</integer>             <key>SCORE</key>             <integer>60000</integer>             <key>LEVEL</key>             <integer>8</integer>         </dict>         <dict>             <key>RANK</key>             <integer>2</integer>             <key>SCORE</key>             <integer>7500</integer>             <key>LEVEL</key>             <integer>6</integer>         </dict>         <dict>             <key>RANK</key>             <integer>3</integer>             <key>SCORE</key>             <integer>5000</integer>             <key>LEVEL</key>             <integer>4</integer>         </dict>     </array> </plist>", fp);
	fclose(fp);
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

	// Save file
	pDoc->SaveFile(path.c_str());

	if (pDoc)
	{
		delete pDoc;
	}
}

void FileOperation::readFile()
{
	string path = getFilePath();
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
	//path.append("C:/Users/user/Desktop/TBDEntertainment/Games/projects/Cpp/ProjectDrumroll/Resources/");
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