#include "log.h"


CLog* CLog::INSTANCE = 0;
#ifndef Q_OS_ANDROID
QFile* CLog::m_logFile = 0;
#endif

int CLog::m_iLevel = LOG_INFO;

CLog::CLog(int iLevel, const QString& strFilePath)
{
  if (INSTANCE) return;
  INSTANCE = this;
  m_iLevel = iLevel;

#ifndef Q_OS_ANDROID
  m_logFile = new QFile(strFilePath);
#endif

  setFile(strFilePath);
}

CLog* CLog::instance()
{
  if (!INSTANCE)
    new CLog();

  return INSTANCE;
}

void CLog::setFile(const QString& strFilePath)
{
#ifndef Q_OS_ANDROID

  if ( strFilePath.isEmpty() )
  {
    m_logFile->open(stdout, QIODevice::WriteOnly|QIODevice::Text);
    return;
  }

  m_logFile->setFileName(strFilePath);

  if (!m_logFile->open(QIODevice::WriteOnly|QIODevice::Text))
    m_logFile->open(stdout, QIODevice::WriteOnly|QIODevice::Text);
#endif
}

void CLog::setLevel(int iLevel)
{
  m_iLevel = iLevel;
}

void CLog::log(int level, const QString& strWho, const QString& strWhat, const QString& strOwner)
{
  if (level > m_iLevel) return;

  QString strLevel;

  switch (level)
  {
    case LOG_DEBUG:
      strLevel = "[DBG]";
    break;
    case LOG_INFO:
      strLevel = "[INF]";
    break;
    case LOG_WARNING:
      strLevel = "[WAR]";
    break;
    case LOG_ERROR:
      strLevel = "[ERR]";
    break;
    case LOG_FATAL:
      strLevel = "[FATAL]";
    default:
    break;
  }

  QString strWhoMod = strWho;
  if (!strOwner.isEmpty())
  {
    strWhoMod.append(" -> " +strOwner);
//        strWhoMod.append("("+strOwner+")");
  }

  QString strNow = QDateTime::currentDateTime().toString("[yyyyddMM|HH:mm:ss:zzz]");
  QString strMsg = strNow + strLevel.toUpper() + " " + strWhoMod.toUpper() + ": " + strWhat + "\n";

#ifndef Q_OS_ANDROID
  m_logFile->write(strMsg.toStdString().c_str(),strMsg.size());
  m_logFile->flush();
#endif
#ifdef Q_OS_ANDROID
  qDebug() << strMsg.toStdString().c_str();
#endif
}

QFile*CLog::device()
{
#ifndef Q_OS_ANDROID
  return m_logFile;
#endif
}
