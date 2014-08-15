#ifndef CONFIGITEM_H
#define CONFIGITEM_H

#include <QObject>
#include <QMap>
//#include <QVariant>
//#include <QtQml>

#include "../model/listmodel.h"

class CConfigGroup;

class CConfigItem: public ListItem
{
  Q_OBJECT
  Q_ENUMS(ConfigItemType)
public:
  enum ConfigItemType
  {
    TEXT = 0,
    INT,
    BOOL,
    ARRAY
  };

  enum Roles
  {
    GroupRole = Qt::UserRole +1,
    NameRole,
    TitleRole,
    DescRole,
    ValueRole,
    TypeRole
  };

  explicit CConfigItem(QObject *parent = 0);

//  QVariant data(int role) const;
  QHash<int, QByteArray> roleNames() const;

//  Q_PROPERTY(QString group READ group WRITE setGroup NOTIFY groupChanged)
//  Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
//  Q_PROPERTY(QVariant value READ value WRITE setValue NOTIFY valueChanged)
//  Q_PROPERTY(ConfigItemType type READ type WRITE setType NOTIFY typeChanged)

signals:
  void groupChanged();
  void nameChanged();
  void titleChanged();
  void descChanged();
  void valueChanged();
  void typeChanged();

public slots:
  QString group() const { return data(GroupRole).toString(); }
  void setGroup(QVariant value) { setData(GroupRole,value); }

  QString name() const { return data(NameRole).toString(); }
  void setName(QVariant value) { setData(NameRole,value); }

  QString title() const { return data(TitleRole).toString(); }
  void setTitle(QVariant value) { setData(TitleRole,value); }

  QString desc() const { return data(DescRole).toString(); }
  void setDesc(QVariant value) { setData(DescRole,value); }

  QVariant value() const { return data(ValueRole); }
  void setValue(QVariant value) { setData(ValueRole,value); }

  ConfigItemType type() const { return data(TypeRole).value<ConfigItemType>(); }
  void setType(QVariant value) { setData(TypeRole,value); }

protected slots:
//  QVariant field(Roles field) const { return m_fields[field]; }
//  void setData(Roles data, const QVariant &value);

private:
//  QMap<int, QVariant> m_fields;
  CConfigGroup* m_children;

};

Q_DECLARE_METATYPE(CConfigItem::ConfigItemType)

#endif // CONFIGITEM_H
