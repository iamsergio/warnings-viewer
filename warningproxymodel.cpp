/*
   This file is part of warning-viewer.

  Copyright (C) 2015 Sergio Martins <smartins@kde.org>

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

#include "warningproxymodel.h"

WarningProxyModel::WarningProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    connect(this, &WarningProxyModel::rowsInserted, this, &WarningProxyModel::countChanged);
    connect(this, &WarningProxyModel::rowsRemoved, this, &WarningProxyModel::countChanged);
    connect(this, &WarningProxyModel::modelReset, this, &WarningProxyModel::countChanged);
    connect(this, &WarningProxyModel::layoutChanged, this, &WarningProxyModel::countChanged);
}

bool WarningProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    if (source_parent.isValid())
        return false;

    if (m_categories.isEmpty())
        return false;

    const QModelIndex sourceIndex = sourceModel()->index(source_row, 0);
    const Warning warn = sourceIndex.data(WarningModel::WarningRole).value<Warning>();
    const QString category = warn.m_category;
    if (!m_categories.isEmpty() && !m_categories.contains(category))
        return false;

    if (!m_text.isEmpty() && !warn.m_completeText.toLower().contains(m_text.toLower()))
        return false;

    return true;
}

void WarningProxyModel::setCategories(const QSet<QString> &categories)
{
    if (categories != m_categories) {
        m_categories = categories;
        invalidateFilter();
    }
}

void WarningProxyModel::setText(const QString &filter)
{
    if (filter != m_text) {
        m_text = filter;
        invalidateFilter();
    }
}
