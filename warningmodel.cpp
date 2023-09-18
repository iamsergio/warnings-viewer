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

#include "warningmodel.h"
#include "warning.h"

#include <QFile>
#include <QRegularExpression>
#include <QDebug>

WarningModel::WarningModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    connect(this, &WarningModel::rowsInserted, this, &WarningModel::countChanged);
    connect(this, &WarningModel::rowsRemoved, this, &WarningModel::countChanged);
    connect(this, &WarningModel::modelReset, this, &WarningModel::countChanged);
    connect(this, &WarningModel::layoutChanged, this, &WarningModel::countChanged);
}

WarningModel::~WarningModel()
{
}

int WarningModel::columnCount(const QModelIndex &) const
{
    return ColumnCount;
}

int WarningModel::rowCount(const QModelIndex & /*parent*/) const
{
    return m_warnings.count();
}

QVariant WarningModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_warnings.count())
        return QVariant();

    const int col = index.column();
    const Warning &warning = m_warnings.at(index.row());

    if (role == Qt::DisplayRole) {
        if (col == ShortFilenameColumn) {
            return warning.shortFileName() + QStringLiteral(":%1:%2").arg(warning.lineNumber()).arg(warning.columnNumber());
        } else if (col == TextColumn) {
            return warning.m_text;
        } else if (col == WarningNameColumn) {
            return warning.m_warningName;
        }
    } else if (role == Qt::ToolTipRole) {
        return warning.m_completeText;
    } else if (role == WarningNameRole) {
        return warning.m_warningName;
    } else if (role == WarningRole) {
        return QVariant::fromValue<Warning>(warning);
    }

    return QVariant();
}

QVariant WarningModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal)
        return {};

    if (role == Qt::DisplayRole) {
        switch (section) {
        case ShortFilenameColumn:
            return tr("file name");
        case TextColumn:
            return "description";
        case WarningNameColumn:
            return "warning";
        }
    }

    return QAbstractTableModel::headerData(section, orientation, role);
}

int WarningModel::count() const
{
    return m_warnings.count();
}

bool WarningModel::loadFile(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        emit loadFinished(false, tr("Couldn't open file %1").arg(filename));
        return false;
    }

    beginResetModel();
    QRegularExpression re("(.*)warning:(.*)\\[-W(.*)\\].*");
    QSet<QByteArray> uniqueWarnings;
    QHash<QString, Warning> uniqueWarningsHash;
    while (!file.atEnd()) {
        uniqueWarnings.insert(file.readLine());
    }

    foreach (const QByteArray &warning, uniqueWarnings) {
        const QString warningStr = QString::fromUtf8(warning);
        QRegularExpressionMatch match = re.match(warningStr);
        if (match.hasMatch()) {
            Warning warn;
            warn.m_completeText = match.captured(0);
            const QStringList filenameTokens = match.captured(1).trimmed().split(':');
            warn.setFilename(filenameTokens.first());

            // filenameTokens is for example { "file.cpp", 10, 1 }
            if (filenameTokens.size() > 1) {
                bool ok = false;
                int line = filenameTokens[1].toInt(&ok);
                if (ok) {
                    warn.setLineNumber(line);
                    if (filenameTokens.size() > 2) {
                        int column = filenameTokens[2].toInt(&ok);
                        if (ok)
                            warn.setColumnNumber(column);
                    }
                }
            }

            if (warn.filename().isEmpty())
                continue;

            warn.m_text = match.captured(2);
            warn.m_warningName = match.captured(3);
            uniqueWarningsHash.insert(warn.toString(), warn);
        }
    }

    m_warnings.clear();
    m_warnings.reserve(uniqueWarningsHash.size());
    for (const auto &warning : uniqueWarningsHash)
        m_warnings.push_back(warning);

    endResetModel();

    qDebug() << "Loaded " << m_warnings.size() << " unfiltered warnings from" << filename;
    emit loadFinished(true, QString());
    return true;
}
