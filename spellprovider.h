#ifndef SPELLPROVIDER_H
#define SPELLPROVIDER_H

#include <QObject>

class SpellProvider : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString spellSource READ spellSource NOTIFY spellSourceChanged)
    QString m_spellSource;

public:
    explicit SpellProvider(QObject *parent = nullptr);
    QString spellSource() const;
    Q_INVOKABLE void nextSpell();

private:
    QString getCurrentSpellSource();

    qint32 m_currentSpellIndex = 0;
    QStringList m_spellSources;
    const QString m_spellLocalDirPath = QString(":/spells/");
    const QString m_spellQmlDirPath = QString("qrc:/spells/");

signals:
    void spellSourceChanged(QString spellSource);

};

#endif // SPELLPROVIDER_H
