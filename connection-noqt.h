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

#pragma once

#include <QObject>
#include <QFile>
#include <QJsonObject>
#include <QThread>

/*
 * This class is responsible for managing all stdout/stdin connections emitting JSON
 */
class Connection : public QObject
{
    Q_OBJECT
public:
    static Connection* self();
    void sendData(const QJsonObject &data);

Q_SIGNALS:
    void dataReceived(const QJsonObject &data);

private:
    Connection();
    ~Connection() = default;
    QFile m_stdOut;
};

class StdinThread : public QThread
{
    Q_OBJECT
public:
    StdinThread(Connection *con);
    void run() override;
Q_SIGNALS:
    void resultReady(const QJsonObject &data);

private:
    ~StdinThread() = default;
    Connection *con;
};
