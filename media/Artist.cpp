#include "Artist.h"

CArtist::CArtist(QObject *parent): QObject(parent)
{
  qmlRegisterType<CArtist>("music", 1, 0, "Artist");

  m_fanart = new CImage(this);
}

CArtist &CArtist::operator =(CArtist &newItem)
{
  if (this == &newItem)      // Same object?
     return *this;        // Yes, so skip assignment, and just return *this.

  setidArtist(newItem.idArtist());
  setName(newItem.name());
  setFanart(newItem.fanart());

  return *this;
//  setAlbums(newItem.getAlbums());
}

//QVariant CArtist::data(int role) const
//{

//  switch(role) {
//    case ID:
//      return QVariant::fromValue(idArtist());
//    break;
//    case NAME:
//      return QVariant::fromValue(name());
//    break;
//    case FANART:
//      return QVariant::fromValue(fanart());
//    break;
////    case ALBUMS:
////      return QVariant::fromValue(albums());
////    break;

//    default:
//    return QVariant();
//  }

//}

QHash<int, QByteArray> CArtist::roleNames() const
{
  QHash<int, QByteArray> names;
  names[ID] = "idArtist";
  names[NAME] = "name";
  names[FANART] = "fanart";
//  names[ALBUMS] = "albums";
  return names;
}

void CArtist::setData(Roles field, const QVariant &value)
{
  if (m_fields[field] == value) return;

  m_fields[field] = value;

  switch (field)
  {
    case ID:
      emit idArtistChanged();
    break;
    case NAME:
      emit nameChanged();
    break;

    default:
    break;
  }

}

//int CArtist::addAlbum(CAlbum *album)
//{
//  m_albums << album;
//  emit albumsChanged();

//  return m_albums.size() -1;
//}

//void CArtist::setAlbums(QList<CAlbum *> albumsList)
//{
//  m_albums = albumsList;
//  emit albumsChanged();
//}
