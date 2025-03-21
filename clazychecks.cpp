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

#include "clazychecks.h"

#include <QFile>
#include <QJsonDocument>
#include <QVariant>
#include <QDebug>

ClazyChecks::ClazyChecks()
{
}

bool ClazyChecks::readChecks()
{
    QFile file(":/checks.json");
    file.open(QIODevice::ReadOnly);
    QByteArray serializedData = file.readAll();
    file.close();

    QJsonParseError jsonError;
    QJsonDocument document = QJsonDocument::fromJson(serializedData, &jsonError);
    if (jsonError.error != QJsonParseError::NoError) {
        qWarning() << "ClazyChecks::readChecks()" << jsonError.errorString();
        return false;
    }

    QVariantMap rootMap = document.toVariant().toMap();
    for (const QVariant &category : rootMap.value("available_categories").toList())
        m_availableCategories.append(category.toString());

    for (const QVariant &c : rootMap.value("checks").toList()) {
        QString name = c.toMap().value("name").toString();
        Q_ASSERT(!name.isEmpty());

        int level = c.toMap().value("level", -1000).toInt();
        Q_ASSERT(level != -1000);

        QStringList categories;

        for (const QVariant &category : c.toMap().value("categories").toList())
            categories.append(category.toString());

        m_checks.append({ level, name, categories });
    }

    return true;
}

ClazyCheck ClazyChecks::checkForName(const QString &name) const
{
    QString simpleName = name;
    simpleName.replace("clazy-", "");

    for (auto c : std::as_const(m_checks)) {
        if (c.name == simpleName)
            return c;
    }

    return ClazyCheck();
}
