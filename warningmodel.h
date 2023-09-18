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

#ifndef VIEWER_WARNING_MODEL_H
#define VIEWER_WARNING_MODEL_H

#include "warning.h"

#include <QAbstractListModel>
#include <QList>
#include <QString>
#include <QSet>
#include <QAbstractTableModel>

class WarningModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
public:
    enum Role {
        WarningNameRole = Qt::UserRole + 1,
        WarningRole
    };

    enum Column {
        ShortFilenameColumn,
        TextColumn,
        WarningNameColumn,
        ColumnCount
    };

    explicit WarningModel(QObject *parent = 0);
    ~WarningModel();
    int columnCount(const QModelIndex &parent) const override;
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    int count() const;

    bool loadFile(const QString &filename);

Q_SIGNALS:
    void countChanged();
    void loadFinished(bool success, const QString &errorMessage);

private:
    Warning::List m_warnings;
};

#endif
