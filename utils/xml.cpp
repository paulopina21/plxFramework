#include "xml.h"

#include <QString>

//using namespace plxfw::utils;

//bool XMLUtils::GetHex(const TiXmlNode* pRootNode, const char* strTag, uint32_t& hexValue)
//{
//  const TiXmlNode* pNode = pRootNode->FirstChild(strTag );
//  if (!pNode || !pNode->FirstChild()) return false;
//  sscanf(pNode->FirstChild()->Value(), "%x", (uint32_t*) &hexValue );
//  return true;
//}


//bool XMLUtils::GetUInt(const TiXmlNode* pRootNode, const char* strTag, uint32_t &uintValue)
//{
//  const TiXmlNode* pNode = pRootNode->FirstChild(strTag );
//  if (!pNode || !pNode->FirstChild()) return false;
//  uintValue = atol(pNode->FirstChild()->Value());
//  return true;
//}

bool XMLUtils::GetLong(const TiXmlNode* pRootNode, const char* strTag, long& lLongValue)
{
  const TiXmlNode* pNode = pRootNode->FirstChild(strTag );
  if (!pNode || !pNode->FirstChild()) return false;
  lLongValue = atol(pNode->FirstChild()->Value());
  return true;
}

bool XMLUtils::GetInt(const TiXmlNode* pRootNode, const char* strTag, int& iIntValue)
{
  const TiXmlNode* pNode = pRootNode->FirstChild(strTag );
  if (!pNode || !pNode->FirstChild()) return false;
  iIntValue = atoi(pNode->FirstChild()->Value());
  return true;
}

bool XMLUtils::GetInt(const TiXmlNode* pRootNode, const char* strTag, int &value, const int min, const int max)
{
  if (GetInt(pRootNode, strTag, value))
  {
    if (value < min) value = min;
    if (value > max) value = max;
    return true;
  }
  return false;
}

bool XMLUtils::GetDouble(const TiXmlNode *root, const char *tag, double &value)
{
  const TiXmlNode* node = root->FirstChild(tag);
  if (!node || !node->FirstChild()) return false;
  value = atof(node->FirstChild()->Value());
  return true;
}

bool XMLUtils::GetFloat(const TiXmlNode* pRootNode, const char* strTag, float& value)
{
  const TiXmlNode* pNode = pRootNode->FirstChild(strTag );
  if (!pNode || !pNode->FirstChild()) return false;
  value = (float)atof(pNode->FirstChild()->Value());
  return true;
}

bool XMLUtils::GetFloat(const TiXmlNode* pRootElement, const char *tagName, float& fValue, const float fMin, const float fMax)
{
  if (GetFloat(pRootElement, tagName, fValue))
  { // check range
    if (fValue < fMin) fValue = fMin;
    if (fValue > fMax) fValue = fMax;
    return true;
  }
  return false;
}

bool XMLUtils::GetBoolean(const TiXmlNode* pRootNode, const char* strTag, bool& bBoolValue)
{
  const TiXmlNode* pNode = pRootNode->FirstChild(strTag );
  if (!pNode || !pNode->FirstChild()) return false;
  QString strEnabled = pNode->FirstChild()->Value();
  strEnabled = strEnabled.toLower();
  if (strEnabled == "off" || strEnabled == "no" || strEnabled == "disabled" || strEnabled == "false" || strEnabled == "0" )
    bBoolValue = false;
  else
  {
    bBoolValue = true;
    if (strEnabled != "on" && strEnabled != "yes" && strEnabled != "enabled" && strEnabled != "true")
      return false; // invalid bool switch - it's probably some other string.
  }
  return true;
}

bool XMLUtils::GetString(const TiXmlNode* pRootNode, const char* strTag, QString &strStringValue)
{
  const TiXmlElement* pElement = pRootNode->FirstChildElement(strTag );
  if (!pElement) return false;
  //const char* encoded = pElement->Attribute("urlencoded");
  const TiXmlNode* pNode = pElement->FirstChild();
  if (pNode != NULL)
  {
    strStringValue = pNode->Value();
    //if (encoded && strcasecmp(encoded,"yes") == 0)
    //  CUtil::URLDecode(strStringValue);
    return true;
  }
  strStringValue.clear();
  return false;
}

bool XMLUtils::GetAdditiveString(const TiXmlNode* pRootNode, const char* strTag,
                                 const QString& strSeparator, QString& strStringValue)
{
  QString strTemp;
  const TiXmlElement* node = pRootNode->FirstChildElement(strTag);
  bool bResult=false;
  while (node)
  {
    if (node->FirstChild())
    {
      bResult = true;
      strTemp = node->FirstChild()->Value();
      const char* clear=node->Attribute("clear");
      if (strStringValue.isEmpty() || (clear /*&& strcasecmp(clear,"true")==0*/))
        strStringValue = strTemp;
      else
        strStringValue += strSeparator+strTemp;
    }
    node = node->NextSiblingElement(strTag);
  }

  return bResult;
  return false;
}

