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

#include "mainwindow.h"
#include "warningmodel.h"
#include "warningproxymodel.h"

#include <QApplication>
#include <QFileDialog>
#include <QString>
#include <QListWidgetItem>
#include <QItemSelectionModel>
#include <QMenu>
#include <QCursor>
#include <QClipboard>
#include <QProcess>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow())
    , m_model(new WarningModel(this))
    , m_proxyModel(new WarningProxyModel(this))
{
    ui->setupUi(this);
    connect(ui->actionQuit, &QAction::triggered, qApp, &QApplication::quit);
    connect(ui->actionOpen_Log, &QAction::triggered, this, &MainWindow::askOpenLog);
    connect(m_model, &WarningModel::categoriesChanged, this, &MainWindow::updateCategoryFilter);
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::selectAllCategories);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::unselectAllCategories);
    connect(ui->filterListWidget->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::filterByCategory);
    connect(ui->filterLineEdit, &QLineEdit::textChanged, this, &MainWindow::filterByText);
    connect(m_proxyModel, &WarningProxyModel::countChanged, this, &MainWindow::updateStatusBar);

    m_proxyModel->setSourceModel(m_model);
    ui->tableView->setModel(m_proxyModel);
    ui->filterListWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);

    m_model->loadFile("/tmp/make-qtbase.log");
    setWindowTitle("warnings-viewer");
    ui->tableView->setSortingEnabled(true);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectItems);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *ev)
{
    QMainWindow::resizeEvent(ev);
    ui->tableView->resizeColumnsToContents();
}

void MainWindow::contextMenuEvent(QContextMenuEvent *)
{
    QMenu menu(this);
    QAction *copy = menu.addAction("Copy");

    QModelIndex index = selectedIndex();
    Warning warn = index.data(WarningModel::WarningRole).value<Warning>();
    if (warn.pathIsAbsolute()) {
        QAction *open = menu.addAction("Open");
        connect(open, &QAction::triggered, this, &MainWindow::openCellInEditor);
    }

    connect(copy, &QAction::triggered, this, &MainWindow::copyCell);
    menu.exec(QCursor::pos());
}

void MainWindow::askOpenLog()
{
    const QString filename = QFileDialog::getOpenFileName(this, tr("Open log"));
    if (!filename.isEmpty())
        openLog(filename);
}

void MainWindow::openLog(const QString &filename)
{
    m_model->loadFile(filename);
}

void MainWindow::updateCategoryFilter()
{
    ui->filterListWidget->clear();
    foreach (const QString &category, m_model->categories()) {
         auto item = new QListWidgetItem(category, ui->filterListWidget);
         item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
    }
    ui->tableView->resizeColumnsToContents();
}

void MainWindow::selectAllCategories()
{
    ui->filterListWidget->selectAll();
}

void MainWindow::unselectAllCategories()
{
    ui->filterListWidget->clearSelection();
}

void MainWindow::filterByCategory()
{
    QSet<QString> categories;
    const QModelIndexList indexes = ui->filterListWidget->selectionModel()->selectedIndexes();
    foreach (const auto &index, indexes) {
        categories.insert(index.data(Qt::DisplayRole).toString());
    }

    m_proxyModel->setCategories(categories);
    ui->tableView->resizeColumnsToContents();
}

void MainWindow::filterByText()
{
    m_proxyModel->setText(ui->filterLineEdit->text());
}

void MainWindow::updateStatusBar()
{
    statusBar()->showMessage(QString("showing %1 warnings").arg(m_proxyModel->rowCount({})));
}

void MainWindow::copyCell()
{
    QModelIndex index = selectedIndex();
    if (!index.isValid())
        return;

    QClipboard *clipboard = QApplication::clipboard();
    const QString text = index.data(Qt::DisplayRole).toString();
    clipboard->setText(text, QClipboard::Clipboard);
    clipboard->setText(text, QClipboard::Selection);
}

void MainWindow::openCellInEditor()
{
    QModelIndex index = selectedIndex();
    if (!index.isValid())
        return;
    Warning warn = index.data(WarningModel::WarningRole).value<Warning>();
    QProcess::startDetached("/data/bin/kf.py", { warn.filename() });
}

QModelIndex MainWindow::selectedIndex() const
{
    const auto indexes = ui->tableView->selectionModel()->selectedIndexes();
    return indexes.isEmpty() ? QModelIndex() : indexes.first();
}
