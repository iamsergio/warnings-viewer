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

#ifndef VIEWER_MAIN_WINDOW_H
#define VIEWER_MAIN_WINDOW_H

#include "ui_mainwindow.h"
#include "settings.h"
#include "tab.h"

#include <QMainWindow>
#include <QPointer>

namespace Ui {
class MainWindow;
}

class WarningModel;
class WarningProxyModel;
class QTableView;
class SettingsWindow;

class MainWindow : public QMainWindow
{
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void openLog(const QString &filename);
protected:
    void resizeEvent(QResizeEvent *) override;
    void contextMenuEvent(QContextMenuEvent *ev) override;

private Q_SLOTS:
    void askOpenLog();
    void updateCategoryView();
    void selectAllCategories();
    void unselectAllCategories();
    void filterByCategory();
    void filterByText();
    void updateStatusBar();
    void copyCell();
    void openCellInEditor();
    void onTabChanged();
    void closeTab(int index);
    void openSettings();
    void onCategoryFilterChanged(const QString &);
private:
    void selectFirstCategory();
    void resizeColumnsToContents();
    QTableView *currentTableView() const;
    WarningModel *currentModel() const;
    WarningProxyModel *currentProxyModel() const;
    Tab* currentTab() const;
    QModelIndex selectedIndex() const;
    Ui::MainWindow *const ui;
    QPointer<SettingsWindow> m_settingsWindow;
    Settings m_settings;
    Tab::List m_tabs; // Visible and invisible tabs
};

#endif
