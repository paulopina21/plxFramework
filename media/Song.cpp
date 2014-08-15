#include "Song.h"

#include <QtQml/QtQml>

CSong::CSong(QObject *parent): QObject(parent)
{
  qmlRegisterType<CSong>("music", 1, 0, "Song");

  m_fields[ID]        = -1;
//  m_fields[ID_ALBUM]  = -1;
//  m_fields[ID_ARTIST] = -1;
  m_fields[IS_BROKEN] = false;
  m_fields[IS_VIDEO] = false;
}

CSong::~CSong()
{
  m_fields.clear();
}

bool CSong::setData(int role, const QVariant &value)
{
  if (m_fields[role] == value) return false;
  m_fields[role] = value;
//  setProperty(metaObject()->property(role).name(), value);
  emit dataChanged();
  return true;
}


QVariant CSong::data(int role) const
{
  return m_fields[role];
//  return property(metaObject()->property(role).name());
}



//void CSong::setField(CSong::Roles field, const QVariant& value)
//{
//  if (m_fields[field] == value) return;

//  m_fields[field] = value;

//  switch (field) {
//    case ID:
//      emit idSongChanged();
//    break;
//    case ID_ALBUM:
//      emit idAlbumChanged();
//    break;
//    case ID_ARTIST:
//      emit idArtistChanged();
//    break;
//    case ARTIST:
//      emit artistChanged();
//    break;
//    case ALBUM_ARTIST:
//      emit albumArtistChanged();
//    break;
//    case ALBUM:
//      emit albumChanged();
//    break;
//    case TRACK:
//      emit trackChanged();
//    break;
//    case TITLE:
//      emit titleChanged();
//    break;
//    case LENGHT:
//      emit lenghtChanged();
//    break;
//    case YEAR:
//      emit yearChanged();
//    break;
//    case GENRE:
//      emit genreChanged();
//    break;
//    case ISRC:
//      emit isrcChanged();
//    break;
//    case LABEL:
//      emit labelChanged();
//    break;
//    case FILEPATH:
//      emit filePathChanged();
//    break;
//    case TIMESTAMP:
//      emit timestampChanged();
//    break;
//    case IS_BROKEN:
//      emit brokenChanged();
//    break;
//  }

//}
