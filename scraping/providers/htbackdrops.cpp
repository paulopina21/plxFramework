#include "htbackdrops.h"

#include "../../media/mediaitem.h"
#include "../../network/NetworkAccess.h"
//#include "utilities.h"

#include <QtNetwork/QtNetwork>

namespace GLOBAL {
  CNetworkAccess* http();
}

/*******************************************************************************
  FanartProvider_HTBackDrops
*******************************************************************************/
CHTBackdrops::CHTBackdrops(QObject* parent): iProvider(parent)
{
  setName("htbackdrops");
  m_APIKey = "7681a907c805e0670330c694e788e8e8";
}

//! ----  set_search -----------------------------------------------------------
void CHTBackdrops::set_search(const QString& artist)
{
  qDebug() << "Provider: HTBackDrops -> set_search  _listRequests" << _listRequests;

  m_params[ARTIST]  = GetLegalUrlName(artist);
  m_params[FANART]  = CMediaItem::buildImageName(artist,"");
}

//! ----  start_search ---------------------------------------------------------
void CHTBackdrops::work(int id)
{
  qDebug() << "Provider: HTBackDrops -> Starting search";
  if (m_params[ARTIST].toString().isEmpty())
  {
    emit finished(id);
  }
  else
  {
    requestInfo(id);
  }
}


//! ----  slot_send_fanart_info_request -----------------------------------------
void CHTBackdrops::requestInfo(int id)
{
  qDebug() << "Provider: HTBackdrops -> Requesting info for " << m_params[ARTIST].toString();

  QUrlQuery url("http://htbackdrops.com/api/"+apiKey()+"/searchXML/?");
  url.addQueryItem("default_operator", "and");
  url.addQueryItem("keywords", artist());

  QObject *reply = CNetworkAccess::instance()->get(url.query());
  _listRequests[reply] = id;

  connect(reply, SIGNAL(data(QByteArray)), this, SLOT(parseInfo(QByteArray)));
  connect(reply, SIGNAL(error(QNetworkReply*)), this, SLOT(stopSearch()));
}

//! ----  slot_parse_fanart_info ------------------------------------------------
void CHTBackdrops::parseInfo(QByteArray bytes)
{
  qDebug() << "Provider: HTBackDrops -> Parsing info";

  /*-------------------------------------------------*/
  /* Get id from sender reply                        */
  /* ------------------------------------------------*/
  QObject* reply = qobject_cast<QObject*>(sender());

  if (!reply || !_listRequests.contains(reply))   return;
  const int id = _listRequests.take(reply);

  /*-------------------------------------------------*/
  /* Parse info                                      */
  /* ------------------------------------------------*/
  QXmlStreamReader xml(bytes);
  bool imageFound = false;

  //! Xml reading loop
  while(!xml.atEnd() && !xml.hasError())
  {
    QXmlStreamReader::TokenType token = xml.readNext();
    //    qDebug() << token;
    //! search Xml start element

    if(token == QXmlStreamReader::StartElement)
    {
      //! search Xml Image element (size = medium, large, extralarge)
      if(xml.name() == "id")
      {
        //! we have to check if image is already stored
        bool imageAlreadyPresent = false;

        // Laureon: Removed image existance check this isn`t supposed to be here...
        // image has to be stored
        if (!imageAlreadyPresent) {
          QString imageUrl = xml.readElementText();
          if (!imageUrl.isEmpty())
          {
            QUrl url = "http://htbackdrops.com/api/"+apiKey()+"/download/"+imageUrl.toUtf8()+"/fullsize";
            QObject *reply = CNetworkAccess::instance()->get(url);
            _listRequests[reply] = id;

            connect(reply, SIGNAL(data(QByteArray)), this, SLOT(receivedInfo(QByteArray)));
            connect(reply, SIGNAL(error(QNetworkReply*)), this, SLOT(stopSearch()));
            imageFound = true;
            break;
          }
        }
      } // end Xml image element
    } // end Xml start element
  } // end Xml reading loop

  // Error handling.
  if(xml.hasError()) {
    qDebug() << "Provider: HTBackDrops -> Network error!!";
    emit finished(id);
  }

  if(!imageFound) {
    qDebug() << "Provider: HTBackDrops -> No fanart found!!";
    emit finished(id);
  }
}


//! --  slot_fanart_received ----------------------------------------------------
void CHTBackdrops::receivedInfo(QByteArray bytes)
{
  // Get id from sender reply
  QObject* reply = qobject_cast<QObject*>(sender());
  if (!reply || !_listRequests.contains(reply))   return;

  const int id = _listRequests.take(reply);

  // Send signal for data reception
  emit dataReceived(id, bytes);
}



//! --  slot_fanart_search_stop -------------------------------------------------
void CHTBackdrops::stopSearch()
{
  qDebug() << "Provider: HTBackdrops -> Stop search";

  // Get id from sender reply
  QObject* reply = qobject_cast<QObject*>(sender());
  if (!reply || !_listRequests.contains(reply))   return;

  const int id = _listRequests.take(reply);

  // Send signal for data reception
  emit finished(id);
}

const QString CHTBackdrops::GetLegalUrlName(const QString& strFileName) {
  char illegalChars[] = {'*', '/', '<', '>', '?', '\\', '[', ']', '\'', ' ' };
  QString strLegalFileName = strFileName;
//  QString legalfile;
//  legalfile.contains();

  for (unsigned int c = 0; c < sizeof(illegalChars); c++)
  {
    for (int i = 0; i < strLegalFileName.size(); i++)
    {
      if (strLegalFileName.at(i) == illegalChars[c])
      {
        strLegalFileName.replace(i,1,'+');
      }
//      else
//        if (strLegalFileName[i] == ' ') {
//          strLegalFileName.replace(i,1,'+');
//        }
    }
  }

  qDebug() << strFileName + " => " + strLegalFileName;
  return strLegalFileName;
}
