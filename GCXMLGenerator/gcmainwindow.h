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

#ifndef GCMAINWINDOW_H
#define GCMAINWINDOW_H

#include <QMainWindow>
#include <QHash>

/*------------------------------------------------------------------------------------------

  All the code refers to "databases" whereas all the user prompts reference "profiles". This
  is deliberate.  In reality, everything is persisted to SQLite database files, but a friend
  suggested that end users may be intimidated by the use of the word "database" (especially
  if they aren't necessarily technically inclined) and that "profile" may be less scary :)

------------------------------------------------------------------------------------------*/

namespace Ui
{
  class GCMainWindow;
}

class GCDBSessionManager;
class GCDomElementInfo;
class QSignalMapper;
class QTreeWidgetItem;
class QTableWidgetItem;
class QComboBox;
class QDomDocument;
class QDomElement;
class QTimer;
class QLabel;

class GCMainWindow : public QMainWindow
{
  Q_OBJECT
  
public:
  explicit GCMainWindow( QWidget *parent = 0 );
  ~GCMainWindow();

protected:
  void closeEvent( QCloseEvent * event );

private slots:
  /* Called only when a user edits the name of an existing tree widget item
    (i.e. element). An element with the new name will be added to the DB
    (if it doesn't yet exist) with the same associated attributes and attribute
    values as the element name it is replacing (the "old" element will not be
    removed from the DB). All occurrences of the old name throughout the current
    DOM will be replaced with the new name and the tree widget will be updated
    accordingly. */
  void elementChanged( QTreeWidgetItem *item, int column );

  /* Triggered by clicking on a tree widget item, the trigger will populate
    the table widget with the names of the attributes associated with the
    highlighted item's element as well as combo boxes containing their known
    values.  This function will also create "empty" cells and combo boxes so
    that the user may add new attribute names to the selected element.  The
    addition of new attributes and values will automatically be persisted
    to the database. */
  void elementSelected( QTreeWidgetItem *item, int column );

  /* This function is called when the user changes the name of an existing attribute
    via the table widget, or when the attribute's include/exclude state changes. The new
    attribute name will be persisted to the database (with the same known values of
    the "old" attribute) and associated with the current highlighted element.  The current
    DOM will be updated to reflect the new attribute name instead of the one that was replaced. */
  void attributeChanged ( QTableWidgetItem *item );
  void attributeSelected( QTableWidgetItem *item );

  /* Triggered whenever the current value of a combo box changes or when the user edits
    the content of a combo box.  In the first scenario, the DOM will be updated to reflect
    the new value for the specific element and associated attribute, in the latter case,
    the edited/provided value will be persisted to the database as a known value against
    the current element and associated attribute if it was previously unknown. */
  void attributeValueChanged( const QString &value );

  /* Called whenever the user enters or otherwise activates a combo box.  The active
    combo box is used to determine the row of the associated attribute (in the table
    widget), which in turn is required in order to determine which attribute must be
    updated when an attribute value changes. */
  void setCurrentComboBox( QWidget *combo );

  /* XML file related. */
  void newXMLFile();
  void openXMLFile();
  void saveXMLFile();
  void saveXMLFileAs();

  /* DOM and DB. */
  void addNewDatabase() const;
  void addExistingDatabase() const;
  void removeDatabase() const;
  void switchActiveDatabase() const;
  void importXMLToDatabase();

  void deleteElementFromDocument();
  void addElementToDocument();
  void addSnippetToDocument();
  void resetDOM();

  void showRemoveItemsForm();
  void showAddItemsForm();

  /* Direct DOM edit. */
  void revertDirectEdit();
  void saveDirectEdit();

  /* These do exactly what you would expect. */
  void collapseOrExpandTreeWidget( bool checked );
  void activeDatabaseChanged( QString dbName );
  void elementFound( const QDomElement &element );
  void forgetMessagePreferences();
  void searchDocument();
  void insertSnippet();
  void showDOMEditHelp();
  void showMainHelp();
  void goToSite();
  
private:
  void processDOMDoc();
  void populateTreeWidget( const QDomElement &parentElement, QTreeWidgetItem *parentItem );

  void setStatusBarMessage( const QString &message );
  void showErrorMessageBox( const QString &errorMsg );
  void setTextEditContent ( const QDomElement &element );
  void showLargeFileWarnings( qint64 fileSize );

  void insertEmptyTableRow();
  void resetTableWidget();
  void deleteElementInfo();
  void startSaveTimer();
  void toggleAddElementWidgets();

  Ui::GCMainWindow   *ui;
  GCDBSessionManager *m_dbSessionManager;
  QSignalMapper      *m_signalMapper;
  QDomDocument       *m_domDoc;
  QTableWidgetItem   *m_activeAttribute;
  QWidget            *m_currentCombo;
  QTimer             *m_saveTimer;
  QLabel             *m_activeSessionLabel;
  QString             m_currentXMLFileName;
  QString             m_activeAttributeName;
  bool                m_wasTreeItemActivated;
  bool                m_newAttributeAdded;
  bool                m_busyImporting;
  bool                m_DOMTooLarge;

  QHash< QTreeWidgetItem*, GCDomElementInfo* > m_elementInfo;
  QHash< QTreeWidgetItem*, QDomElement > m_treeItemNodes;
  QHash< QWidget*, int/* table row*/ >   m_comboBoxes;

};

#endif // GCMAINWINDOW_H
