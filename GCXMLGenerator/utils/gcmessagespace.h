/* Copyright (c) 2012 by William Hallatt.
 *
 * This file forms part of "GoblinCoding's XML Studio".
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
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program (GNUGPL.txt).  If not, see
 *
 *                    <http://www.gnu.org/licenses/>
 */

#ifndef GCMESSAGESPACE_H
#define GCMESSAGESPACE_H

#include "forms/gcmessagedialog.h"
#include <QVariant>

namespace GCMessageSpace
{
  /* This function will return the saved user preference (if there is one),
    or prompt the user for a decision. */
  bool userAccepted( const QString &uniqueMessageKey,
                     const QString &heading,
                     const QString &text,
                     GCMessageDialog::ButtonCombo buttons,
                     GCMessageDialog::Buttons defaultButton,
                     GCMessageDialog::Icon icon = GCMessageDialog::NoIcon,
                     bool saveCancel = true );

  void forgetAllPreferences();
}

#endif // GCMESSAGESPACE_H