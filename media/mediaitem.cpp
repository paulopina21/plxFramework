#include "mediaitem.h"

#ifndef Q_OS_ANDROID
//#include <taglib/mpegfile.h>
//#include <taglib/fileref.h>
//#include <taglib/tstring.h>
//#include <taglib/id3v2tag.h>
//#include <taglib/attachedpictureframe.h>
//#include <taglib/vorbisfile.h>
//#include <taglib/oggfile.h>
//#include <taglib/oggflacfile.h>
//#include <taglib/flacfile.h>
#endif

#include "../utils/log.h"

const QStringList audioFilter   = QStringList() << "mp3" << "ogg" << "flac" << "wav" << "m4a" << "aac";
const QStringList videoFilter   = QStringList() << "mpg" << "avi" << "mp4" << "m2ts" << "mkv" << "ts" << "mpeg";
const QStringList coverFilter   = QStringList() << "album.jpg" << "album.png" << "cover.png" << "cover.jpg" << "capa.jpg" << "capa.png" << "folder.jpg" << "thumb.jpg";

CMediaItem::CMediaItem(int type, QObject *parent): ListItem(parent)
{
  qmlRegisterType<CArtist>("music", 1, 0, "Artist");
  qmlRegisterType<CAlbum>("music", 1, 0, "Album");
  qmlRegisterType<CSong>("music", 1, 0, "Song");

  qmlRegisterType<CMediaItem>("media", 1, 0, "MediaItem");
  qRegisterMetaType<CMediaItem>();

  m_artistInfo = new CArtist(this);
  m_albumInfo = new CAlbum(this);
  m_songInfo = new CSong(this);

  setType(type);
}

CMediaItem::CMediaItem(const CMediaItem &other): ListItem(other.parent()), QFileInfo(other.filePath()),
  m_type(other.type()), m_timestamp(other.timestamp()), m_filepath(other.m_filepath),
  m_artistInfo(other.artist()), m_albumInfo(other.album()), m_songInfo(other.song())
{
  emit dataChanged();
}

CMediaItem::CMediaItem(CMediaItem *other): ListItem(other->parent()), QFileInfo(other->filePath()),
  m_type(other->type()), m_timestamp(other->timestamp()), m_filepath(other->m_filepath),
  m_artistInfo(other->artist()), m_albumInfo(other->album()), m_songInfo(other->song())
{
  emit dataChanged();
}

CMediaItem &CMediaItem::operator =(CMediaItem &other)
{
  if (this == &other)      // Same object?
     return *this;        // Yes, so skip assignment, and just return *this.
//  setFilePath( other.filePath() );
  setFile( other.filePath() );
  setType( other.type() );
  setTimestamp( other.timestamp() );

  setArtist( other.artist() );
  setAlbum( other.album() );
  setSong( other.song() );

  return *this;
}

//CMediaItem *CMediaItem::operator =(CMediaItem *other)
//{
//  if (this == other)      // Same object?
//     return this;        // Yes, so skip assignment, and just return *this.

//  setFile( other->filePath() );
//  setType( other->type() );
//  setTimestamp( other->timestamp() );

//  setArtist( other->artist() );
//  setAlbum( other->album() );
//  setSong( other->song() );

//  return this;
//}

QVariant CMediaItem::data(int role) const
{
  switch(role)
  {
    case TypeRole:
      return QVariant::fromValue(type());
    break;

    case ArtistRole:
      return QVariant::fromValue(artist());
    break;

    case AlbumRole:
      return QVariant::fromValue(album());
    break;

    case SongRole:
      return QVariant::fromValue(song());
    break;

    case FilePathRole:
      return QVariant::fromValue(filePath());
    break;

    case TimestampRole:
      return QVariant::fromValue(timestamp());
    break;

    default:
    return QVariant();
  }
}

QHash<int, QByteArray> CMediaItem::roleNames() const
{
  QHash<int, QByteArray> names;
  names[TypeRole]       = "type";
  names[ArtistRole]     = "artist";
  names[AlbumRole]      = "album";
  names[SongRole]       = "song";
  names[FilePathRole]   = "filePath";
  names[TimestampRole]  = "timeStamp";
  return names;
}

