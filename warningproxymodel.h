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

#ifndef WARNINGPROXYMODEL_H
#define WARNINGPROXYMODEL_H

#include "warningmodel.h"
#include "settings.h"

#include <QSortFilterProxyModel>

class WarningProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit WarningProxyModel(WarningModel *model, Settings *settings, QObject *parent = nullptr);
    void setAcceptedWarningTypes(const QSet<QString> &warningTypes);
    void setText(const QString &filter);

    QSet<QString> availableWarnings() const;
    void setAvailableWarningFilterRegex(const QString &regex);
    void setSourceModel(QAbstractItemModel *model) override;

signals:
    void countChanged();
    void availableWarningsChanged(int numAvailableWarningTypes);

private Q_SLOTS:
    void onSourceModelLoaded(bool success, const QString &errorMsg);

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    bool isAcceptedWarning(const QString &warning);
    void calculateAvailableWarnings();
    Settings *const m_settings;
    QSet<QString> m_acceptedWarnings;
    QSet<QString> m_availableWarnings;
    QString m_text;
    QString m_availableWarningFilterRegex;
};

#endif
