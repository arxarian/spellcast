#ifndef SPELLCAST_H
#define SPELLCAST_H

#include <QtQuick>

class SpellCast : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QTime time READ time NOTIFY timeChanged)
    Q_PROPERTY(qreal completed READ completed NOTIFY completedChanged)
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)

public:
    explicit SpellCast(QQuickItem *parent = nullptr);
    void paint(QPainter *painter) override;

    QString source() const;
    QTime time() const;
    qreal completed() const;

    Q_INVOKABLE void initSpellPath(QPointF point);
    Q_INVOKABLE void updateSpellPath(QPointF point);
    Q_INVOKABLE void finalizeSpellPath();

    Q_INVOKABLE void reset();

public slots:
    void setSource(QString source);
    void resizeImage();

private:
    bool LoadSource(const QString& path);
    qint32 ColorCount(QColor oColor);
    void DrawPath(QImage &image, QPointF p0, QPointF p1, QColor color);
    void AddImageBackground(QImage* image, QColor oColor = Qt::white);

    QImage m_spellTemplate;
    QImage m_spellDrawing;
    QImage m_loadedImage;
    QString m_source;

    qint32 m_nTotal = 0;
    QList<QPointF> m_arrPoints;

    qreal m_ratio = 0;

    QTime m_time;
    qreal m_completed;

signals:
    void sourceChanged(QString source);
    void completedChanged(qreal completed);
    void timeChanged(QTime time);
};

#endif // SPELLCAST_H
