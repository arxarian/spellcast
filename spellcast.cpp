#include "spellcast.h"

SpellCast::SpellCast(QQuickItem *parent) : QQuickPaintedItem(parent)
{
    setAntialiasing(true);
}

void SpellCast::paint(QPainter *painter)
{
    const QRect rect(0, 0, static_cast<int>(width()), static_cast<int>(height()));
    painter->drawImage(QPoint(0, 0), m_spellDrawing);
}

QString SpellCast::source() const
{
    return m_source;
}

void SpellCast::initSpellPath(QPointF point)
{
    m_arrPoints.clear();
    m_arrPoints.append(point);

    m_nTotal = ColorCount(Qt::white);
}

void SpellCast::updateSpellPath(QPointF point)
{
    const QPointF previousPoint = m_arrPoints.last();
    m_arrPoints.append(point);

    DrawPath(m_spellTemplate, previousPoint, point, Qt::white);
    DrawPath(m_spellDrawing, previousPoint, point, Qt::red);

    emit update();
}

void SpellCast::finalizeSpellPath()
{
    qInfo() << "cover" << (1 - ColorCount(Qt::white) / static_cast<qreal>(m_nTotal));
}

void SpellCast::setSource(QString source)
{
    if (m_source == source)
        return;

    m_source = source;

    const QString path = QQmlFile::urlToLocalFileOrQrc(m_source);

    LoadSource(path);

    emit sourceChanged(m_source);
}

bool SpellCast::LoadSource(const QString &path)
{
    QImage loadedImage;
    if (!loadedImage.load(path))
        return false;

    qDebug() << "size" << loadedImage.size();
    AddImageBackground(&loadedImage);
    m_spellTemplate = loadedImage;
    m_spellDrawing = loadedImage;

    update();

    return true;
}

qint32 SpellCast::ColorCount(QColor oColor)
{
    qint32 nCount = 0;

    for (int y = 0; y < m_spellTemplate.height(); ++y) {
        for (int x = 0; x < m_spellTemplate.width(); ++x) {
            if (m_spellTemplate.pixelColor(x, y).toRgb() != oColor) {
                ++nCount;
            }
        }
    }

    return nCount;
}

void SpellCast::DrawPath(QImage &image, QPointF p0, QPointF p1, QColor color)
{
    QPainter painter(&image);
    painter.setPen(QPen(color, 12));

    QPainterPath path;
    path.moveTo(p0);
    path.lineTo(p1);
    painter.drawPath(path);
    painter.end();
}

void SpellCast::AddImageBackground(QImage *image, QColor oColor)
{
    QImage newImage(image->size(), QImage::Format_RGB32);
    newImage.fill(oColor);
    QPainter painter(&newImage);

    painter.drawImage(QPoint(0, 0), *image);

    *image = newImage;
}
