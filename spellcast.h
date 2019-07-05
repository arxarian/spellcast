#ifndef SPELLCAST_H
#define SPELLCAST_H

#include <QtQuick>

class SpellCast : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)

public:
    explicit SpellCast(QQuickItem *parent = nullptr);
    void paint(QPainter *painter) override;

    QString source() const;

public slots:
    void setSource(QString source);

private:
    bool LoadSource(const QString& path);

    QImage m_spellTemplate;
    QImage m_spellDrawing;
    QString m_source;

signals:
    void sourceChanged(QString source);
};

#endif // SPELLCAST_H
