#include "database.h"

#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlField>
#include <QtSql/QSqlError>
#include <QtSql/QSqlResult>

#include <QFile>
#include "../utils/log.h"

QSqlDatabase* CDatabase::m_pDB = new QSqlDatabase();

/*******************************************************************************
    Database
*******************************************************************************/
CDatabase::CDatabase(QObject *parent): QObject(parent)
{
  m_strConnectionName = "";
  m_strDBRevision     = "0.0";
  m_pDB = NULL;
  m_pDS = NULL;
}


CDatabase::~CDatabase()
{
  close();
  if(m_pDB != NULL)
  {
    QSqlDatabase::removeDatabase(m_strConnectionName);
    //m_pDB->removeDatabase(m_strConnectionName);
    delete m_pDB;
  }
  if (m_pDS != NULL)
    delete m_pDS;
}

bool CDatabase::beginTransaction()
{
  CLog::log(LOG_INFO,"DATABASE", ": Starting transaction", m_strConnectionName);
  return m_pDB->driver()->beginTransaction();
}

bool CDatabase::commitTransaction()
{
  CLog::log(LOG_INFO,"DATABASE", ": Commiting transaction", m_strConnectionName);
  return m_pDB->driver()->commitTransaction();
}

bool CDatabase::rollbackTransaction()
{
  CLog::log(LOG_INFO,"DATABASE", ": Rollingback transaction", m_strConnectionName);
  return m_pDB->driver()->rollbackTransaction();
}

//inline QSqlQuery& Database::Query()
//{
//  if (m_query == NULL)
//    qDebug() << "- Database -> Query is NULL. Database is connected and opened?? - " << m_strConnectionName;


//}

bool CDatabase::query(const QString &strQuery, ...)
{
  if (!m_pDS)
  {
    return false;
    CLog::log(LOG_ERROR,"DATABASE", "DRIVER NOT LOADED", m_strConnectionName);
  }

  CLog::log(LOG_DEBUG,"DATABASE", "Query: "+strQuery, m_strConnectionName);
  bool bExec = m_pDS->exec(strQuery);

  if (!bExec)
    CLog::log(LOG_ERROR,"DATABASE", m_pDS->lastError().text(), m_strConnectionName);

  return bExec;
}

/*******************************************************************************
    database connection
*******************************************************************************/
bool CDatabase::connect(const QString &connection)
{
  CLog::log(LOG_INFO,"DATABASE", "Connection requested", connection);
  if ( m_strDBFile.isEmpty() )
  {
    CLog::log(LOG_ERROR,"DATABASE", "No file to connect was defined", connection);
    return false;
  }

  m_strConnectionName = connection;

  if (QSqlDatabase::contains(m_strConnectionName))
  {
    CLog::log(LOG_DEBUG,"DATABASE", "This connection already exists", m_strConnectionName);
    m_pDB = new QSqlDatabase(QSqlDatabase::database(m_strConnectionName));
    m_pDB->setDatabaseName(m_strDBFile);
    return true;
  }

  bool bCreate = !QFile::exists(m_strDBFile);

  //! Sql Database creation
  m_pDB = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", m_strConnectionName));
  m_pDB->setDatabaseName(m_strDBFile);

  if (bCreate)
  {
    if (open())
    {
      beginTransaction();
      createTables();
      commitTransaction();
      close();
    }
  }

  if (m_pDB != NULL)
    CLog::log(LOG_INFO,"DATABASE", "Connected!", m_strConnectionName);
  else
    CLog::log(LOG_ERROR,"DATABASE", "Connection failed", m_strConnectionName);

  return m_pDB != NULL;
}

bool CDatabase::open()
{
  CLog::log(LOG_DEBUG,"DATABASE", "Opening file: "+m_strDBFile, m_strConnectionName);
  if (m_pDB == NULL) return false;

  if (!m_pDB->open())
  {
    CLog::log(LOG_ERROR,"DATABASE", "Failed to establish connection: "+m_pDB->lastError().text(), m_strConnectionName);
    return false;
  }

  m_pDS = new QSqlQuery(*m_pDB);

  query("PRAGMA synchronous = OFF");
  query("PRAGMA journal_mode = MEMORY");
  query("PRAGMA auto_vacuum = FULL");

  CLog::log(LOG_DEBUG,"DATABASE", "Its open!", m_strConnectionName);
  return true;
}

void CDatabase::close()
{
  CLog::log(LOG_DEBUG,"DATABASE", "Closing", m_strConnectionName);
  m_pDB->close();
}

bool CDatabase::createTables()
{
  bool bResult = true;

  bResult = bResult && query("CREATE TABLE db_attribute (name VARCHAR(255), value TEXT);");

  CLog::log(LOG_DEBUG,"DATABASE", "Inserting revision", m_strConnectionName);

  bResult = bResult && query("INSERT INTO db_attribute (name, value) values ('version', "+m_strDBRevision+");");
  bResult = bResult && query("INSERT INTO db_attribute (name, value) values ('lastUpdate', 0);");
  return bResult;
}

/*******************************************************************************
    database creation
*******************************************************************************/
//const QString CDatabase::vprepare(const QString& strFormat, va_list args) const
//{
//  // Laureon: FIX: Possible bug HERE!!
////  char *p = (char*) malloc(sizeof(char*) * ( strFormat.size() + 1 ));;
////  std::string result = "";

//  qDebug() << "vprepare IN: " << strFormat;

////  if ( p )
////  {
////    result = p;
////    free(p);
////  }

////  QString strResult;
////  strResult.vsprintf(strFormat.toStdString().c_str(), args).toStdString();

////  return strResult;
//}

QString CDatabase::formatValue(const QString &strValue)
{
  if (strValue.isEmpty())
    return "";

  QSqlField data("col",QVariant::String);
  data.setValue(strValue);

  QString strResult = CDatabase::m_pDB->driver()->formatValue(data,true);

  // Laureon: These Deletes are here because formatvalue adds '' to string...
  strResult.remove(0,1);
  strResult.remove(strResult.size()-1,1);

  return strResult;
}

void CDatabase::setFileName(const QString &strFileName)
{
  m_strDBName         = strFileName;
  m_strDBFile         = strFileName;
}

QString CDatabase::prepareSQL(const QString &strQuery, ...)
{
  QString strResult = "";

  const char * string = strQuery.toStdString().c_str();
  va_list args;
  va_start(args, string);
  strResult.vsprintf(strQuery.toStdString().c_str(), args);
  //strResult = vprepare(strQuery, args);
  va_end(args);

  return strResult;
}
