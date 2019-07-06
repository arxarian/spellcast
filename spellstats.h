#ifndef SPELLSTATS_H
#define SPELLSTATS_H

#include <QObject>
#include <QElapsedTimer>

class SpellStats : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qint32 time READ time WRITE setTime)
    Q_PROPERTY(qreal covered READ covered WRITE setCovered)
    Q_PROPERTY(qreal penalty READ penalty WRITE setPenalty)

public:
    explicit SpellStats(QObject *parent = nullptr);

    qint32 time() const;
    qreal covered() const;
    qreal penalty() const;

    void reset(qreal spellTempleteLength);
    void complete(qreal spellTempleteLength);
    void addSpellDrawingLength(qreal spellDrawingLength);

public slots:
    void setTime(qint32 time_ms);
    void setCovered(qreal covered);
    void setPenalty(qreal penalty);

private:
    qint32 m_time_ms = 0;
    qreal m_covered = 0.0;
    qreal m_penalty = 0.0;

    qreal m_spellTemplateLength = 0.0;
    qreal m_spellDrawingLength = 0.0;

    QElapsedTimer m_timer;
};

#endif // SPELLSTATS_H