CArtist *CMediaItem::getArtist()
{
//  if (!m_artistInfo)
//    m_artistInfo = new CArtist(this);

  return m_artistInfo;
}

CAlbum *CMediaItem::getAlbum()
{
//  if (!m_albumInfo)
//    m_albumInfo = new CAlbum(this);

  return m_albumInfo;
}

CSong *CMediaItem::getSong()
{
//  if (!m_songInfo)
//    m_songInfo = new CSong(this);

  return m_songInfo;
}

void CMediaItem::setArtist(CArtist *artist)
{
  if (m_artistInfo)
    delete m_artistInfo;

  m_artistInfo = artist;
  emit artistChanged(m_artistInfo);
  emit dataChanged();
}

void CMediaItem::setAlbum(CAlbum *album)
{
  if (m_albumInfo)
    delete m_albumInfo;

  m_albumInfo = album;
  emit albumChanged(m_albumInfo);
  emit dataChanged();
}

void CMediaItem::setSong(CSong *song)
{
  if (m_songInfo)
    delete m_songInfo;

  m_songInfo = song;
  emit songChanged(m_songInfo);
  emit dataChanged();
}

bool CMediaItem::loadFromFile(const QString &strFileName)
{
  CLog::log(LOG_INFO, "MEDIAITEM", "Loading from file: " + strFileName);

  this->setFile(strFileName);
  setTimestamp(lastModified().toTime_t());

//  m_songInfo->setFilePath(absoluteFilePath().toUtf8());
//  m_songInfo->setTitle(baseName());
  loadTags(strFileName);

  //! FilePath & FileName complementing
  QDir songDir(absoluteFilePath().toUtf8());
  songDir.cdUp();
  int pos;

  QString song_fileDir  = songDir.dirName();
  QRegExp rxFileDir("([\\w\\s!?@#$%&*'(){}\\[\\]”:;.,+]{1,})([-–\\s]*)([\\w\\s!?@#$%&*'(){}\\[\\]- ”:;.,+]{0,})"); //([\w\s!?@#$%&*'(){}\[\]”:;.,]{1,})([-\s]*)([\w\s!?@#$%&*'(){}\[\]- ”:;.,]{0,})
  rxFileDir.setCaseSensitivity(Qt::CaseInsensitive);
  rxFileDir.setPatternSyntax(QRegExp::RegExp);
  pos = rxFileDir.indexIn(song_fileDir);
  qDebug() << "INFO: RegExp -> "<< pos << " found in " << song_fileDir;
  QStringList song_fileArtistAlbum;
  song_fileArtistAlbum.append(rxFileDir.cap(1));
  song_fileArtistAlbum.append(rxFileDir.cap(3));//Cap(2) is only the "-" if applyed, so we need cap(1) and cap(3)
  qDebug() << "INFO: RegExp -> Artist, Album: " << song_fileArtistAlbum;

  QString song_fileName = completeBaseName();
//  QRegExp rxFileName("(^[\\d]*)\\D*([\\w][\\w\\s!?@#$%&*'(){}- ”:;.,+]{1,})(?:\\.)([^ :\\\\/.]*)"); // REGEXP WITH SUFIX (TRACK - TITLE - SUFIX) // (^[\d]*)\D*([\w][\w\s!?@#$%&*'(){}- ”:;.,]{1,})(?:\.)([^ :\\/.]*)
  QRegExp rxFileName("(^[\\d]*)\\D*([\\w][\\w\\s!?@#$%&*'(){}- ”:;.,+]{1,})"); // REGEXP WITHOUT SUFIX (TRACK - TITLE) // (^[\d]*)\D*([\w][\w\s!?@#$%&*'(){}- ”:;.,]{1,})
  rxFileName.setCaseSensitivity(Qt::CaseInsensitive);
  rxFileName.setPatternSyntax(QRegExp::RegExp);  
  rxFileName.indexIn(song_fileName);
  qDebug() << "INFO: RegExp -> "<< pos << " found in " << song_fileName;
  QStringList song_fileTrackName;
  song_fileTrackName.append(rxFileName.cap(1));
  song_fileTrackName.append(rxFileName.cap(2));
  song_fileTrackName.append(rxFileName.cap(3));
  qDebug() << "INFO: RegExp -> Track, Title, Sufix: " << song_fileTrackName;

  if ((m_songInfo->track() == 0) && song_fileTrackName.count() > 0)
  {
    m_songInfo->setTrack(song_fileTrackName[0].trimmed().toInt());
    CLog::log(LOG_DEBUG, "MEDIAITEM", "Extracted Track: " + song_fileTrackName[0].trimmed());
  }

  if (m_songInfo->title().isEmpty() && song_fileTrackName.count() > 0)
  {
    m_songInfo->setTitle(song_fileTrackName[1].trimmed());
    CLog::log(LOG_DEBUG, "MEDIAITEM", "Extracted Title: " + song_fileTrackName[1].trimmed());
  }

  if (isVideo())
  {
    m_songInfo->setVideo(true);
    CLog::log(LOG_DEBUG, "MEDIAITEM", "Marked as Video: " + song_fileTrackName[1].trimmed());
  }else{
    m_songInfo->setVideo(false);
    CLog::log(LOG_DEBUG, "MEDIAITEM", "Marked as Audio: " + song_fileTrackName[1].trimmed());
  }

  if (m_artistInfo->name().isEmpty() && song_fileArtistAlbum.count() > 0)
  {
    m_artistInfo->setName(song_fileArtistAlbum[0].trimmed());
    CLog::log(LOG_DEBUG, "MEDIAITEM", "Extracted Artist: " + song_fileArtistAlbum[0].trimmed());
  }

  if (m_albumInfo->title().isEmpty() && song_fileArtistAlbum.count() > 1)
  {
    m_albumInfo->setTitle(song_fileArtistAlbum[1].trimmed());
    CLog::log(LOG_DEBUG, "MEDIAITEM", "Extracted Album: " + song_fileArtistAlbum[1].trimmed());
  }

  //! END: FilePath & FileName complementing

  //!DEFAULT values if empty tag
  //  if (song->field(CSong::ARTIST).toString().isEmpty()) {
  //    QDir dir = QFileInfo(strFilePath).absoluteDir();
  //    CStdString strFolder = dir.dirName().toStdString();
  //    CStdStringArray strResult;
  //    StringUtils::SplitString(strFolder,"-",strResult,0);

  //    if (strResult.size() > 0)
  //      song->setField(CSong::ARTIST, strResult[0].Trim().c_str());
  //    else
  //      song->setField(CSong::ARTIST, QObject::tr("unknown artist"));
  //  }
  // Laureon: TODO: implement this shit...

  //  if (song->field(CSong::ALBUM).toString().isEmpty()) {
  //    QDir dir = QFileInfo(strFilePath).absoluteDir();
  //    CStdString strFolder = dir.dirName().toStdString();
  //    CStdStringArray strResult;
  //    StringUtils::SplitString(strFolder,"-",strResult,0);

  //    if (strResult.size() > 1)
  //      song->setField(CSong::ALBUM, strResult[1].Trim().c_str());
  //    else
  //      song->setField(CSong::ALBUM, QObject::tr("unknown album"));
  //  }
  // Laureon: TODO: this one either

  if (m_songInfo->title().isEmpty())
    m_songInfo->setTitle(QObject::tr("unknown title"));

  if (m_albumInfo->title().isEmpty())
    m_albumInfo->setTitle(QObject::tr("unknown album"));

  if (m_songInfo->artist().isEmpty())
    m_songInfo->setArtist(QObject::tr("unknown artist"));

  if (m_artistInfo->name().isEmpty())
    m_artistInfo->setName(QObject::tr("unknown album artist"));

  if (m_songInfo->genre().isEmpty())
    m_songInfo->setGenre(QObject::tr("unknown genre"));

  if (m_songInfo->label().isEmpty())
    m_songInfo->setLabel(QObject::tr("unknown label"));

  if (m_songInfo->year() == 0)
    m_songInfo->setYear(1900);

  //  if (song->field(CSong::TITLE).toString().isEmpty()) {
  //    CStdString strName = song->field(CSong::TITLE).toString().toStdString();
  //    CStdStringArray strResult;
  //    StringUtils::SplitString(strName,"-",strResult,0);
  //    if (strResult.size() > 1)
  //      song->setField(CSong::TITLE, strResult[1].Trim().c_str());
  //    else
  //      song->setField(CSong::TITLE, strName.Trim().c_str());
  //  }
  // Laureon: TODO: and this one...
  //  if (song->field(CSong::LENGHT).toInt() == 0) {
  //    CStdString strName = song->field(CSong::TITLE).toString().toStdString();
  //    CStdStringArray strResult;
  //    StringUtils::SplitString(strName,"-",strResult,0);
  //    if (strResult.size() > 0)
  //      song->setField(CSong::LENGHT, strResult[0].Trim().c_str());
  //  }
  // Laureon: TODO: .... and you know what...

  //! replaygain metadata
  //    MEDIA::ReplayGainTagMap map = MEDIA::readReplayGainTags( fileref );
  //    if ( map.contains( MEDIA::ReplayGain_Track_Gain ) )
  //      song->setField(TRACK::trackGain] = map[MEDIA::ReplayGain_Track_Gain];
  //    if ( map.contains( MEDIA::ReplayGain_Track_Peak ) )
  //      song->setField(TRACK::trackPeak] = map[MEDIA::ReplayGain_Track_Peak];
  //    if ( map.contains( MEDIA::ReplayGain_Album_Gain ) )
  //      song->setField(TRACK::albumGain] = map[MEDIA::ReplayGain_Album_Gain];
  //    else
  //      song->setField(TRACK::albumGain] = song->setField(TRACK::trackGain];
  //    if ( map.contains( MEDIA::ReplayGain_Album_Peak ) )
  //      song->setField(TRACK::albumPeak] = map[MEDIA::ReplayGain_Album_Peak];
  //    else
  //      song->setField(TRACK::albumPeak] = song->setField(TRACK::trackPeak];

  //    //! statistics metadata (playcount + rating)
  //    MEDIA::StatisticTagMap stat_map = MEDIA::readStatisticTags( fileref );
  //    if ( stat_map.contains( MEDIA::Rating_Tag ) )
  //      song->setField(TRACK::rating]    = stat_map[MEDIA::Rating_Tag];
  //    if ( stat_map.contains( MEDIA::Playcount_Tag ) )
  //      song->setField(TRACK::playcount] = stat_map[MEDIA::Playcount_Tag];

  //! default value
  //    song->setField(SONG::isPlaying]    =  false;
  //    song->setField(TRACK::lastPlayed]   =  -1;
  //    song->setField(TRACK::isPlayed]     =  false;
  //    song->setField(TRACK::isStopAfter]  =  false;
  //    song->setField(TRACK::playcount]    =  0;
  //    song->setField(TRACK::rating]       =  0;
  return true;
}

