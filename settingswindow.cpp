/*
   This file is part of warning-viewer.

  Copyright (C) 2016 Sergio Martins <smartins@kde.org>

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

#include "settingswindow.h"
#include "ui_settings.h"
#include "settings.h"

#include <QDebug>

SettingsWindow::SettingsWindow(Settings *settings, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SettingsDialog())
    , m_settings(settings)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);
    fillForm();
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}

void SettingsWindow::fillForm()
{
    ui->externalEditor->setText(m_settings->externalEditor());
    ui->regexp->setText(m_settings->categoryFilterRegexp());
}

void SettingsWindow::saveForm()
{
    m_settings->setExternalEditor(ui->externalEditor->text());
    m_settings->setCategoryFilterRegexp(ui->regexp->text());
    m_settings->save();
}

void SettingsWindow::accept()
{
    saveForm();
    QDialog::accept();
}
