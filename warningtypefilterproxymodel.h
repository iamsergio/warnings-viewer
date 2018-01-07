/*
   This file is part of warning-viewer.

  Copyright (C) 2018 Sergio Martins <smartins@kde.org>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.

  As a special exception, permission is given to link this program
  with any edition of Qt, and distribute the resulting executable,
  without including the source code for Qt in the source distribution.
*/

#ifndef WARNINGTYPEFILTERPROXYMODEL_H
#define WARNINGTYPEFILTERPROXYMODEL_H

#include "clazychecks.h"

#include <QSortFilterProxyModel>

class WarningTypeFilterProxyModel : public QSortFilterProxyModel
{
public:
    explicit WarningTypeFilterProxyModel(QObject *parent);
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
    void setEnabledClazyCategories(const QStringList &categories);
    void enableClazyCategory(const QString &category, bool enable);

private:
    QStringList m_enabledClazyCategories;
    ClazyChecks m_clazyChecks;
};

#endif
