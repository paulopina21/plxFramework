#ifndef ARTIST_H
#define ARTIST_H

#include <QObject>
//#include <QQmlContext>
//#include <QQmlListProperty>
//#include <QDeclarativeItem>
//#include "Album.h"

//#include "../model/listmodel.h"

#include "../utils/image.h"

class CArtist: public QObject
{
  Q_OBJECT
//  Q_ENUMS(Fields)
public:

  enum Roles
  {
    ID = Qt::UserRole+1,
    NAME,
    FANART,
    THUMB,
    ALBUMS,
    PLAY_COUNT,
    RATING
  };

  CArtist(QObject *parent = 0);

  CArtist & operator=(CArtist &newItem);

//  QVariant data(int role) const;
  QHash<int, QByteArray> roleNames() const;

  Q_PROPERTY(int idArtist READ idArtist WRITE setidArtist NOTIFY idArtistChanged)
  int idArtist() { return data(ID).toInt(); }

  Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
  QString name() { return data(NAME).toString(); }

  Q_PROPERTY(CImage* fanart READ fanart WRITE setFanart NOTIFY fanartChanged)
  CImage* fanart() const { return m_fanart; }

//  Q_PROPERTY(QQmlListProperty<CAlbum> albums READ albums CONSTANT)
//  QQmlListProperty<CAlbum> albums() { return QQmlListProperty<CAlbum>(this, m_albums); }

protected slots:
//  QVariant field(Roles field) const { return m_fields[field]; }
//  void setField(Roles field, const QVariant &value);

public slots:
  void setidArtist(QVariant value) { setData(ID, value); }
  void setName(QVariant value) { setData(NAME, value); }
  void setFanart(CImage* fanart) { m_fanart = fanart; emit fanartChanged();}

//  int addAlbum(CAlbum* album);
//  void setAlbums(QList<CAlbum *> albumsList);
//  QList<CAlbum*> getAlbums() { return m_albums; }

//  CAlbum* getAlbum(int index) { return m_albums[index]; }


signals:
  void idArtistChanged();
  void nameChanged();
  void fanartChanged();
  void albumsChanged();

private:
  QMap<int, QVariant> m_fields;

  QString m_strArtist;
  CImage* m_fanart;

  QVariant data(Roles field) { return m_fields[field]; }
  void setData(Roles field, const QVariant &value);


//  QList<CAlbum *> m_albums;
//  CAlbum *album(int index) const { return (index >= 0 && index < m_albums.count()) ? m_albums.at(index) : 0; }
};

typedef QList<CArtist*> VEC_ARTIST;

#endif // ARTIST_H
