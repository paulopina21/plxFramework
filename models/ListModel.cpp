#include "ListModel.h"
// DEBUG
#include <QDebug>


/*!
 * \namespace plxModels
 *
 * \brief The Models namespace contains all classes use for Data Model
 * encapsulation within the application.
 *
 * \inmodule plxFW
 *
 *
 */


/*!
 * \class ListItem
 *
 * \code
 * #include <ListItem.h>
 * \endcode
 *
 * \brief The basic element ListModel are composed of.
 *
 * The basic element ListModel are composed of. It gives a representation
 * of the data it contains throught the use of data retrieved by matching role names.
 * Your model items should inherit from this class and implement the various methods to reflect
 * what your item contains.
 *
 * \sa ListModel
 *
 * \inmodule plxFW
 */

/*!
 * \fn ListItem::ListItem(QObject *parent)
 *
 * Constructs a new model item with the optionnal \a parent argument/
 */

/*!
 * \fn ListItem::~ListItem()
 *
 * Destroys a ListItem instance.
 */

/*!
 * \fn int ListItem::id() const
 *
 * Returns the id of the item.
 */

/*!
 * \fn QVariant ListItem::data(int role) const
 *
 * Returns the data associated to a given \a role.
 */

/*!
 * \fn QHash<int, QByteArray> ListItem::roleNames() const
 *
 * Returns a QHash containing the roleNames and a corresponding role identifier.
 */

/*!
 * \fn void ListItem::triggerItemUpdate()
 *
 * Triggers an update of the data model to reflect the changes of the ListItem.
 */

/*!
 * \fn void ListItem::dataChanged()
 *
 * Triggered when an update of the ListItem is requested.
 */

/*!
 * \class ListModel
 *
 * \code
 * #include <ListModel.h>
 * \endcode
 *
 * \brief The ListModel class is a utility model class
 *
 * The ListModel class provides an easy way to create a C++ model
 * and later expose it to the Qml View.
 *
 * \inmodule plxFW
 *
 * \sa Models::SubListedListModel
 */

/*!
 * Instanciates a new ListModel and sets the list row prototype to \a prototype.
 * The \a parent parameter is optional.
 *
 */
ListModel::ListModel(ListItem *prototype, QObject *parent) : QAbstractListModel(parent)
{
  this->m_prototype = prototype;
  this->m_sortEnabled = false;
}

/*!
 * Destroys a ListModel instance.
 */
ListModel::~ListModel()
{
  delete this->m_prototype;
  this->m_prototype = NULL;
  this->clear();
}

/*!
 * \property Models::ListModel::count
 *
 * This property holds the number of rows of the model.
 */

/*!
 * Returns number of rows in the model.
 * \a index is not used but needed to reimplement the method.
 */
int ListModel::rowCount(const QModelIndex &) const
{
  return this->m_items.size();
}

int ListModel::count() const
{
  return this->m_items.size();
}

bool ListModel::isEmpty() const
{
  return this->m_items.isEmpty();
}

/*!
 * Returns a QVariant containing the data associed to \a role for row at \a index.
 */
QVariant ListModel::data(const QModelIndex &index, int role) const
{
  if (index.row() >= 0 && index.row() < this->m_items.size())
    return this->m_items.at(index.row())->data(role);
  return QVariant();
}

/*!
 * Returns a hash containing the roleNames of the Model.
 */
QHash<int, QByteArray>  ListModel::roleNames() const
{
  return this->m_prototype->roleNames();
}

/*!
 * Returns the ListItem contained at \a row if it has been specified.
 * Otherwise the first ListItem is returned.
 */
ListItem *ListModel::takeRow(int row, const QModelIndex &index)
{
  if (row == -2) // IF ROW HAS NOT BEEN SPECIFIED TAKE FIRST ITEM
    row = 0;
  if (row >= 0 && row < this->m_items.size())
  {
    beginRemoveRows(index, row, row);
    ListItem *item = this->m_items.takeAt(row);
    endRemoveRows();
    emit (countChanged(this->rowCount()));
    return item;
  }
  return NULL;
}

/*!
 * Returns a list of items that are contained in the model from the row \a row
 * to the row \a row + \a count. The items are removed from the model.
 * If \a row is not specified, it starts from the first row.
 * If count is not specified, returns items from \a row to the end of the model.
 * In that case if row is not the first row, nothing will be returned.
 */
QList<ListItem *> ListModel::takeRows(int row, int count, const QModelIndex &index)
{
  QList<ListItem *> items;

  if (row == -2) // IF ROW HAS NOT BEEN SPECIFIED TAKE FIRST ITEM
    row = 0;
  if (count == -1)
    count = this->m_items.size();
  if (row >= 0 && count > 0 && (row + count) <= this->m_items.size())
  {
    beginRemoveRows(index, row, row + count - 1);
    for (int i = 0; i < count; i++)
      items << this->m_items.takeAt(row);
    endRemoveRows();
    emit (countChanged(this->rowCount()));
  }
  return items;
}

/*!
 * Appends a single row \a item to the Model.
 */
void ListModel::appendRow(ListItem *item)
{
  if (item != NULL)
  {
    this->appendRows(QList<ListItem *>() << item);
    emit (countChanged(this->rowCount()));
  }
}

/*!
 * Appends several rows \a items to the Model.
 */
