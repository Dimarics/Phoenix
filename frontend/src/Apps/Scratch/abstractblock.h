#ifndef ABSTRACTBLOCK_H
#define ABSTRACTBLOCK_H

#include <QQuickItem>
#include <QCursor>
#include <QDrag>
#include <QMimeData>
#include <QGuiApplication>
#include <QQmlContext>

class AbstractBlock : public QQuickItem
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(bool clonable READ clonable WRITE setClonable NOTIFY clonableChanged)
    Q_PROPERTY(Qt::CursorShape cursorShape READ cursorShape WRITE setCursorShape NOTIFY cursorShapeChanged)
    Q_PROPERTY(QQuickItem* canvas READ canvas WRITE setCanvas NOTIFY canvasChanged)
    Q_PROPERTY(QQuickItem* overlay READ overlay WRITE setOverlay NOTIFY overlayChanged)
    Q_PROPERTY(AbstractBlock* dragTarget READ dragTarget WRITE setDragTarget NOTIFY dragTargetChanged)

public:
    AbstractBlock(QQuickItem *parent = nullptr);

    void setClonable(bool clonable) {
        if (clonable == m_clonable) return;
        m_clonable = clonable;
        emit clonableChanged();
    }
    void setCursorShape(Qt::CursorShape shape) {
        if (cursorShape() == shape) return;
        setCursor(QCursor(shape));
        emit cursorShapeChanged();
    }
    void setCanvas(QQuickItem *canvas) {
        if (canvas == m_canvas) return;
        m_canvas = canvas;
        emit canvasChanged();
    }
    void setOverlay(QQuickItem *overlay) {
        if (overlay == m_overlay) return;
        m_overlay = overlay;
        emit overlayChanged();
    }
    void setDragTarget(AbstractBlock *dragTarget) {
        if (m_dragTarget == dragTarget) return;
        m_dragTarget = dragTarget;
        emit dragTargetChanged();
    }

    inline bool clonable() const { return m_clonable; }
    inline Qt::CursorShape cursorShape() const { return cursor().shape(); }
    inline QQuickItem* canvas() { return m_canvas; }
    inline QQuickItem* overlay() { return m_overlay; }
    inline AbstractBlock* dragTarget() { return m_dragTarget; }

protected:
    void mousePressEvent(QMouseEvent *event) override {
        /*if (event->button() == Qt::LeftButton) {
            QDrag *drag = new QDrag(this);
            drag->setDragCursor(QCursor(Qt::ClosedHandCursor).pixmap(), Qt::MoveAction);
            drag->setPixmap(QPixmap(1, 1));

            QMimeData *mimeData = new QMimeData;
            drag->setMimeData(mimeData);

            Qt::DropAction dropAction = drag->exec(Qt::MoveAction);
        }*/
        if (event->button() == Qt::LeftButton) {
            //qDebug() << "press";
            m_dragTarget = this;
            m_pressPoint = event->position();
            QGuiApplication::setOverrideCursor(QCursor(Qt::ClosedHandCursor));
        }
    }
    void mouseMoveEvent(QMouseEvent *event) override {
        if (m_dragActive) {
            if (m_dragTarget) {
                // на андроид существует баг, при отпускании экрана координаты улетают
                if (event->globalPosition().x() < 50'000 && event->globalPosition().y() < 50'000) //8'388'608
                    m_dragPos = event->globalPosition();
                m_dragTarget->setPosition(m_canvas->mapFromGlobal(m_dragPos) - m_pressPoint);
                emit dragMove(m_dragTarget, m_dragPos);
            }
        } else {
            if ((event->position() - m_pressPoint).manhattanLength() >= 10) {
                m_dragActive = true;
                emit dragStart(this);
            }
        }
    }
    void mouseUngrabEvent() override {
        m_dragActive = false;
        QGuiApplication::restoreOverrideCursor();
        emit dropped(m_dragTarget, m_dragPos);
    }

private:
    bool m_clonable = false;
    bool m_dragActive = false;
    QPointF m_pressPoint;
    QPointF m_dragPos;
    QQuickItem *m_canvas = nullptr;
    QQuickItem *m_overlay = nullptr;
    AbstractBlock *m_dragTarget = nullptr;

signals:
    void clonableChanged();
    void cursorShapeChanged();
    void canvasChanged();
    void overlayChanged();
    void dragTargetChanged();
    void dragStart(AbstractBlock *target);
    void dragMove(AbstractBlock *target, const QPointF &position);
    void dropped(AbstractBlock *target, const QPointF &position);
    void clicked(AbstractBlock *target);
};

#endif // ABSTRACTBLOCK_H
