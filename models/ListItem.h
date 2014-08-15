#ifndef LISTITEM_H
#define LISTITEM_H

#include <QObject>
#include <QVariant>
#include <QHash>

class ListItem : public QObject
{
  Q_OBJECT
public :
  ListItem(QObject *parent = 0) : QObject(parent) {}
  virtual ~ListItem() {}
  virtual int id() const = 0;
  Q_INVOKABLE virtual inline QVariant data(int role) const { return this->m_fields[role]; }
  virtual QHash<int, QByteArray> roleNames() const = 0;
  virtual inline void triggerItemUpdate() {emit dataChanged();}
  virtual bool operator<(const ListItem &nextElem) { Q_UNUSED(nextElem); return false; }
  virtual inline ListItem& operator = (const ListItem &other) { if (this == &other) return *this; this->m_fields = other.m_fields; return *this;}

  virtual inline void setData(int role, QVariant value) { this->m_fields[role] = value; triggerItemUpdate(); }
signals:
  void dataChanged();

protected:
  QMap<int, QVariant> m_fields;
};

#endif // LISTITEM_H
