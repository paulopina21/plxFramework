#include "Catalog.h"

CCatalog::CCatalog(QObject *parent): ListItem(parent)
{

}

QHash<int, QByteArray> CCatalog::roleNames() const
{
  QHash<int, QByteArray> names;
  names[FIELD_ID]           = "idCatalog";  
  names[FIELD_NAME]         = "name";
  names[FIELD_SOURCE]       = "source";
  names[FIELD_OUTPUTPATH]   = "outputPath";
  names[FIELD_ALBUMCOUNT]   = "albumCount";
  names[FIELD_ARTISTCOUNT]  = "artistCount";
  names[FIELD_SONGCOUNT]    = "songCount";
  names[FIELD_AUTOREBUILD]  = "autoRebuild";
  names[FIELD_DBFILENAME]   = "dbFileName";

  return names;
}
