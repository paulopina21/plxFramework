#ifndef ALBUM_H
#define ALBUM_H

#include <QObject>
#include <QtQml>

#include "Song.h"
#include "Artist.h"

#include "../utils/image.h"

class CAlbum: public QObject
{
  Q_OBJECT
  Q_ENUMS(Fields)
public:
  enum Fields
  {
    ID,
    ID_ARTIST,
    ARTIST,
    TITLE,
    GENRE,
    YEAR,
    LABEL,
    PFC_FILE,

    ID_COVER,

    SONGS_COUNT,
    PLAY_COUNT,
    RATING
  };


  CAlbum(QObject *parent = 0);

  Q_PROPERTY(int idAlbum READ idAlbum WRITE setIdAlbum NOTIFY idAlbumChanged)
  int idAlbum() { return  m_fields[ID].toInt(); }

  Q_PROPERTY(int idArtist READ idArtist WRITE setIdArtist NOTIFY idArtistChanged)
  int idArtist() { return m_fields[ID_ARTIST].toInt(); }

  Q_PROPERTY(CArtist* artist READ artist WRITE setArtist NOTIFY artistChanged)
  CArtist* artist() { return m_artist; }

  Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
  QString title() { return m_fields[TITLE].toString(); }

  Q_PROPERTY(QString genre READ genre WRITE setGenre NOTIFY genreChanged)
  QString genre() { return m_fields[GENRE].toString(); }

  Q_PROPERTY(int year READ year WRITE setYear NOTIFY yearChanged)
  int year() { return m_fields[YEAR].toInt(); }

  Q_PROPERTY(QString label READ label WRITE setLabel NOTIFY labelChanged)
  QString label() { return m_fields[LABEL].toString(); }

  Q_PROPERTY(CImage* cover READ cover WRITE setCover NOTIFY coverChanged)
  CImage* cover() { return m_cover; }

  Q_PROPERTY(QQmlListProperty<CSong> songs READ songs CONSTANT)
  QQmlListProperty<CSong> songs() { return QQmlListProperty<CSong>(this, m_songs); }

public slots:
  void setIdAlbum(QVariant value) { setField(ID, value); }
  void setIdArtist(QVariant value) { setField(ID_ARTIST, value); }
  void setArtist(CArtist* value);
  void setTitle(QVariant value) { setField(TITLE, value); }
  void setGenre(QVariant value) { setField(GENRE, value); }
  void setYear(QVariant value) { setField(YEAR, value); }
  void setLabel(QVariant value) { setField(LABEL, value); }

  void setCover(CImage* cover);

  int addSong(CSong* song);  
  void setSongs(QList<CSong *> songsList);

  int countSongs();
  CSong* getSong(int i);

signals:
  void idAlbumChanged();
  void idArtistChanged();
  void artistChanged();
  void titleChanged();
  void genreChanged();
  void yearChanged();
  void labelChanged();
  void coverChanged();
  void songsChanged();

private:
  QMap<int, QVariant> m_fields;
  QList<CSong*> m_songs;

  CArtist* m_artist;
  CImage* m_cover;

  QVariant field(Fields field) { return m_fields[field]; }
  void setField(Fields field, const QVariant &value);
};

typedef QList<CAlbum*> VEC_ALBUM;

//QML_DECLARE_TYPE(CAlbum)

#endif // ALBUM_H
