#include "configitem.h"
#include "configgroup.h"

CConfigItem::CConfigItem(QObject *parent): ListItem(parent)
{
  qmlRegisterType<CConfigItem>("settings", 1, 0, "ConfigItem");
//  m_children = new ListModel(new CConfigItem(this), this);
}

//QVariant CConfigItem::data(int role) const
//{
//  switch(role)
//  {
//    case TypeRole:
//      return QVariant::fromValue(type());
//    break;
//    case GroupRole:
//      return QVariant::fromValue(group());
//    break;
//    case NameRole:
//      return QVariant::fromValue(name());
//    break;
//    case TitleRole:
//      return QVariant::fromValue(title());
//    break;
//    case DescRole:
//      return QVariant::fromValue(desc());
//    break;
//    case ValueRole:
//      return QVariant::fromValue(value());
//    break;
//    default:
//      return QVariant();
//    break;
//  }
//}


//void CConfigItem::setData(CConfigItem::Roles field, const QVariant &value)
//{
//  if (m_fields[field] == value) return;
//  m_fields[field] = value;

//  switch (field)
//  {
//    case GroupRole:
//      emit groupChanged();
//    break;
//    case NameRole:
//      emit nameChanged();
//    break;
//    case TitleRole:
//      emit titleChanged();
//    break;
//    case DescRole:
//      emit descChanged();
//    break;
//    case ValueRole:
//      emit valueChanged();
//    break;
//    case TypeRole:
//      emit typeChanged();
//    break;
//    default:
//    break;
//  }
//  emit dataChanged();
//}


QHash<int, QByteArray> CConfigItem::roleNames() const
{
  QHash<int, QByteArray> names;
  names[TypeRole] = "type";
  names[GroupRole] = "group";
  names[NameRole] = "name";
  names[TitleRole] = "title";
  names[DescRole] = "desc";
  names[ValueRole] = "value";
  return names;
}
