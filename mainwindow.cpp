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
#include "tab.h"

#include <QApplication>
#include <QFileDialog>
#include <QString>
#include <QListWidgetItem>
#include <QItemSelectionModel>
#include <QMenu>
#include <QCursor>
#include <QClipboard>
#include <QProcess>
#include <QTableView>
#include <QFileInfo>

#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow())
{
    ui->setupUi(this);
    connect(ui->actionQuit, &QAction::triggered, qApp, &QApplication::quit);
    connect(ui->actionOpen_Log, &QAction::triggered, this, &MainWindow::askOpenLog);
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::selectAllCategories);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::unselectAllCategories);
    connect(ui->filterListWidget->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::filterByCategory);
    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &MainWindow::onTabChanged);
    connect(ui->tabWidget, &QTabWidget::tabCloseRequested, this, &MainWindow::closeTab);
    connect(ui->filterLineEdit, &QLineEdit::textChanged, this, &MainWindow::filterByText);

    ui->filterListWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);

    setWindowTitle("warnings-viewer");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *ev)
{
    QMainWindow::resizeEvent(ev);
    resizeColumnsToContents();
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
    QFileInfo finfo(filename);
    if (!finfo.exists() || !finfo.isFile())
        return;

    auto tab = new Tab(filename);
    if (tab->model()->rowCount({}) > 0) {
        ui->tabWidget->addTab(tab, finfo.fileName());
        connect(tab->model(), &WarningModel::categoriesChanged, this, &MainWindow::updateCategoryFilter);
        connect(tab->proxyModel(), &WarningProxyModel::countChanged, this, &MainWindow::updateStatusBar);
    } else {
        tab->deleteLater();
        std::cout << "File does not contain any warnings (" << finfo.fileName().toStdString() << ")" << std::endl;
    }
}

void MainWindow::updateCategoryFilter()
{
    WarningModel *model = currentModel();
    if (!model)
        return;

    ui->filterListWidget->clear();
    foreach (const QString &category, model->categories()) {
         auto item = new QListWidgetItem(category, ui->filterListWidget);
         item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
    }
    resizeColumnsToContents();
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
    WarningProxyModel *proxy = currentProxyModel();
    if (!proxy)
        return;

    QSet<QString> categories;
    const QModelIndexList indexes = ui->filterListWidget->selectionModel()->selectedIndexes();
    foreach (const auto &index, indexes) {
        categories.insert(index.data(Qt::DisplayRole).toString());
    }

    proxy->setCategories(categories);
    resizeColumnsToContents();
}

void MainWindow::filterByText()
{
    if (WarningProxyModel *proxy = currentProxyModel())
        proxy->setText(ui->filterLineEdit->text());
}

void MainWindow::updateStatusBar()
{
    if (WarningProxyModel *proxy = currentProxyModel())
        statusBar()->showMessage(QString("showing %1 warnings").arg(proxy->rowCount({})));
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
    QTableView *tableView = currentTableView();
    if (tableView) {
        const auto indexes = tableView->selectionModel()->selectedIndexes();
        return indexes.isEmpty() ? QModelIndex() : indexes.first();
    }

    return QModelIndex();
}

void MainWindow::resizeColumnsToContents()
{
    QTableView *table = currentTableView();
    if (table)
        table->resizeColumnsToContents();
}

QTableView * MainWindow::currentTableView() const
{
    Tab *tab = currentTab();
    return tab ? tab->tableView() : nullptr;
}

Tab* MainWindow::currentTab() const
{
    return qobject_cast<Tab*>(ui->tabWidget->currentWidget());
}

WarningModel *MainWindow::currentModel() const
{
    Tab *tab = currentTab();
    return tab ? tab->model() : nullptr;
}

WarningProxyModel *MainWindow::currentProxyModel() const
{
    Tab *tab = currentTab();
    return tab ? tab->proxyModel() : nullptr;
}

void MainWindow::onTabChanged()
{
    filterByText();
    updateCategoryFilter();
    updateStatusBar();
    selectFirstCategory();
}

void MainWindow::selectFirstCategory()
{
    auto model = ui->filterListWidget->model();
    if (model->rowCount() > 0)
        ui->filterListWidget->selectionModel()->select(model->index(0, 0), QItemSelectionModel::Select);
}

void MainWindow::closeTab(int index)
{
    delete ui->tabWidget->widget(index);
}
