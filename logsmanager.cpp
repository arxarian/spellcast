#include "logsmanager.h"

#include <QDateTime>

LogsManager::LogsManager(QObject *parent) : QObject(parent)
{
    //
}

void LogsManager::append(QString text)
{
    m_text.prepend(QString("%1:\n%2\n").arg(QDateTime::currentDateTime().toString()).arg(text));

    emit textChanged();
}

void LogsManager::clear()
{
    m_text.clear();

    emit textChanged();
}

QString LogsManager::text() const
{
    return m_text;
}

