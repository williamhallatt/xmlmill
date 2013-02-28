/* Copyright (c) 2012 by William Hallatt.
 *
 * This file forms part of "XML Mill".
 *
 * The official website for this project is <http://www.goblincoding.com> and,
 * although not compulsory, it would be appreciated if all works of whatever
 * nature using this source code (in whole or in part) include a reference to
 * this site.
 *
 * Should you wish to contact me for whatever reason, please do so via:
 *
 *                 <http://www.goblincoding.com/contact>
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program (GNUGPL.txt).  If not, see
 *
 *                    <http://www.gnu.org/licenses/>
 */

#ifndef GCDOMTREEWIDGET_H
#define GCDOMTREEWIDGET_H

#include <QTreeWidget>

class GCTreeWidgetItem;
class QDomDocument;

class GCDomTreeWidget : public QTreeWidget
{
  Q_OBJECT
public:
  /*! Constructor. */
  GCDomTreeWidget( QWidget *parent = 0 );

  /*! Destructor. */
  ~GCDomTreeWidget();

  /*! Returns the current item as a GCTreeWidgetItem. */
  GCTreeWidgetItem* gcCurrentItem() const;

  /*! Returns the underlying DOM document via the default shallow copy constructor. */
  QDomDocument document() const;

  /*! Returns a list of all the GCTreeWidgetItems in the tree. */
  QList< GCTreeWidgetItem* > includedGcTreeWidgetItems() const;

  /*! This function starts the recursive process of populating the tree widget with items
      consisting of the element hierarchy starting at "baseElementName". If "baseElementName"
      is empty, a complete hierarchy of the current active profile will be constructed. This
      method also automatically clears and resets GCDomTreeWidget's state, expands the
      entire tree, sets the first top level item as current and emits the "gcCurrentItemChanged"
      signal.
      \sa processNextElement */
  void populateFromDatabase( const QString &baseElementName = QString() );

  /*! Adds a new item and corresponding DOM element node named "element". If the tree
      is empty, the new item will be added to the invisible root, otherwise it will be
      added as a child of the current item.  The new item is also set as the current item.
      \sa insertItem */
  void addItem( const QString &element );

  /*! Adds a new item and corresponding DOM element node named "elementName" and inserts
      the new tree widget item into position "index" of the current item. If the tree
      is empty, the new item will be added to the invisible root. The new item is also set
      as the current item.
      \sa addItem */
  void insertItem( const QString &elementName, int index );

  /*! Iterates through the tree and sets all items' check states to "state". */
  void setAllCheckStates( Qt::CheckState state );

  /*! Clears and resets the tree as well as the underlying DOM document. */
  void clearAndReset();

signals:
  /*! \sa emitGcCurrentItemChanged */
  void gcCurrentItemChanged( GCTreeWidgetItem*,int );

private slots:
  /*! Connected to "itemClicked(QTreeWidgetItem*,int)". Re-emits the clicked item
      as a GCTreeWidgetItem.
      \sa gcCurrentItemChanged */
  void emitGcCurrentItemChanged( QTreeWidgetItem* item, int column );

private:
  /*! Processes individual elements.  This function is called recursively from within
      "populateFromDatabase", creating a representative tree widget item (and corresponding
      DOM element) named "element" and adding it (the item) to the correct parent.
      @param element - the name of the element for which a tree widget item must be created.
      \sa populateFromDatabase */
  void processNextElement( const QString &element );

  QDomDocument *m_domDoc;
  bool          m_isEmpty;

  QList< GCTreeWidgetItem* > m_items;
};

#endif // GCDOMTREEWIDGET_H