bool CMediaItem::loadTags(const QString &strFileName)
{
#ifndef Q_OS_ANDROID
//PAULOPINA - REMOVER TAGLIB
//  //! encodedName shall be valid as long as fileName exists
//  CLog::log(LOG_INFO, "MEDIAITEM", "Loading tags for: " + strFileName);

//  QByteArray fileName = QFile::encodeName(strFileName);
//  const char * encodedName = fileName.constData();

//  //! TagLib reference
//  CLog::log(LOG_DEBUG, "MEDIAITEM", "Loading...", "Taglib");
//  TagLib::FileRef fileref = TagLib::FileRef(encodedName, true);

//  if (fileref.isNull())
//  {
//    CLog::log(LOG_ERROR, "MEDIAITEM", "Tag access failed. Marked as broken " + strFileName, "Taglib");
//    m_songInfo->setBroken(true);
//    return false;
//  }
//  else
//  {
//    CLog::log(LOG_DEBUG, "MEDIAITEM", "Ready to go...", "Taglib");
//  }

//  //! Lenght reading
//  CLog::log(LOG_DEBUG, "MEDIAITEM", "Reading file length", "Taglib");
//  TagLib::AudioProperties *audioProperties = fileref.audioProperties();
//  if (audioProperties)
//  {
//    m_songInfo->setLenght(lenghtFromInt(audioProperties->length())); // Returns the length of the file in format mm:ss
//    CLog::log(LOG_DEBUG, "MEDIAITEM", "File length: " + audioProperties->length(), "Taglib");
//  }
//  else
//  {
//    CLog::log(LOG_ERROR, "MEDIAITEM", "File length read failed. Marked as broken", "Taglib");
//    m_songInfo->setLenght("");
//    m_songInfo->setBroken(true);
//    return false;
//  }
//FIM PAULOPINA: REMOVER TAGLIB

  //! Tag reading
//  CLog::log(LOG_DEBUG, "MEDIAITEM", "Reading main tags", "Taglib");
//  TagLib::Tag *tag = fileref.tag();

//  if (tag)
//  {
//    m_songInfo->setTitle(TStringToQString(tag->title()).trimmed());
//    getArtist()->setName(TStringToQString(tag->artist()).trimmed());
//    m_albumInfo->setTitle(TStringToQString(tag->album()).trimmed());
//    m_songInfo->setGenre(TStringToQString(tag->genre()).trimmed());
//    m_songInfo->setTrack(tag->track());
//    m_songInfo->setYear(tag->year());
//  }
//  else
//  {
//    CLog::log(LOG_ERROR, "MEDIAITEM", "Can't read main tags", "Taglib");
//  }

//  qDebug() << "DEBUG: 222";

//  if ( TagLib::MPEG::File* file = dynamic_cast<TagLib::MPEG::File *>( fileref.file() ) )
//  {
//    qDebug() << "DEBUG: MediaItem -> Taglib: MPEG file found. Reading...";
//    if ( file->ID3v2Tag() )
//    {
//      qDebug() << "DEBUG: MediaItem -> Taglib: ID3v2 tag found. Reading...";
//      if ( !file->ID3v2Tag()->frameListMap()["TPE2"].isEmpty() )
//      {
//        const QString value = TStringToQString( file->ID3v2Tag()->frameListMap()["TPE2"].front()->toString() ).trimmed();
//        qDebug() << "DEBUG: MediaItem -> Taglib: Album Artist: " << value;
//        if(!value.isEmpty())
//          getArtist()->setName(value);

////        qDebug() << "DEBUG: MediaItem -> Taglib: Album Artist: " << value;
//      }
//      if ( !file->ID3v2Tag()->frameListMap()["TPE1"].isEmpty() )
//      {
//        const QString value = TStringToQString( file->ID3v2Tag()->frameListMap()["TPE1"].front()->toString() ).trimmed();
//        if(!value.isEmpty())
//          m_songInfo->setArtist(value);

//        qDebug() << "DEBUG: MediaItem -> Taglib: Song Artist: " << value;
//      }
//      if ( !file->ID3v2Tag()->frameListMap()["TPUB"].isEmpty() )
//      {
//        const QString value = TStringToQString( file->ID3v2Tag()->frameListMap()["TPUB"].front()->toString() ).trimmed();
//        m_songInfo->setLabel(value);
//        qDebug() << "DEBUG: MediaItem -> Taglib: Label: " << value;
//      }
//      if ( !file->ID3v2Tag()->frameListMap()["TSRC"].isEmpty() )
//      {
//        const QString value = TStringToQString( file->ID3v2Tag()->frameListMap()["TSRC"].front()->toString() ).trimmed();
//        m_songInfo->setIsrc(value);
//        qDebug() << "DEBUG: MediaItem -> Taglib: ISRC: " << value;
//      }
//    }
//  }
//  else if (TagLib::Ogg::Vorbis::File* file = dynamic_cast<TagLib::Ogg::Vorbis::File*>( fileref.file() ))
//  {
//    qDebug() << "DEBUG: MediaItem -> Taglib: OGG Vorbis file found. Reading...";
//    if (file->tag())
//    {
//      const TagLib::Ogg::FieldListMap& map = file->tag()->fieldListMap();

//      if (!map["ALBUMARTIST"].isEmpty())
//      {
//        getArtist()->setName(TStringToQString( map["ALBUMARTIST"].front() ).trimmed());
//      }
//      else if (!map["ALBUM ARTIST"].isEmpty())
//      {
//        getArtist()->setName(TStringToQString( map["ALBUM ARTIST"].front() ).trimmed());
//      }
//      if (!map["ARTIST"].isEmpty())
//      {
//        m_songInfo->setArtist(TStringToQString( map["ARTIST"].front() ).trimmed());
//      }
//      if (!map["PUBLISHER"].isEmpty())
//      {
//        m_songInfo->setLabel(TStringToQString( map["PUBLISHER"].front() ).trimmed());
//      }
//    }
//  }
//  else if (TagLib::FLAC::File* file = dynamic_cast<TagLib::FLAC::File*>( fileref.file() ))
//  {
//    qDebug() << "DEBUG: MediaItem -> Taglib: FLAC file found. Reading...";
//    if ( file->xiphComment() )
//    {
//      const TagLib::Ogg::FieldListMap& map = file->xiphComment()->fieldListMap();
//      if (!map["ALBUMARTIST"].isEmpty())
//      {
//        getArtist()->setName(TStringToQString( map["ALBUMARTIST"].front() ).trimmed());
//      }
//      else if (!map["ALBUM ARTIST"].isEmpty())
//      {
//        getArtist()->setName(TStringToQString( map["ALBUM ARTIST"].front() ).trimmed());
//      }
//      if (!map["ARTIST"].isEmpty())
//      {
//        m_songInfo->setArtist(TStringToQString( map["ARTIST"].front() ).trimmed());
//      }
//      if (!map["PUBLISHER"].isEmpty())
//      {
//        m_songInfo->setLabel(TStringToQString( map["PUBLISHER"].front() ).trimmed());
//      }
//    }
//  }

  return true;
#else
  return false;
#endif
}

