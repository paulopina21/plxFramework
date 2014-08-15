#pragma once

#include <QtGlobal>
//#include <QtXml>
#include "tinyxml/tinyxml.h"


// thus it simplifies the include patterns

//enum NodeType
//{
//	DOCUMENT,
//	ELEMENT,
//	COMMENT,
//	UNKNOWN,
//	TEXT,
//	DECLARATION,
//	TYPECOUNT
//};

class QString;
class TiXmlNode;
class TiXmlDocument;

//namespace plxfw {
//namespace utils {

class XMLUtils {
public:
  static bool HasUTF8Declaration(const QString &strXML);

//  static bool GetHex(const TiXmlNode* pRootNode, const char* strTag, uint32_t& dwHexValue);
//  static bool GetUInt(const TiXmlNode* pRootNode, const char* strTag, uint32_t& dwUIntValue);
  static bool GetLong(const TiXmlNode* pRootNode, const char* strTag, long& lLongValue);
  static bool GetFloat(const TiXmlNode* pRootNode, const char* strTag, float& value);
  static bool GetDouble(const TiXmlNode* pRootNode, const char* strTag, double &value);
  static bool GetInt(const TiXmlNode* pRootNode, const char* strTag, int& iIntValue);
  static bool GetBoolean(const TiXmlNode* pRootNode, const char* strTag, bool& bBoolValue);
  static bool GetString(const TiXmlNode* pRootNode, const char* strTag, QString& strStringValue);
  static bool GetAdditiveString(const TiXmlNode* pRootNode, const char* strTag, const QString& strSeparator, QString &strStringValue);
  static bool GetEncoding(const TiXmlDocument* pDoc, QString& strEncoding);
  static bool GetPath(const TiXmlNode* pRootNode, const char* strTag, QString &strStringValue);
  static bool GetFloat(const TiXmlNode* pRootNode, const char* strTag, float& value, const float min, const float max);
  static bool GetInt(const TiXmlNode* pRootNode, const char* strTag, int& iIntValue, const int min, const int max);

  static void SetString(TiXmlNode* pRootNode, const char *strTag, const QString& strValue);
  static void SetAdditiveString(TiXmlNode* pRootNode, const char *strTag, const QString& strSeparator, const QString& strValue);
  static void SetInt(TiXmlNode* pRootNode, const char *strTag, int value);
  static void SetFloat(TiXmlNode* pRootNode, const char *strTag, float value);
  static void SetBoolean(TiXmlNode* pRootNode, const char *strTag, bool value);
//  static void SetHex(TiXmlNode* pRootNode, const char *strTag, uint32_t value);
  static void SetPath(TiXmlNode* pRootNode, const char *strTag, const QString& strValue);
  static void SetLong(TiXmlNode* pRootNode, const char *strTag, long iValue);
};


//class CXMLFile {
//public:
//  CXMLFile();

//public slots:
//  bool loadFromFile(const QString& strFileName);


//private:
//  QFile*  m_pFile;

//  QString m_content;
//};

//}
//}
