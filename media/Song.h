#ifndef SONG_H
#define SONG_H

#include <QObject>

#include <QVariant>
#include <QStringList>

class CSong : public QObject
{
  Q_OBJECT
//  Q_ENUMS(Roles)
public:

  enum Roles
  {
    ID = Qt::UserRole+1,
//    ID_ARTIST,
//    ID_ALBUM,
//    ALBUM_ARTIST,
    ARTIST,
//    ALBUM,
    TRACK,
    TITLE,
    LENGHT,
    YEAR,
    GENRE,
    ISRC,
    LABEL,
    FILEPATH,
    TIMESTAMP,
    PLAY_COUNT,
    IS_BROKEN,
    IS_VIDEO
  };

  CSong(QObject *parent = 0);
  ~CSong();

  virtual QVariant data(int role) const;
  bool setData(int role, const QVariant &value);

  Q_PROPERTY(int idSong READ idSong WRITE setidSong NOTIFY idSongChanged)
  int idSong() { return data(ID).toInt(); }

  Q_PROPERTY(QStringList artist READ artist WRITE setArtist NOTIFY artistChanged)
  QStringList artist() { return data(ARTIST).toStringList(); }

  Q_PROPERTY(int track READ track WRITE setTrack NOTIFY trackChanged)
  int track() { return data(TRACK).toInt(); }

  Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
  QString title() { return data(TITLE).toString(); }

  Q_PROPERTY(QString filepath READ filepath WRITE setFilePath NOTIFY filepathChanged)
  QString filepath() { return data(FILEPATH).toString(); }

  Q_PROPERTY(QString lenght READ lenght WRITE setLenght NOTIFY lenghtChanged)
  QString lenght() { return data(LENGHT).toString(); }

  Q_PROPERTY(int year READ year WRITE setYear NOTIFY yearChanged)
  int year() { return data(YEAR).toInt(); }

  Q_PROPERTY(QString genre READ genre WRITE setGenre NOTIFY genreChanged)
  QString genre() { return data(GENRE).toString(); }

  Q_PROPERTY(QString isrc READ isrc WRITE setIsrc NOTIFY isrcChanged)
  QString isrc() { return data(ISRC).toString(); }

  Q_PROPERTY(QString label READ label WRITE setLabel NOTIFY labelChanged)
  QString label() { return data(LABEL).toString(); }

  Q_PROPERTY(bool broken READ broken WRITE setBroken NOTIFY brokenChanged)
  bool broken() { return data(IS_BROKEN).toBool(); }

  Q_PROPERTY(bool video READ video WRITE setVideo NOTIFY videoChanged)
  bool video() { return data(IS_VIDEO).toBool(); }

protected slots:

public slots:
  void setidSong(const QVariant& value) { setData(ID,value); }
  void setArtist(const QVariant& value) { setData(ARTIST, value); }
  void setTrack(const QVariant& value) { setData(TRACK, value); }
  void setTitle(const QVariant& value) { setData(TITLE, value); }
  void setFilePath(const QVariant& value) { setData(FILEPATH, value); }
  void setLenght(const QVariant& value) { setData(LENGHT, value); }
  void setYear(const QVariant& value) { setData(YEAR, value); }
  void setGenre(const QVariant& value) { setData(GENRE, value); }
  void setIsrc(const QVariant& value) { setData(ISRC, value); }
  void setLabel(const QVariant& value) { setData(LABEL, value); }
  void setBroken(const QVariant& value) { setData(IS_BROKEN, value); }
  void setVideo(const QVariant& value) { setData(IS_VIDEO, value); }

private:
  QMap<int, QVariant> m_fields;


signals:
  void idSongChanged();
  void artistChanged();
  void trackChanged();
  void titleChanged();
  void filepathChanged();
  void lenghtChanged();
  void yearChanged();
  void genreChanged();
  void isrcChanged();
  void labelChanged();
  void timestampChanged();
  void brokenChanged();
  void videoChanged();
  void dataChanged();
};
typedef QList<CSong*> VEC_SONG;

//Q_DECLARE_METATYPE(CSong)

#endif // SONG_H
