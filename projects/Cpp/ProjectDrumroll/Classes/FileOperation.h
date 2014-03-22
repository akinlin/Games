#ifndef __HELLOWORLD_FILE_OPERATION__
#define __HELLOWORLD_FILE_OPERATION__

#include "cocos2d.h"
#include "platform/win32/CCPlatformDefine.h"
#include <string>
#include "support/tinyxml2/tinyxml2.h"

using namespace std;
using namespace tinyxml2;

class CC_DLL FileOperation
{
public:
	static void saveFile();
	static void savePlistFile();
	static void saveHighScoreFile(cocos2d::CCArray* highscores);
	static void readFile(void);
	static string getFilePath();
    
    // general file managment
    static bool saveAppFile(string fileName, char* fileContent);
    static char* loadAppFile(string fileName);
    static string getAppFilePath();
    static bool fileCopy(string sourceFilePath, string destFilePath);

private:
	static XMLElement* addNewElement(tinyxml2::XMLDocument *pDoc, XMLElement *addToEle, const char* key, const char* value);
	static XMLElement* addNewElement(tinyxml2::XMLDocument *pDoc, XMLElement *addToEle, const char* key);
	static XMLElement* addNewElement(tinyxml2::XMLDocument *pDoc, const char* key);
};

#endif
