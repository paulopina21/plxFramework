#ifndef HTBACKDROPS_H
#define HTBACKDROPS_H

#include "../Provider.h"

#include <QObject>
#include <QString>
#include <QMap>
#include <QByteArray>

enum Params
{
  ARTIST,
  FANART,
  FANART_THUMB,
  API_KEY
};

class CHTBackdrops  : public iProvider
{
Q_OBJECT
public:
    CHTBackdrops(QObject* parent = 0);

    void set_search(const QString& artist);
    void work(int id);

private slots:
    void requestInfo(int id);
    void parseInfo(QByteArray);
    void receivedInfo(QByteArray);
    void stopSearch();
    const QString GetLegalUrlName(const QString& strFileName);

public slots:
    QString artist() { return m_params[ARTIST].toString(); }
    QString fanartName() { return m_params[FANART].toString(); }
    QString apiKey() { return m_APIKey; }

//signals from FanartProvider
//    void data_received(int, QByteArray);
//    void search_finished(int);


private:
//    QString      m_artist;
//    QString      m_fanart_name;
    QString      m_APIKey;

    QMap<QObject*, int> _listRequests;
};

#endif // HTBACKDROPS_H
