/* Copyright (c) 2012 - 2015 by William Hallatt.
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
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program (GNUGPL.txt).  If not, see
 *
 *                    <http://www.gnu.org/licenses/>
 */
#include "elementeditwidget.h"
#include "valuecombobox.h"
#include "db/dbinterface.h"
#include "utils/messagespace.h"

#include <QStringListModel>
#include <QTableWidgetItem>
#include <QLabel>

#include <assert.h>

//----------------------------------------------------------------------

int ElementEditWidget::intFromEnum(Columns column) {
  return static_cast<int>(column);
}

//----------------------------------------------------------------------

ElementEditWidget::ElementEditWidget(QDomElement element, QTableWidget *table,
                                     bool primary)
    : QWidget(nullptr), m_element(element), m_table(table),
      m_associatedAttributes(), m_elementName(), m_parentElementName(),
      m_documentRoot() {
  assert(!m_element.isNull());
  assert(m_table);

  if (!m_element.isNull()) {
    m_elementName = m_element.tagName();
    m_documentRoot = m_element.ownerDocument().documentElement().tagName();

    /* If we don't have a parent node, we're dealing with the document root. */
    if (!m_element.parentNode().isNull() &&
        m_element.parentNode().isElement()) {
      m_parentElementName = m_element.parentNode().toElement().tagName();
    }

    retrieveAssociatedAttributes();

    if (!m_associatedAttributes.isEmpty()) {
      insertElementNameItem(primary);
      populateTable();
      connect(m_table, SIGNAL(itemChanged(QTableWidgetItem *)), this,
              SLOT(attributeChanged(QTableWidgetItem *)));
    }
  }
}

//----------------------------------------------------------------------

void ElementEditWidget::processResult(DB::Result status, const QString &error) {
  if (status != DB::Result::Failed) {
    MessageSpace::showErrorMessageBox(this, error);
  }
}

//----------------------------------------------------------------------

void ElementEditWidget::attributeChanged(QTableWidgetItem *item) {
  assert(item);

  if (item) {
    ValueComboBox *combo = valueComboForRow(item->row());

    if (combo) {
      bool checked = item->checkState() == Qt::Checked;
      combo->setEnabled(checked);

      QString attribute = item->text();

      if (checked) {
        QString value = combo->currentText();
        m_element.setAttribute(attribute, value);
      } else {
        m_element.removeAttribute(attribute);
      }

      emit contentsChanged();
    }
  }
}

//----------------------------------------------------------------------

void ElementEditWidget::attributeValueChanged(int row) {
  QTableWidgetItem *item = itemForRow(row);
  attributeChanged(item);
}

//----------------------------------------------------------------------

void ElementEditWidget::retrieveAssociatedAttributes() {
  DB db;
  connect(&db, SIGNAL(result(DB::Result, QString)), this,
          SLOT(processResult(DB::Result, QString)));

  m_associatedAttributes =
      db.attributes(m_elementName, m_parentElementName, m_documentRoot);
}

//----------------------------------------------------------------------

void ElementEditWidget::insertElementNameItem(bool primary) {
  const int row = m_table->rowCount();
  m_table->setRowCount(row + 1);

  QString name =
      m_elementName +
      QString(" (%1)").arg(primary ? "selected element"
                                   : "\"" + m_parentElementName + "\" child");
  QTableWidgetItem *header = new QTableWidgetItem(name);
  header->setFlags(Qt::NoItemFlags);

  /* Switch foreground and background colours. */
  QBrush background(style()->standardPalette().brush(QPalette::WindowText));
  QBrush foreground(style()->standardPalette().brush(QPalette::Window));
  header->setBackground(background);
  header->setForeground(foreground);

  m_table->setItem(row, intFromEnum(Columns::Attribute), header);
  m_table->setSpan(row, intFromEnum(Columns::Attribute), 1,
                   intFromEnum(Columns::Count));
}

//----------------------------------------------------------------------

void ElementEditWidget::populateTable() {
  foreach (QString attribute, m_associatedAttributes) {
    const bool isAttributeActive = m_element.hasAttribute(attribute);
    const int row = m_table->rowCount();
    m_table->setRowCount(row + 1);

    QTableWidgetItem *attributeItem = new QTableWidgetItem(attribute);
    attributeItem->setFlags((attributeItem->flags() | Qt::ItemIsUserCheckable) ^
                            Qt::ItemIsEditable);
    attributeItem->setCheckState(isAttributeActive ? Qt::Checked
                                                   : Qt::Unchecked);
    m_table->setItem(row, intFromEnum(Columns::Attribute), attributeItem);

    /* Table takes ownership through setItem */
    ValueComboBox *valueCombo = new ValueComboBox(row);
    valueCombo->setEnabled(isAttributeActive);
    valueCombo->setEditable(true);

    DB db;
    connect(&db, SIGNAL(result(DB::Result, QString)), this,
            SLOT(processResult(DB::Result, QString)));

    QStringList values = db.attributeValues(
        attribute, m_elementName, m_parentElementName, m_documentRoot);
    values.sort();

    QStringListModel *listModel = new QStringListModel(values, valueCombo);
    valueCombo->setModel(listModel);
    valueCombo->setCurrentText(m_element.attribute(attribute));

    connect(valueCombo, SIGNAL(valueChanged(int)), this,
            SLOT(attributeValueChanged(int)));
    // Takes ownership.
    m_table->setCellWidget(row, intFromEnum(Columns::Value), valueCombo);
  }
}

//----------------------------------------------------------------------

ValueComboBox *ElementEditWidget::valueComboForRow(int row) {
  return dynamic_cast<ValueComboBox *>(
      m_table->cellWidget(row, intFromEnum(Columns::Value)));
}

//----------------------------------------------------------------------

QTableWidgetItem *ElementEditWidget::itemForRow(int row) {
  return m_table->item(row, intFromEnum(Columns::Attribute));
}

//----------------------------------------------------------------------