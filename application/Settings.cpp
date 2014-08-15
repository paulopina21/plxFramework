#include "Settings.h"
#include "../utils/log.h"

CSettings* CSettings::INSTANCE = 0;

CSettings::CSettings(QObject* parent): QSettings(parent)
{
  if (INSTANCE) return;
  INSTANCE = this;

  CLog::log( LOG_DEBUG, "Application Settings", "Settings file: " + this->fileName());
}

CSettings::CSettings(const QString& fileName, QSettings::Format format, QObject *parent): QSettings(fileName, format, parent)
{
  if (INSTANCE) return;
  INSTANCE = this;

  CLog::log( LOG_DEBUG, "Application Settings", "Settings file: " + this->fileName());
}

CSettings* CSettings::instance()
{
  if (!INSTANCE) new CSettings();
  return INSTANCE;
}


//void CSettings::setValue(const QString &strGroup, const QString &strName, const QVariant& value)
//{
//  QString strKey;
//  if (strGroup.isEmpty())
//    strKey = strName;

//  strGroup+"/"+strName

//  QSettings::setValue(strKey, value);
//  this->sync();
//}

//QVariant CSettings::getValue(const QString &strGroup, const QString &strName, const QVariant &defaultValue)
//{
//  return this->value(strGroup+"/"+strName, defaultValue);
//}

//QVariant CSettings::getValue(const QString &strField, const QVariant &defaultValue)
//{
//  return this->value(strField, defaultValue);
//}

void CSettings::setFolder(int role, QString strFolderPath)
{
  CSettings::instance()->m_folders[role] = strFolderPath;
}

QString CSettings::folder(int role)
{
  return CSettings::instance()->m_folders[role];
}
