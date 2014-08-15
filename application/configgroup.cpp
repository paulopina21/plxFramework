#include "configgroup.h"

CConfigGroup::CConfigGroup(QObject *parent): ListItem(parent)
{
//  qmlRegisterType<CConfigGroup>("settings", 1, 0, "ConfigGroup");
  m_children = new ListModel(this,this);
}

//QVariant CConfigGroup::data(int role) const
//{
//  switch(role)
//  {
//    case TypeRole:
//      return QVariant::fromValue(type());
//    break;
//    case NameRole:
//      return QVariant::fromValue(name());
//    break;
//    default:
//      return QVariant();
//    break;
//  }
//}

QHash<int, QByteArray> CConfigGroup::roleNames() const
{
  QHash<int, QByteArray> names;
  names[GroupRole] = "group";
  names[TypeRole] = "type";
  names[PermissionRole] = "permission";
  names[NameRole] = "name";
  names[TitleRole] = "title";
  names[DescRole] = "desc";
  names[ValueRole] = "value";
  return names;
}

void CConfigGroup::addChild(CConfigGroup *pItem)
{
  pItem->setGroup(name());
  children()->appendRow(pItem);
  emit dataChanged();
}

//void CConfigGroup::setName(QVariant value)
//{
//  m_name = value.toString();
//  emit nameChanged();
//  emit dataChanged();
//}

//void CConfigGroup::setTitle(QVariant value)
//{
//  m_title = value.toString();
//  emit titleChanged();
//  emit dataChanged();
//}

//void CConfigGroup::setDesc(QVariant value)
//{
//  m_desc = value.toString();
//  emit descChanged();
//  emit dataChanged();
//}

//void CConfigGroup::setPermission(QVariant value)
//{
//  m_groupType = value.value<ConfigPermission>();
//  emit typeChanged();
//  emit dataChanged();
//}
