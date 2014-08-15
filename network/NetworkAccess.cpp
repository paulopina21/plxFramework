#include "NetworkAccess.h"
#include "../utils/log.h"

//namespace Network {
//  NetworkAccess* http();
//}

/*******************************************************************************
    NetworkReply
*******************************************************************************/
CNetworkReply::CNetworkReply(QNetworkReply *networkReply) : QObject(networkReply)
{
  this->networkReply = networkReply;
  followRedirects = true;

  timer = new QTimer(this);
  timer->setInterval(60000);
  timer->setSingleShot(true);
  connect(timer, SIGNAL(timeout()), SLOT(abort()));
  timer->start();
}

void CNetworkReply::finished()
{
  timer->stop();

  if (networkReply->error() != QNetworkReply::NoError)
  {
    CLog::log(LOG_ERROR,"NETWORKREPLY", "Finished with error: " + networkReply->error(), networkReply->url().toString());
    networkReply->deleteLater();
    return;
  }

  if (followRedirects)
  {
    QUrl redirection = networkReply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
    if (redirection.isValid()) {

      QNetworkReply *redirectReply = CNetworkAccess::instance()->simpleGet(redirection, networkReply->operation());

      setParent(redirectReply);
      networkReply->deleteLater();
      networkReply = redirectReply;

      // when the request is finished we'll invoke the target method
      connect(networkReply, SIGNAL(finished()), this, SLOT(finished()), Qt::QueuedConnection);

      return;
    }
  }

  emit finished(networkReply);

  // get the HTTP response body
  QByteArray bytes = networkReply->readAll();
  emit data(bytes);

  // bye bye my reply
  // this will also delete this NetworkReply as the QNetworkReply is its parent
  networkReply->deleteLater();
}

void CNetworkReply::requestError(QNetworkReply::NetworkError /*code*/)
{
  timer->stop();
  emit error(networkReply);
}

void CNetworkReply::abort()
{
  CLog::log(LOG_ERROR,"NETWORKREPLY", "Timeout ", networkReply->url().toString());
  networkReply->abort();
  emit error(networkReply);
}


/*******************************************************************************
    NetworkAccess
*******************************************************************************/
CNetworkAccess::CNetworkAccess(QObject* parent) : QObject( parent )
{
  m_userAgent = qApp->applicationName() + " " +qApp->applicationVersion();
}

QNetworkReply* CNetworkAccess::simpleGet(QUrl url, int operation)
{
  QNetworkAccessManager *manager = new QNetworkAccessManager(this);

  QNetworkRequest request(url);
  request.setRawHeader("User-Agent", m_userAgent.toUtf8());
  request.setRawHeader("Connection", "Keep-Alive");

  QNetworkReply *networkReply;
  switch (operation) {

    case QNetworkAccessManager::GetOperation:
      CLog::log(LOG_DEBUG,"NETWORKREPLY", "Get: " + url.toEncoded());
      networkReply = manager->get(request);
    break;

    case QNetworkAccessManager::HeadOperation:
      CLog::log(LOG_DEBUG,"NETWORKREPLY", "Head: " + url.toEncoded());
      networkReply = manager->head(request);
    break;

    default:
      CLog::log(LOG_ERROR,"NETWORKREPLY", "Unknown Operation " + QString::number(operation));
    return 0;

  }

  // error handling
  connect(networkReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)), Qt::QueuedConnection);

  return networkReply;

}

CNetworkReply* CNetworkAccess::get(const QUrl url)
{
  QNetworkReply *networkReply = simpleGet(url);
  CNetworkReply *reply = new CNetworkReply(networkReply);

  // error signal
  connect(networkReply, SIGNAL(error(QNetworkReply::NetworkError)),
          reply, SLOT(requestError(QNetworkReply::NetworkError)), Qt::QueuedConnection);

  // when the request is finished we'll invoke the target method
  connect(networkReply, SIGNAL(finished()), reply, SLOT(finished()), Qt::QueuedConnection);

  return reply;

}

