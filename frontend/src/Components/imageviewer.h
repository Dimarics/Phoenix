#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QQuickPaintedItem>
#include <QImage>

class ImageViewer : public QQuickPaintedItem
{
    Q_OBJECT
    QML_ELEMENT
public:
    ImageViewer(QQuickItem *parent = nullptr);
    void setImageFromData(const QByteArray &data);

private:
    QImage m_image;
    void paint(QPainter *painter) override;
};

#endif // IMAGEVIEWER_H
