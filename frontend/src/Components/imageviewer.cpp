#include "imageviewer.h"
#include <QPainter>

ImageViewer::ImageViewer(QQuickItem *parent) : QQuickPaintedItem(parent)
{
    //setFillColor(Qt::blue);
}

void ImageViewer::setImageFromData(const QByteArray &data)
{
    m_image.loadFromData(data);
    update();
}

void ImageViewer::paint(QPainter *painter)
{
    QImage drawImage(m_image.scaled(width(), height(), Qt::KeepAspectRatio));
    int x = (width() - drawImage.width()) / 2.f;
    int y = (height() - drawImage.height()) / 2.f;
    painter->drawImage(x, y, drawImage);

    //bool ratio_compare = (float)width() / (float)height() >= (float)m_image.width() / (float)m_image.height();
    //int sourceWidth = ratio_compare ? width() : width() * (1.f + 1.f / (float)height());
    //int sourceHeight = ratio_compare ? height() : height() * (1.f + 1.f / (float)width());
    //painter->drawImage(x, y, m_image, 0, 0, sourceWidth, sourceHeight);
}