bool CMediaItem::isAudio()
{
  return audioFilter.contains(suffix().toLower());
}

bool CMediaItem::isVideo()
{
  return videoFilter.contains(suffix().toLower());
}

bool CMediaItem::isAudio(const QString& strFileName)
{
  return audioFilter.contains(QFileInfo(strFileName).suffix().toLower());
}

bool CMediaItem::isVideo(const QString& strFileName)
{
  return videoFilter.contains(QFileInfo(strFileName).suffix().toLower());
}


CMediaItem::~CMediaItem()
{
//  if (m_artistInfo)
//    delete m_artistInfo;
//  if (m_albumInfo)
//    delete m_albumInfo;
//  if (m_songInfo)
//    delete m_songInfo;
}

QString CMediaItem::buildImageName(const QString& strArtist, const QString& strAlbum, bool bThumb)
{
  if( (!strArtist.isEmpty()) )
  {
    QCryptographicHash hash(QCryptographicHash::Sha1);
    hash.addData(strArtist.toUtf8().constData());

    if ( !strAlbum.isEmpty() )
      hash.addData(strAlbum.toUtf8().constData());

    if (bThumb)
      return QString(hash.result().toHex() + ".tbn");
    else
      return QString(hash.result().toHex() + ".jpg");
  }

  return QString();
}

