#ifndef LOG_H
#define LOG_H

#include <QObject>
#include <QDateTime>
#include <QDebug>
#include <QFile>

enum eLOGLEVEL
{
  LOG_NONE = 0,
  LOG_FATAL,
  LOG_ERROR,
  LOG_WARNING,
  LOG_INFO,
  LOG_DEBUG
};

class CLog
{
  static CLog* INSTANCE;

public:
  CLog(int iLevel = LOG_INFO, const QString& strFilePath = "");
  static CLog* instance();

  static void setFile(const QString &strFilePath);
  static void setLevel(int iLevel);

  static void log(int level, const QString& strWho, const QString& strWhat, const QString& strOwner = "");

  static QFile* device();

private:
#ifndef Q_OS_ANDROID
  static QFile* m_logFile;
#endif
  static int m_iLevel;
};

//void CLog::log(int level, const QString &strWho, const QString &strWhat, const QString& strOwner)
//{
//}

#endif // LOG_H
