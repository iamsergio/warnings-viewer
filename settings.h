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

#ifndef WARNING_VIEWER_SETTINGS_H
#define WARNING_VIEWER_SETTINGS_H

#include <QSettings>

class Settings : public QObject
{
    Q_OBJECT
public:
    explicit Settings(QObject *parent = nullptr);
    void save();

    void setCategoryFilterRegexp(const QString &);
    QString categoryFilterRegexp() const;

    void setExternalEditor(const QString &);
    QString externalEditor() const;

signals:
    void categoryFilterRegexpChanged(const QString &);

private:
    QSettings m_settings;
};

#endif
