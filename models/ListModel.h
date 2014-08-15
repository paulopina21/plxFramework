#ifndef LISTMODEL_H
#define LISTMODEL_H

#include <QAbstractListModel>
#include <QVariant>
#include <QList>
#include <QHash>
#include <QByteArray>
#include <QtAlgorithms>
#include "ListItem.h"

bool compareFunc(void *a, void *b);

class ListModel : public QAbstractListModel
{
  Q_OBJECT
  Q_PROPERTY(int count READ rowCount NOTIFY countChanged)
  Q_PROPERTY(bool sorting READ sortingEnabled WRITE setSorting NOTIFY sortingChanged)
public:
  explicit ListModel(ListItem *prototype, QObject *parent = 0);
  ~ListModel();

  // REIMPLEMENTED METHODS
  QVariant                data(const QModelIndex &index, int role) const;
  QHash<int, QByteArray>  roleNames() const;

  void                    appendRow(ListItem *item);
  void                    appendRows(QList<ListItem *> &items);
  void                    insertRow(int row, ListItem *item);
  ListItem*               takeRow(int row = -2, const QModelIndex &index = QModelIndex());
  QList<ListItem *>       takeRows(int row = -2, int count = -1, const QModelIndex &index = QModelIndex());
  bool                    removeRow(int row, const QModelIndex &index = QModelIndex());
  bool                    removeRows(int row, int count, const QModelIndex &index = QModelIndex());
  void                    removeFirst();
  void                    removeLast();

  ListItem*               find(int itemId) const;
  int                     getRowFromItem(ListItem *item) const;
  QModelIndex             indexFromItem(ListItem *item) const;
  QList<ListItem *>       toList() const;

  Q_INVOKABLE QVariant    get(int index);
  Q_INVOKABLE ListItem*   at(int index);
  Q_INVOKABLE ListItem*   first();
  Q_INVOKABLE ListItem*   last();
  Q_INVOKABLE int         rowIndexFromId(int id);
  Q_INVOKABLE void        clear();

  bool                    sortingEnabled() const;
  void                    setSorting(bool value);

protected:
  ListItem*               m_prototype;
  QList<ListItem *>       m_items;
  bool                    m_sortEnabled;

private:
  void sort();

public slots:
  Q_INVOKABLE int         rowCount(const QModelIndex &parent = QModelIndex()) const;
  Q_INVOKABLE int         count() const;
  bool                    isEmpty() const;

private slots:
  void updateItem();

signals:
  void countChanged(int);
  void sortingChanged(bool);
};

#endif // LISTMODEL_H
