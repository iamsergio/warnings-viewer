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

#include "warningtypefilterproxymodel.h"
#include <QDebug>

WarningTypeFilterProxyModel::WarningTypeFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    m_clazyChecks.readChecks();
    setEnabledClazyCategories(m_clazyChecks.categories());
}

static bool isClazyWarning(const QString &name)
{
    return name.startsWith("clazy-");
}

bool WarningTypeFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &) const
{
    QString warningType = sourceModel()->data(sourceModel()->index(source_row, 0), Qt::DisplayRole).toString();
    if (!isClazyWarning(warningType))
        return true;

    ClazyCheck check = m_clazyChecks.checkForName(warningType);
    if (!check.isValid()) {
        qWarning() << "Unkown clazy check" << warningType << ". Update checks.json";
        return true;
    }

    for (const QString &category : check.categories) {
        if (m_enabledClazyCategories.contains(category))
            return true;
    }

    return false;
}

void WarningTypeFilterProxyModel::setEnabledClazyCategories(const QStringList &categories)
{
    if (categories != m_enabledClazyCategories) {
        m_enabledClazyCategories = categories;
        invalidateFilter();
    }
}

void WarningTypeFilterProxyModel::enableClazyCategory(const QString &category, bool enable)
{
    if (enable && !m_enabledClazyCategories.contains(category)) {
        m_enabledClazyCategories.append(category);
        invalidateFilter();
    } else if (!enable) {
        m_enabledClazyCategories.removeAll(category);
        invalidateFilter();
    }
}
