#ifndef SPELLSTATS_H
#define SPELLSTATS_H

#include <QObject>
#include <QElapsedTimer>

class SpellStats : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qint32 timeComplete READ timeComplete WRITE setTimeComplete)
    Q_PROPERTY(qint32 time READ time WRITE setTime)
    Q_PROPERTY(qreal covered READ covered WRITE setCovered)
    Q_PROPERTY(qreal penalty READ penalty WRITE setPenalty)

public:
    explicit SpellStats(QObject *parent = nullptr);

    qint32 timeComplete() const;
    qint32 time() const;
    qreal covered() const;
    qreal penalty() const;

    void resetCompleteTime();
    void reset(qreal spellTempleteLength);
    void complete(qreal spellTempleteLength);
    void addSpellDrawingLength(qreal spellDrawingLength);


public slots:
    void setTimeComplete(qint32 timeComplete_ms);
    void setTime(qint32 time_ms);
    void setCovered(qreal covered);
    void setPenalty(qreal penalty);

private:
    qint32 m_timeComplete_ms = 0;
    qint32 m_time_ms = 0;
    qreal m_covered = 0.0;
    qreal m_penalty = 0.0;

    qreal m_spellTemplateLength = 0.0;
    qreal m_spellDrawingLength = 0.0;

    QElapsedTimer m_timer_spellCast;
    QElapsedTimer m_timer_complete;
};

#endif // SPELLSTATS_H
