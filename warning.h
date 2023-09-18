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

#ifndef WARNING_VIEWER_WARNING_H
#define WARNING_VIEWER_WARNING_H

#include <QString>
#include <QVector>
#include <QStringList>
#include <QMetaType>

class Warning
{
public:
    typedef QVector<Warning> List;
    QString m_text;
    QString m_warningName;
    QString m_completeText;

    void setFilename(const QString &filename)
    {
        m_filename = filename;

        if (pathIsAbsolute()) {
            m_shortFilename = m_filename;
        } else {
            auto splitted = m_filename.split("/");
            m_shortFilename.clear();
            const int numTokens = splitted.size();
            QStringList newList;
            for (int i = 0; i < 5 && i < numTokens; ++i) {
                newList.prepend(splitted.at(numTokens - i - 1));
            }
            m_shortFilename = newList.join("/");
        }
    }
    QString filename() const
    {
        return m_filename;
    }
    int lineNumber() const
    {
        return m_lineNumber;
    }
    void setLineNumber(int n)
    {
        m_lineNumber = n;
    }
    int columnNumber() const
    {
        return m_columnNumber;
    }
    void setColumnNumber(int n)
    {
        m_columnNumber = n;
    }
    QString shortFileName() const
    {
        return m_shortFilename;
    }
    bool pathIsAbsolute() const
    {
        return m_filename.startsWith("/");
    }

    QString toString() const
    {
        return m_shortFilename + ":" + m_text + ":" + m_warningName;
    }

private:
    QString m_filename;
    QString m_shortFilename;
    int m_lineNumber = -1;
    int m_columnNumber = -1;
};

Q_DECLARE_METATYPE(Warning)

#endif
