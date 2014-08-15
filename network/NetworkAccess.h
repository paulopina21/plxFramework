#ifndef NETWORKACCESS_H
#define NETWORKACCESS_H

#include <QtNetwork/QtNetwork>
#include <QHash>
#include <QThread>

//#include "constants.h"

//static const QString USER_AGENT = APP_NAME + " " + VERSION;

typedef QPointer<QObject> ObjectPointer;

namespace GLOBAL {
  QNetworkAccessManager* networkAccessManager();
}

class CNetworkReply : public QObject
{
  Q_OBJECT

public:
  CNetworkReply(QNetworkReply* networkReply);
  bool followRedirects;

public slots:
  void finished();
  void requestError(QNetworkReply::NetworkError);

signals:
  void data(QByteArray);
  void error(QNetworkReply*);
  void finished(QNetworkReply*);

private slots:
  void abort();

private:
  QNetworkReply *networkReply;
  QTimer *timer;
};


class CNetworkAccess : public QObject
{
  Q_OBJECT
public:
  static CNetworkAccess* instance();

  CNetworkAccess(QObject* parent=0);
  QNetworkReply* simpleGet(QUrl url, int operation = QNetworkAccessManager::GetOperation);
  CNetworkReply* get(QUrl url);
  CNetworkReply* head(QUrl url);
  QNetworkReply* syncGet(QUrl url);
  QNetworkReply* syncHead(QUrl url);
  QByteArray syncGetBytes(QUrl url);
  QString syncGetString(QUrl url);
  bool syncTest(QUrl url);

private slots:
  void error(QNetworkReply::NetworkError);
  void syncMetaDataChanged();
  void syncFinished();

private:
  QNetworkReply *networkReply;
  bool working;
  QString m_userAgent;
};

static QHash<QThread*, CNetworkAccess *> INSTANCES;

Q_DECLARE_METATYPE(ObjectPointer)

#endif // NETWORKACCESS_H
