#include "spellprovider.h"

#include <QDir>

SpellProvider::SpellProvider(QObject *parent) : QObject(parent)
{
    QDir spellDir(m_spellLocalDirPath);
    m_spellSources = spellDir.entryList(QStringList("*.svg"));
    m_spellSource = getCurrentSpellSource();
}

QString SpellProvider::spellSource() const
{
    return m_spellSource;
}

void SpellProvider::nextSpell()
{
    m_currentSpellIndex = (m_currentSpellIndex + 1) % m_spellSources.count();
    m_spellSource = getCurrentSpellSource();

    emit spellSourceChanged(m_spellSource);
}

QString SpellProvider::getCurrentSpellSource()
{
    return m_spellQmlDirPath + m_spellSources[m_currentSpellIndex];
}