QString CMediaItem::findCoverName(const QString& strFileName)
{
  CLog::log(LOG_DEBUG, "MEDIAITEM", "Searching cover for" + strFileName, "Media Info");

  //! search album art into file source directory
  const QStringList imageFilters = QStringList() << "*.jpg" << "*.png";
  QDir sourceDir(QFileInfo(strFileName).absolutePath());

  sourceDir.setNameFilters(imageFilters);

  QStringList entryList = sourceDir.entryList(coverFilter, QDir::Files, QDir::Size);
  if (entryList.size() <= 0)
    entryList = sourceDir.entryList(imageFilters, QDir::Files, QDir::Size);

  QString strCoverFileName;
  if (entryList.size() > 0) {
    strCoverFileName = QFileInfo(strFileName).absolutePath() + "/" + entryList.takeFirst();
    CLog::log(LOG_INFO, "MEDIAITEM", "Cover found: " + strCoverFileName, "Media Info");
  }
  else
  {
    //strCoverFileName = MEDIA::GetThumbName(mi->data[SONG::strAlbum].toString(), mi->data[SONG::strAlbum].toString(), false);
    CLog::log(LOG_WARNING, "MEDIAITEM", "Cover not found", "Media Info");
  }

  return strCoverFileName;
}

QString CMediaItem::lenghtFromInt(const int lenght)
{
  const int duree = (lenght) > 0 ? lenght : 0;
  return QTime(0,0,0,0).addSecs(duree).toString("mm:ss");;
}
