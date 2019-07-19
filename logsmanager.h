
#pragma once

#include <QObject>

class LogsManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString text READ text NOTIFY textChanged)

public:
    explicit LogsManager(QObject *parent = nullptr);

    Q_INVOKABLE void append(QString text);
    Q_INVOKABLE void clear();

    QString text() const;

private:
    QString m_text;

signals:
    void textChanged();
};
