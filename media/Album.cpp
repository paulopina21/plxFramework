#include "Album.h"

CAlbum::CAlbum(QObject *parent): QObject(parent)
{
  qmlRegisterType<CAlbum>("music", 1, 0, "Album");
  m_cover = new CImage(this);
  m_artist = NULL;//new CArtist(this);
}

void CAlbum::setArtist(CArtist *value)
{
  if (m_artist)
    delete m_artist;

  m_artist = value;
}

void CAlbum::setField(CAlbum::Fields field, const QVariant &value)
{
  if (m_fields[field] == value) return;

  m_fields[field] = value;

  switch (field)
  {
    case ID:
      emit idAlbumChanged();
    break;

    case ID_ARTIST:
      emit idArtistChanged();
    break;

    case ARTIST:
      emit artistChanged();
    break;

    case TITLE:
      emit titleChanged();
    break;

    case GENRE:

    break;

    case YEAR:
      emit yearChanged();
    break;

    case LABEL:
      emit labelChanged();
    break;

    case PFC_FILE:
    break;
    case ID_COVER:
    break;
    case SONGS_COUNT:
    break;
    case PLAY_COUNT:
    break;
    case RATING:
    break;
  }

}

void CAlbum::setCover(CImage *cover)
{
  m_cover = cover;
  emit coverChanged();
}

int CAlbum::addSong(CSong *song)
{
  m_songs << song;
  emit songsChanged();

  return m_songs.size() -1;
}

void CAlbum::setSongs(QList<CSong *> songsList)
{
  m_songs = songsList;
}

CSong* CAlbum::getSong(int i)
{
  return m_songs.at(i);
}

int CAlbum::countSongs()
{
  return m_songs.size();
}
