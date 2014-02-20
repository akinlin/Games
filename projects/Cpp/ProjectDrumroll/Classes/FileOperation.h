#ifndef __HELLOWORLD_FILE_OPERATION__
#define __HELLOWORLD_FILE_OPERATION__

#include "platform/win32/CCPlatformDefine.h"
#include <string>
#include "support/tinyxml2/tinyxml2.h"

using namespace tinyxml2;

class CC_DLL FileOperation
{
public:
	static void saveFile(void);
	static void savePlistFile();
	static void readFile(void);
	static std::string getFilePath();

private:
	static XMLElement* addNewElement(tinyxml2::XMLDocument *pDoc, XMLElement *addToEle, const char* key, const char* value);
	static XMLElement* addNewElement(tinyxml2::XMLDocument *pDoc, XMLElement *addToEle, const char* key);
	static XMLElement* addNewElement(tinyxml2::XMLDocument *pDoc, const char* key);
};

#endif
