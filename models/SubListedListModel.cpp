#include "SubListedListModel.h"

/*!
 * \class SubListedListModel
 * \code
 * #include <SubListedListModel.h>
 * \endcode
 * \brief The Models::SubListedListModel class provides an easy way to create C++ n-depth models
 * and expose them for Qml. This is useful in cases where you need to retrieve a sub model from
 * a given element in a model.
 *
 * \inmodule plxFW
 *
 * \sa ListModel
 */

/*!
 * \class SubListedListItem
 * \code
 * #include <SubListedListItem.h>
 * \endcode
 *
 * \brief The basic element SubListedListModel are composed of.
 *
 * The basic element SubListedListModel are composed of. It gives a representation
 * of the data it contains throught the use of data retrieved by matching role names.
 * Your model items should inherit from this class and implement the various methods to reflect
 * what your item contains.
 *
 * The difference with the ListItem element is that a SubListedListModel can contain a
 * submodel. This is particularly interesting when you want to have a model containing another model for
 * each of its row.
 *
 * \sa ListModel
 * \sa SubListedListModel
 * \sa ListItem
 *
 * \inmodule plxFW
 */

/*!
 * \fn  SubListedListItem::SubListedListItem(QObject *parent)
 *
 * Constructs a new SubListedListItem instance given the optionnal \a parent paremeter.
 */

/*!
 * \fn SubListedListItem::~SubListedListItem()
 *
 * Destroys a SubListedListItem instance.
 */

/*!
 * \fn ListModel* SubListedListItem::submodel() const
 *
 * Returns a plxModles::ListModel* instance.
 */



/*!
 * Construcst a new SubListedListModel instance with the provided \a prototype item template.
 */

SubListedListModel::SubListedListModel(SubListedListItem *prototype, QObject *parent) : ListModel(prototype, parent)
{
}

/*!
 * Returns the model contained by row Item identified by \a id.
 */
QObject* SubListedListModel::subModelFromId(int id)
{
  SubListedListItem* item = (SubListedListItem *)this->find(id);
  if (item != NULL)
    return item->submodel();
  return NULL;
}
