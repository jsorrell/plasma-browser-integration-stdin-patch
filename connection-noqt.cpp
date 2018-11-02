/*
    Copyright (C) 2017 by Kai Uwe Broulik <kde@privat.broulik.de>
    Copyright (C) 2017 by David Edmundson <davidedmundson@kde.org>

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/

#include "connection.h"
#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSocketNotifier>

#include <QDebug>
#include <unistd.h>
#include <iostream>

Connection::Connection() : QObject()
{
    m_stdOut.open(STDOUT_FILENO, QIODevice::WriteOnly);
    StdinThread *stdinThread = new StdinThread(this);
    stdinThread->start();
}

void Connection::sendData(const QJsonObject &data)
{
    const QByteArray rawData = QJsonDocument(data).toJson(QJsonDocument::Compact);
    //note, don't use QDataStream as we need to control the binary format used
    quint32 len = rawData.count();
    m_stdOut.write((char*)&len, sizeof(len));
    m_stdOut.write(rawData);
    m_stdOut.flush();
}

Connection* Connection::self()
{
    static Connection *s = nullptr;
    if (!s) {
        s = new Connection();
    }
    return s;
}

StdinThread::StdinThread(Connection *con) : QThread()
{
    this->con = con;
}

void StdinThread::run() {
    while (1){
        quint32 length = 0;

        /* Read Length */
        for (int i = 0; i < 4; i++)
        {
            quint32 read_char = getchar();
            length = length | (read_char << i*8);
        }

        /* Read Data */
        QByteArray data;
        for (quint32 i = 0; i < length; i++)
        {
            char c = getchar();
            data.append(c);
        }

        const QJsonObject json = QJsonDocument::fromJson(data).object();
        emit this->con->dataReceived(json);
    }
}