/*!
  Returns true if the encoding of the document is other then UTF-8.
  /param strEncoding Returns the encoding of the document. Empty if UTF-8
*/
bool XMLUtils::GetEncoding(const TiXmlDocument* pDoc, QString& strEncoding)
{
  const TiXmlNode* pNode=NULL;
  while ((pNode=pDoc->IterateChildren(pNode)) && pNode->Type()!=TiXmlNode::TINYXML_DECLARATION) {}

  if (!pNode) return false;

  const TiXmlDeclaration* pDecl=pNode->ToDeclaration();

  if (!pDecl) return false;
  strEncoding=pDecl->Encoding();

  if (strEncoding == "UTF-8" || strEncoding == "UTF8")
    strEncoding.clear();

  strEncoding = strEncoding.toUpper();//MakeUpper();
  return !strEncoding.isEmpty(); // Other encoding then UTF8?
}

/*!
  Returns true if the encoding of the document is specified as as UTF-8
  /param strXML The XML file (embedded in a string) to check.
*/
bool XMLUtils::HasUTF8Declaration(const QString &strXML)
{
  QString test = strXML;
  test = test.toLower();
  // test for the encoding="utf-8" string
  if (test.contains("encoding=\"utf-8\"", Qt::CaseInsensitive) >= 0)
    return true;
  // TODO: test for plain UTF8 here?
  return false;
}

bool XMLUtils::GetPath(const TiXmlNode* pRootNode, const char* strTag, QString& strStringValue)
{
  const TiXmlElement* pElement = pRootNode->FirstChildElement(strTag);
  if (!pElement) return false;

  int pathVersion = 0;
  pElement->Attribute("pathversion", &pathVersion);
  //const char* encoded = pElement->Attribute("urlencoded");
  const TiXmlNode* pNode = pElement->FirstChild();
  if (pNode != NULL)
  {
    strStringValue = pNode->Value();
  //  if (encoded && strcasecmp(encoded,"yes") == 0)
  //    CUtil::URLDecode(strStringValue);
  //  strStringValue = CSpecialProtocol::ReplaceOldPath(strStringValue, pathVersion);
    return true;
  }
  strStringValue.clear();
  return false;
}

void XMLUtils::SetAdditiveString(TiXmlNode* pRootNode, const char *strTag, const QString& strSeparator, const QString& strValue)
{
//  QStringArray list;
//  StringUtils::SplitString(strValue,strSeparator,list);
//  for (unsigned int i=0;i<list.size() && !list[i].IsEmpty();++i)
//    SetString(pRootNode,strTag,list[i]);
}

void XMLUtils::SetString(TiXmlNode* pRootNode, const char *strTag, const QString &strValue)
{
  TiXmlElement newElement(strTag);
  TiXmlNode *pNewNode = pRootNode->InsertEndChild(newElement);
  if (pNewNode)
  {
    TiXmlText value(strValue.toStdString().c_str());
    pNewNode->InsertEndChild(value);
  }
}

void XMLUtils::SetInt(TiXmlNode* pRootNode, const char *strTag, int value)
{
  QString strValue;
  strValue = value;
//  strValue.format("%i", value);
  SetString(pRootNode, strTag, strValue);
}

void XMLUtils::SetLong(TiXmlNode* pRootNode, const char *strTag, long value)
{
  QString strValue;
  strValue = value;
  //strValue.Format("%l", value);
  SetString(pRootNode, strTag, strValue);
}

void XMLUtils::SetFloat(TiXmlNode* pRootNode, const char *strTag, float value)
{
  QString strValue;
  strValue = value;
//  strValue.Format("%f", value);
  SetString(pRootNode, strTag, strValue);
}

void XMLUtils::SetBoolean(TiXmlNode* pRootNode, const char *strTag, bool value)
{
  SetString(pRootNode, strTag, value ? "true" : "false");
}

//void XMLUtils::SetHex(TiXmlNode* pRootNode, const char *strTag, uint32_t value)
//{
//  CStdString strValue;
//  strValue.Format("%x", value);
//  SetString(pRootNode, strTag, strValue);
//}

void XMLUtils::SetPath(TiXmlNode* pRootNode, const char *strTag, const QString& strValue)
{
  TiXmlElement newElement(strTag);
  //newElement.SetAttribute("pathversion", CSpecialProtocol::path_version);
  TiXmlNode *pNewNode = pRootNode->InsertEndChild(newElement);
  if (pNewNode)
  {
    TiXmlText value(strValue.toStdString().c_str());
    pNewNode->InsertEndChild(value);
  }
}


//CXMLFile::CXMLFile()
//{
//  m_pFile = new QFile();
//}

//bool CXMLFile::loadFromFile(const QString& strFileName)
//{
//  m_pFile->setFileName(strFileName);

//  if (!m_pFile->open(QFile::ReadWrite | QFile::Text))
//  {
//    qDebug() << "CXMLFile cant open file: " << strFileName;
//    return false;
//  }

//  m_content = m_pFile->readAll();
//}
