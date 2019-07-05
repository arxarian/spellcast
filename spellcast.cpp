#include "spellcast.h"

SpellCast::SpellCast(QQuickItem *parent) : QQuickPaintedItem(parent)
{
    setAntialiasing(true);

    connect(this, &QQuickPaintedItem::heightChanged, this, &SpellCast::resizeImage);
    connect(this, &QQuickPaintedItem::widthChanged, this, &SpellCast::resizeImage);
}

void SpellCast::paint(QPainter *painter)
{
    const QRect targetRect(0, 0, static_cast<int>(width()), static_cast<int>(height()));
    painter->drawImage(targetRect, m_spellDrawing);
}

QString SpellCast::source() const
{
    return m_source;
}

QTime SpellCast::time() const
{
    return m_time;
}

qreal SpellCast::completed() const
{
    return m_completed;
}

void SpellCast::initSpellPath(QPointF point)
{
    point *= m_ratio;
    m_arrPoints.clear();
    m_arrPoints.append(point);

    m_nTotal = ColorCount(Qt::white);
}

void SpellCast::updateSpellPath(QPointF point)
{
    point *= m_ratio;
    const QPointF previousPoint = m_arrPoints.last();
    m_arrPoints.append(point);

    DrawPath(m_spellTemplate, previousPoint, point, Qt::white);
    DrawPath(m_spellDrawing, previousPoint, point, Qt::red);

    emit update();
}

void SpellCast::finalizeSpellPath()
{
    m_completed = (1 - ColorCount(Qt::white) / static_cast<qreal>(m_nTotal));

    emit completedChanged(m_completed);
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

void SpellCast::resizeImage()
{
    if (m_loadedImage.isNull()) return;

    AddImageBackground(&m_loadedImage);

    m_spellTemplate =  m_loadedImage;
    m_spellDrawing =  m_loadedImage;

    m_ratio = m_loadedImage.width() / width();

    if (qFuzzyCompare(m_ratio, (m_loadedImage.height() / height())) == false)
    {
        qInfo() << "image is not a square";
    }

    update();
}

bool SpellCast::LoadSource(const QString &path)
{
    if (!m_loadedImage.load(path))
        return false;

    resizeImage();

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
