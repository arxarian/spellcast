#ifndef SPELLCAST_H
#define SPELLCAST_H

#include <QtQuick>

struct SpellParameters
{
    QPen m_pen;
};

class SpellCast : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(qint32 time READ time NOTIFY timeChanged)
    Q_PROPERTY(qreal completed READ completed NOTIFY completedChanged)
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)

public:
    explicit SpellCast(QQuickItem *parent = nullptr);
    void paint(QPainter *painter) override;

    enum SpellType
    {
        Template, Decorative, User
    };

    QString source() const;
    qint32 time() const;
    qreal completed() const;

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

    qint32 m_nTotal = 0;
    QList<QPointF> m_arrPoints;

    qreal m_ratio = 0;

    qint32 m_time_ms;
    QElapsedTimer m_timer;
    qreal m_completed;

signals:
    void sourceChanged(QString source);
    void completedChanged(qreal completed);
    void timeChanged(qint32 time);
};

#endif // SPELLCAST_H
