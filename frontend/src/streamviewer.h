#ifndef STREAMVIEWER_H
#define STREAMVIEWER_H

#include "imageviewer.h"
#include <QWebSocket>
#ifdef __EMSCRIPTEN__
class StreamViewer : public ImageViewer
{
    Q_OBJECT
    QML_ELEMENT
public:
    StreamViewer(QQuickItem *parent = nullptr);

private:
    QWebSocket *m_webSocket;
};
#else
class StreamViewer : public ImageViewer
{
    Q_OBJECT
    QML_ELEMENT
public:
    StreamViewer(QQuickItem *parent = nullptr);
};
#endif

#endif // STREAMVIEWER_H
