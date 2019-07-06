#include "spellstats.h"
#include <QDebug>

SpellStats::SpellStats(QObject *parent) : QObject(parent)
{
    //
}

qint32 SpellStats::time() const
{
    return m_time_ms;
}

qreal SpellStats::covered() const
{
    return m_covered;
}

qreal SpellStats::penalty() const
{
    return m_penalty;
}

void SpellStats::reset(qreal spellTempleteLength)
{
    m_spellDrawingLength = 0;
    m_spellTemplateLength = spellTempleteLength;

    m_time_ms = 0;
    m_covered = 0;
    m_penalty = 0;

    m_timer.start();
}

void SpellStats::complete(qreal spellTempleteLength)
{
    m_covered = (1 - spellTempleteLength/ m_spellTemplateLength);
    m_time_ms = static_cast<qint32>(m_timer.elapsed());

    qreal penalty = m_spellDrawingLength / m_spellTemplateLength - 1.0;
    m_penalty = qMin(qMax(0.0, penalty * 5), 10.0);
}

void SpellStats::addSpellDrawingLength(qreal spellDrawingLength)
{
    m_spellDrawingLength += spellDrawingLength;
}

void SpellStats::setTime(qint32 time_ms)
{
    m_time_ms = time_ms;
}

void SpellStats::setCovered(qreal covered)
{
    m_covered = covered;
}

void SpellStats::setPenalty(qreal penalty)
{
    m_penalty = penalty;
}
