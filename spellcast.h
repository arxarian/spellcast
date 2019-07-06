#ifndef SPELLCAST_H
#define SPELLCAST_H

#include <QtQuick>

#include "spellstats.h"

struct SpellParameters
{
    QPen m_pen;
    QColor m_backgroundColor;
};

class SpellCast : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(SpellStats* spellStats READ spellStats NOTIFY spellStatsChanged)

public:
    explicit SpellCast(QQuickItem *parent = nullptr);
    void paint(QPainter *painter) override;

    enum SpellType
    {
        Template, Decorative, Drawing
    };

    QString source() const;
    SpellStats* spellStats();

    Q_INVOKABLE void initSpellPath(QPointF point);
    Q_INVOKABLE void updateSpellPath(QPointF point);
    Q_INVOKABLE void finalizeSpellPath();

    Q_INVOKABLE void reset();


public slots:
    void setSource(QString source);
    void resizeImage();
    void decorateSpellPath();

private:
    bool LoadSource(const QString& path);
    qint32 ColorCount(QColor oColor);
    void DrawPath(QImage &image, QPointF p0, QPointF p1, SpellParameters parameters);
    void AddImageBackground(QImage* image, QColor oColor = Qt::white);

    SpellParameters m_spellParameters[3];

    QImage m_spellTemplate;
    QImage m_spellDrawing;
    QImage m_loadedImage;
    QString m_source;

    QList<QPointF> m_arrSpellPath;

    qreal m_ratio = 0;

    SpellStats m_spellStats;

signals:
    void sourceChanged(QString source);
    void spellStatsChanged();
};

#endif // SPELLCAST_H