void ListModel::appendRows(QList<ListItem *> &items)
{
  if (items.size() == 0)
    return ;
  // NEEDED TO UPDATE VIEW
  this->beginInsertRows(QModelIndex(), this->rowCount(), this->rowCount() + items.size() - 1);
  foreach(ListItem *item, items)
  {
    QObject::connect(item, SIGNAL(dataChanged()), this, SLOT(updateItem()));
    this->m_items.append(item);
  }
  // NEEDED TO UPDATE VIEW
  this->endInsertRows();
  this->sort();
  emit (countChanged(this->rowCount()));
}

/*!
 * Insert new row described by \a item at position defined by \a row.
 */
void ListModel::insertRow(int row, ListItem *item)
{
  if (item == NULL)
    return ;
  this->beginInsertRows(QModelIndex(), row, row);
  QObject::connect(item, SIGNAL(dataChanged()), this, SLOT(updateItem()));
  this->m_items.insert(row, item);
  this->endInsertRows();
  this->sort();
  emit (countChanged(this->rowCount()));
}

/*!
 * Removes a single row at position defined by \a row .
 * The \a index argument is optional.
 * Returns true if row was removed, false if row not found or \a row is invalid.
 */
bool ListModel::removeRow(int row, const QModelIndex &index)
{
  if (row >= 0 && row < this->m_items.size())
  {
    beginRemoveRows(index, row, row);
    ListItem *item = this->m_items.takeAt(row);
    delete item;
    item = NULL;
    endRemoveRows();
    emit (countChanged(this->rowCount()));
    return true;
  }
  return false;
}

/*!
 * Removes several rows starting at position defined by \a row until either \a count
 * or the model's last row is reached. The \a index argument is optional.
 * Returns true if the rows were removed, false if \a row is invalid.
 */
bool ListModel::removeRows(int row, int count, const QModelIndex &index)
{
  if (row >= 0 && count > 0 && (row + count) <= this->m_items.size())
  {
    beginRemoveRows(index, row, row + count - 1);
    for (int i = 0; i < count; i++)
    {
      ListItem *item = this->m_items.takeAt(row);
      delete item;
      item = NULL;
    }
    endRemoveRows();
    emit (countChanged(this->rowCount()));
    return true;
  }
  return false;
}

void ListModel::removeFirst()
{
  this->m_items.removeFirst();
  emit (countChanged(this->rowCount()));
}

void ListModel::removeLast()
{
  this->m_items.removeLast();
  emit (countChanged(this->rowCount()));
}

/*!
 * Clears the whole model removing all rows.
 */
void ListModel::clear()
{    
  this->removeRows(0, this->m_items.size());
  emit (countChanged(this->rowCount()));
}

bool ListModel::sortingEnabled() const
{
  return this->m_sortEnabled;
}

void ListModel::setSorting(bool value)
{
  if (value == this->sortingEnabled())
    return;
  this->m_sortEnabled = value;
  emit sortingChanged(value);
  this->sort();
}

bool compareFunc(void *a, void *b)
{
  return *reinterpret_cast<ListItem *>(a) < *reinterpret_cast<ListItem *>(b);
}
/*!
 * Sorts the elements of the models
 */
void ListModel::sort()
{
  if (this->m_sortEnabled)
  {
    qSort(this->m_items.begin(), this->m_items.end(), compareFunc);
    foreach (ListItem *item, this->m_items)
    {
      QModelIndex index = this->indexFromItem(item);
      if (index.isValid())
        emit dataChanged(index, index);
    }
  }
}

/*!
 * Returns the index of the row in the model containing \a item.
 */
QModelIndex ListModel::indexFromItem(ListItem *item) const
{
  if (item != NULL)
  {
    for (int i = 0; i < this->m_items.size(); i++)
      if (this->m_items.at(i) == item)
        return index(i);
  }
  return QModelIndex();
}

/*!
 * Returns the item whose id matches \a itemId.
 */
ListItem* ListModel::find(int itemId) const
{
  foreach(ListItem *item, this->m_items)
    if (item->id() == itemId)
      return item;
  return NULL;
}
/*!
 * Returns row index at which \a item can be found in the model.
 */
int ListModel::getRowFromItem(ListItem *item) const
{
  if (item != NULL)
    for (int i = 0; i < this->m_items.size(); i++)
      if (this->m_items.at(i) == item)
        return i;
  return -1;
}

/*!
 * Returns model as a QList.
 */
QList<ListItem *>   ListModel::toList() const
{
  return this->m_items;
}

/*!
 * Slot triggered when a row item needs to be updated to reflect data changes.
 */
void ListModel::updateItem()
{
  ListItem *item = static_cast<ListItem *>(sender());
  QModelIndex index = this->indexFromItem(item);
  if (index.isValid())
    emit dataChanged(index, index);
}

/*!
 * Returns a QVariant containg the data of the row item at \a index in the model.
 */
QVariant ListModel::get(int index)
{
  if (index >= this->m_items.size() || index < 0)
    return QVariant();
  ListItem * item = this->m_items.at(index);
  QMap<QString, QVariant> itemData;
  QHashIterator<int, QByteArray> hashItr(item->roleNames());

  while(hashItr.hasNext())
  {
    hashItr.next();
    itemData.insert(hashItr.value(),QVariant(item->data(hashItr.key())));
  }
  return QVariant(itemData);
}

ListItem *ListModel::at(int index)
{
  if (index >= this->m_items.size() || index < 0)
    return m_prototype;

  return this->m_items.at(index);
}

ListItem *ListModel::first()
{
  return this->m_items.first();
}

ListItem *ListModel::last()
{
  return this->m_items.last();
}

/*!
 * Returns the index for item identified by \a id  in the model.
 */
int ListModel::rowIndexFromId(int id)
{
  ListItem* item = find(id);

  if (item)
    return indexFromItem(item).row();
  return -1;
}
