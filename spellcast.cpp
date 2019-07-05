#include "spellcast.h"

SpellCast::SpellCast(QQuickItem *parent) : QQuickPaintedItem(parent)
{
    setAntialiasing(true);
}

void SpellCast::paint(QPainter *painter)
{
    const QRect rect(0, 0, static_cast<int>(width()), static_cast<int>(height()));
    painter->drawImage(rect, m_spellDrawing);
}

QString SpellCast::source() const
{
    return m_source;
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

    m_spellTemplate = loadedImage;
    m_spellDrawing = loadedImage;

    update();

    return true;
}
