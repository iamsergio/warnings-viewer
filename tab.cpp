/*
   This file is part of warning-viewer.

  Copyright (C) 2015-2016 Sergio Martins <smartins@kde.org>

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

#include "tab.h"
#include "warningmodel.h"
#include "warningproxymodel.h"

#include <QTableView>
#include <QTableView>
#include <QVBoxLayout>
#include <QTableView>

Tab::Tab(const QString &filename, QWidget *parent)
    : QWidget(parent)
    , m_model(new WarningModel(this))
    , m_proxyModel(new WarningProxyModel(this))
    , m_tableView(new QTableView())
{
    m_model->loadFile(filename);

    QVBoxLayout *l = new QVBoxLayout(this);
    l->addWidget(m_tableView);

    m_tableView->setSortingEnabled(true);
    m_tableView->setSelectionBehavior(QAbstractItemView::SelectItems);
    m_proxyModel->setSourceModel(m_model);
    m_tableView->setModel(m_proxyModel);
}

QTableView *Tab::tableView() const
{
    return m_tableView;
}

WarningProxyModel* Tab::proxyModel() const
{
    return m_proxyModel;
}

WarningModel* Tab::model() const
{
    return m_model;
}
