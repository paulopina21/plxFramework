#ifndef CATALOG_H
#define CATALOG_H

#include "../models/ListItem.h"

class CCatalog: public ListItem {
  Q_OBJECT
public:

  enum Roles
  {
    FIELD_ID,
    FIELD_NAME,
    FIELD_SOURCE,
    FIELD_OUTPUTPATH,
    FIELD_AUTOREBUILD,
    FIELD_ALBUMCOUNT,
    FIELD_ARTISTCOUNT,
    FIELD_SONGCOUNT,
    FIELD_DBFILENAME
  };

  CCatalog(QObject *parent = 0);

  virtual int id() const { return data(FIELD_ID).toInt(); }
  virtual QHash<int, QByteArray> roleNames() const;
};

#endif // CATALOG_H
