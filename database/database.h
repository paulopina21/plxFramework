#ifndef _DATABASE_H_
#define _DATABASE_H_

#include <QObject>
#include <QString>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlQuery>

#include <stdio.h>
#include <string.h>

/*******************************************************************************
    Database
*******************************************************************************/
class CDatabase: public QObject
{
  Q_OBJECT
public:
  explicit CDatabase(QObject* parent = 0);
  ~CDatabase();

  virtual bool beginTransaction();
  virtual bool commitTransaction();
  virtual bool rollbackTransaction();

  QSqlDatabase* db() { return m_pDB; }
  QSqlQuery* query() { return m_pDS; }

  bool query(const QString& strQuery, ...);

  virtual bool connect(const QString& connection);
  virtual bool open();
  virtual void close();

  inline bool isOpen() const { return m_pDB->isOpen(); }

  QString name() {return m_strDBName;}

  virtual bool createTables();
  virtual void createViews() = 0;

  static QString prepareSQL(const QString &strQuery, ...);
  static QString formatValue(const QString &strValue);

  QSqlQuery*       m_pDS;

protected:
  void setFileName(const QString &strFileName);
  static QSqlDatabase*    m_pDB;

private:
  QString          m_strDBName;
  QString          m_strDBFile;
  QString          m_strDBRevision;
  QString          m_strConnectionName;
};

#endif // _DATABASE_H_
