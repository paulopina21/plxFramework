#ifndef CONFIGGROUP_H
#define CONFIGGROUP_H

#include <QObject>

//#include "configitem.h"
#include "../model/listmodel.h"

class CConfigGroup: public ListItem
{
  Q_OBJECT
  Q_ENUMS(ConfigPermission)
  Q_ENUMS(ConfigType)
public:
  enum Roles
  {
    GroupRole = Qt::UserRole +1,
    TypeRole,
    PermissionRole,
    NameRole,
    TitleRole,
    DescRole,
    ValueRole,
    ItemsRole
  };

  enum ConfigType
  {
    GROUP = 0,
    TEXT,
    INT,
    BOOL,
    ARRAY
  };

  enum ConfigPermission
  {
    PLEXUS = 0,
    ADMIN,
    USER
  };

  explicit CConfigGroup(QObject *parent = 0);

//  QVariant data(int role) const;
  QHash<int, QByteArray> roleNames() const;

signals:
//  void nameChanged();
//  void typeChanged();
//  void titleChanged();
//  void descChanged();

public slots:

  QString group() const { return data(GroupRole).toString(); }
  void setGroup(QVariant value) { setData(GroupRole, value); }

  ConfigType type() const { return data(TypeRole).value<ConfigType>(); }
  void setType(QVariant value) { setData(TypeRole, value); }

  ConfigPermission permission() const { return data(PermissionRole).value<ConfigPermission>(); }
  void setPermission(QVariant value) { setData(PermissionRole, value); }

  QString name() const { return data(NameRole).toString(); }
  void setName(QVariant value) { setData(NameRole, value); }

  QString title() const { return data(TitleRole).toString(); }
  void setTitle(QVariant value) { setData(TitleRole, value); }

  QString desc() const { return data(DescRole).toString(); }
  void setDesc(QVariant value) { setData(DescRole, value); }

  QVariant value() const { return data(ValueRole); }
  void setValue(QVariant value) { setData(ValueRole, value); }

  ListModel* children() { return m_children; }
  void addChild(CConfigGroup* pItem);

private:
  ListModel* m_children;
};

Q_DECLARE_METATYPE(CConfigGroup::ConfigPermission)
Q_DECLARE_METATYPE(CConfigGroup::ConfigType)

#endif // CONFIGGROUP_H