CNetworkReply* CNetworkAccess::head(const QUrl url)
{
  QNetworkReply *networkReply = simpleGet(url, QNetworkAccessManager::HeadOperation);
  CNetworkReply *reply = new CNetworkReply(networkReply);
  reply->followRedirects = false;

  // error signal
  connect(networkReply, SIGNAL(error(QNetworkReply::NetworkError)),reply, SLOT(requestError(QNetworkReply::NetworkError)), Qt::QueuedConnection);

  // when the request is finished we'll invoke the target method
  connect(networkReply, SIGNAL(finished()), reply, SLOT(finished()), Qt::QueuedConnection);

  return reply;

}

QNetworkReply* CNetworkAccess::syncGet(QUrl url)
{
  working = true;

  networkReply = simpleGet(url);
  connect(networkReply, SIGNAL(metaDataChanged()),this, SLOT(syncMetaDataChanged()), Qt::QueuedConnection);
  connect(networkReply, SIGNAL(finished()),this, SLOT(syncFinished()), Qt::QueuedConnection);
  connect(networkReply, SIGNAL(error(QNetworkReply::NetworkError)),this, SLOT(error(QNetworkReply::NetworkError)), Qt::QueuedConnection);

  // A little trick to make this function blocking
  while (working) {
    // Do something else, maybe even network processing events
    qApp->processEvents();
  }

  networkReply->deleteLater();
  return networkReply;

}

QNetworkReply* CNetworkAccess::syncHead(QUrl url)
{
  working = true;

  networkReply = simpleGet(url, QNetworkAccessManager::HeadOperation);
  connect(networkReply, SIGNAL(finished()),this, SLOT(syncFinished()), Qt::QueuedConnection);
  connect(networkReply, SIGNAL(error(QNetworkReply::NetworkError)),this, SLOT(error(QNetworkReply::NetworkError)), Qt::QueuedConnection);

  // A little trick to make this function blocking
  while (working) {
    // Do something else, maybe even network processing events
    qApp->processEvents();
  }

  networkReply->deleteLater();
  return networkReply;

}

void CNetworkAccess::syncMetaDataChanged()
{
  QUrl redirection = networkReply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
  if (redirection.isValid()) {
    CLog::log(LOG_INFO,"NetworkAccess", "Redirection to " + redirection.toString());
    networkReply->deleteLater();
    syncGet(redirection);

  }

}

void CNetworkAccess::syncFinished()
{
  // got it!
  working = false;
}

void CNetworkAccess::error(QNetworkReply::NetworkError code)
{
  //! get the QNetworkReply that sent the signal
  QNetworkReply *networkReply = static_cast<QNetworkReply *>(sender());
  if (!networkReply)
  {
    CLog::log(LOG_ERROR,"NetworkAccess", "There is an error but I don't know the sender. Code: " + code);
    return;
  }
  CLog::log(LOG_ERROR,"NetworkAccess", networkReply->errorString() + code);
//  qDebug() << "NetworkAccess -> ERROR: " <<  networkReply->errorString() << code;

  networkReply->deleteLater();
}

QByteArray CNetworkAccess::syncGetBytes(QUrl url)
{
  return syncGet(url)->readAll();
}

QString CNetworkAccess::syncGetString(QUrl url)
{
  return QString::fromUtf8(syncGetBytes(url));
}

bool CNetworkAccess::syncTest(QUrl url)
{
  return (syncHead(url)->error() == QNetworkReply::NoError);
}


CNetworkAccess *CNetworkAccess::instance()
{
  // const QString threadName = QThread::currentThread()->objectName();
  if (INSTANCES.contains(QThread::currentThread()))
  {
    return INSTANCES.value(QThread::currentThread());
  }
  else
  {
    // qDebug() << "NetworkAccess for thread" << QThread::currentThread();
    CNetworkAccess *http = new CNetworkAccess();
    INSTANCES.insert(QThread::currentThread(), http);
    return http;
  }
}
