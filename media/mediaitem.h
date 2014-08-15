#ifndef MEDIAITEM_H
#define MEDIAITEM_H

#include <QObject>
#include <QFileInfo>

#include "Artist.h"
#include "Album.h"
#include "Song.h"
#include "../models/ListItem.h"

class CMediaItem : public ListItem, public QFileInfo
{
  Q_OBJECT
  Q_ENUMS(mediaType)
public:

  enum Roles
  {
    TypeRole = Qt::UserRole+1,
    ArtistRole,
    AlbumRole,
    SongRole,
    FilePathRole,
    TimestampRole
  };

  enum mediaType
  {
    FILE = 0,
    GENERIC,
    SONG,
    ALBUM,
    ARTIST
  };

  ~CMediaItem();
  CMediaItem(int type = 0, QObject *parent = 0);
  CMediaItem(const CMediaItem &other);
  CMediaItem(CMediaItem *other);

  virtual int id() const {return -1;}

  CMediaItem& operator = (CMediaItem &other);
//  CMediaItem* operator = (CMediaItem *other);

  QVariant data(int role) const;
  QHash<int, QByteArray> roleNames() const;

//  Q_PROPERTY(int type READ type WRITE setType NOTIFY typeChanged)
  Q_INVOKABLE int type() const { return m_type; }

//  Q_PROPERTY(CArtist* artist READ artist WRITE setArtist NOTIFY artistChanged)
  Q_INVOKABLE inline CArtist* artist() const { return m_artistInfo; }
  Q_INVOKABLE CArtist* getArtist();

//  Q_PROPERTY(CAlbum* album READ album WRITE setAlbum NOTIFY albumChanged)
  Q_INVOKABLE inline CAlbum* album() const { return m_albumInfo; }
  Q_INVOKABLE CAlbum* getAlbum();

//  Q_PROPERTY(CSong* song READ song WRITE setSong NOTIFY songChanged)
  Q_INVOKABLE inline CSong* song() const { return m_songInfo; }
  Q_INVOKABLE CSong* getSong();

//  Q_PROPERTY(QString filePath READ filePath WRITE setFilePath NOTIFY fileChanged)
//  Q_INVOKABLE inline QString filePath() const { return m_filepath; }
  Q_INVOKABLE inline QString filePath() const { return absoluteFilePath(); }
//  Q_PROPERTY(int timestamp READ timestamp WRITE setTimestamp NOTIFY timestampChanged)
  Q_INVOKABLE inline uint timestamp() const { return m_timestamp; }

signals:
  void typeChanged(int type);
  void artistChanged(CArtist* artist);
  void albumChanged(CAlbum* album);
  void songChanged(CSong* song);
  void timestampChanged();
  void fileChanged();

public slots:
  void setType(int type) { m_type = type; emit typeChanged(m_type); emit dataChanged(); }
  void setArtist(CArtist* artist);
  void setAlbum(CAlbum* album);
  void setSong(CSong* song);
  void setTimestamp(uint value) { m_timestamp = value; }
//  void setFilePath(const QString& strFilePath) { m_filepath = strFilePath; setFile(strFilePath); emit fileChanged(); }

  bool loadFromFile(const QString& strFileName);

  bool isAudio();
  bool isVideo();

  static bool isAudio(const QString& strFileName);
  static bool isVideo(const QString& strFileName);

  static QString buildImageName(const QString& strArtist, const QString& strAlbum = "", bool bThumb = false);
  static QString findCoverName(const QString& strFileName);
  static QString lenghtFromInt(const int lenght);

//  bool isImage();

protected:
  bool loadTags(const QString& strFileName);

private:
  int       m_type;
  uint      m_timestamp;
  QString   m_filepath;
  CArtist*  m_artistInfo;
  CAlbum*   m_albumInfo;
  CSong*    m_songInfo;
};

typedef QList<CMediaItem*> VEC_MEDIAITEM;

Q_DECLARE_METATYPE(CMediaItem)

#endif // MEDIAITEM_H
