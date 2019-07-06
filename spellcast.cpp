#include "spellcast.h"

SpellCast::SpellCast(QQuickItem *parent) : QQuickPaintedItem(parent)
{
    setAntialiasing(true);

    m_spellParameters[Template].m_pen = QPen(QColor(Qt::white), 30);
    m_spellParameters[Template].m_backgroundColor = QColor(Qt::white);

    m_spellParameters[Decorative].m_pen = QPen(QColor(50, 50, 50, 5), 12);

    m_spellParameters[Drawing].m_pen = QPen(QColor(255, 255, 255, 200), 12, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    m_spellParameters[Drawing].m_backgroundColor = QColor(Qt::lightGray);

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

void SpellCast::initSpellPath(QPointF point)
{
    point *= m_ratio;
    m_arrPoints.clear();
    m_arrPoints.append(point);

    m_spellStats.reset(ColorCount(Qt::white));
}

void SpellCast::updateSpellPath(QPointF point)
{
    point *= m_ratio;
    const QPointF previousPoint = m_arrPoints.last();
    m_arrPoints.append(point);

    const qreal spellLength = sqrt(pow(point.x() - previousPoint.x(), 2) + pow(point.y() - previousPoint.y(), 2));
    m_spellStats.addSpellDrawingLength(spellLength);

    DrawPath(m_spellTemplate, previousPoint, point, m_spellParameters[Template]);
//    DrawPath(m_spellDrawing, previousPoint, point, m_spellParameters[Drawing]);

    emit update();
}

void SpellCast::finalizeSpellPath()
{
    m_spellStats.complete(ColorCount(Qt::white));

    emit spellStatsChanged();
}

void SpellCast::reset()
{
    LoadSource(QQmlFile::urlToLocalFileOrQrc(m_source));
}

SpellStats *SpellCast::spellStats()
{
    return &m_spellStats;
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

    QImage spellTemplate = m_loadedImage;
    QImage spellDrawing = m_loadedImage;
    AddImageBackground(&spellTemplate, m_spellParameters[Template].m_backgroundColor);
    AddImageBackground(&spellDrawing, m_spellParameters[Drawing].m_backgroundColor);

    m_spellTemplate = spellTemplate;
    m_spellDrawing = spellDrawing;

    m_ratio = m_loadedImage.width() / width();

    if (qFuzzyCompare(m_ratio, (m_loadedImage.height() / height())) == false)
    {
        qInfo() << "image is not a square";
    }

    decorateSpellPath();

    update();
}

void SpellCast::decorateSpellPath()
{
    QPainter painter(&m_spellDrawing);
    painter.setPen(m_spellParameters[Decorative].m_pen);

    for (int y = 0; y < m_spellTemplate.height(); ++y) {
        for (int x = 0; x < m_spellTemplate.width(); ++x) {
            if (m_spellTemplate.pixelColor(x, y).toRgb() != Qt::white)
            {
                painter.drawEllipse({x, y}, 10, 10);
            }
        }
    }
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

void SpellCast::DrawPath(QImage &image, QPointF p0, QPointF p1, SpellParameters parameters)
{
    QPainter painter(&image);
    painter.setPen(parameters.m_pen);

    QPainterPath path;
    path.moveTo(p0);
    path.lineTo(p1);
    painter.drawPath(path);
    painter.end();
}

void SpellCast::AddImageBackground(QImage *image, QColor oColor)
{
    QImage newImage(image->size(), QImage::Format_ARGB32);
    newImage.fill(oColor);
    QPainter painter(&newImage);

    painter.drawImage(QPoint(0, 0), *image);

    *image = newImage;
}
