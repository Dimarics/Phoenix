#include "appeventfilter.h"
#include "appbackend.h"
#include "qqmlproperty.h"
#include <QKeyEvent>
#include <QQuickItem>

AppEventFilter::AppEventFilter(QObject *parent) : QObject(parent) { }

bool AppEventFilter::eventFilter(QObject *target, QEvent *event) {
    static QObject *focusObject = nullptr;
    if (event->type() == QEvent::MouseButtonPress) {
        //qDebug() << "mouse button press:" << target->metaObject()->className();
        focusObject = target;
        QMetaObject::invokeMethod(this, [] {
            if (focusObject != qApp->focusObject()) {
                if (auto focusItem = qobject_cast<QQuickItem*>(qApp->focusObject())) {
                    focusItem->setFocus(false);
                }
            }
        }, Qt::QueuedConnection);
    }
    if (event->type() == QEvent::FocusAboutToChange) {
        //qDebug() << "focus about to change:" << target->metaObject()->className();
    }
    if (event->type() == QEvent::KeyPress) {
        //qDebug() << target->metaObject()->className();//QQuickWindowQmlImpl
    }
    if (target == qApp->focusObject() &&
            (target->metaObject()->className() == QStringLiteral("QQuickRootItem") ||
             target->metaObject()->className() == QStringLiteral("QQuickWindowQmlImpl"))) {
        /*
        inline void forward() { m_pitchPWM = 1700; }
        inline void backward() { m_pitchPWM = 1300; }
        inline void left() { m_rollPWM = 1300; }
        inline void right() { m_rollPWM = 1700; }
        inline void align() { m_rollPWM = 1500; m_pitchPWM = 1500; }
        */
        switch (event->type()) {
        case QEvent::KeyPress:
            if (static_cast<QKeyEvent*>(event)->isAutoRepeat()) return false;
            switch (static_cast<QKeyEvent*>(event)->key()) {
            case Qt::Key_W: // 17 W
                interface()->setPitchPWM(30);
                break;
            case Qt::Key_A: // 30 A
                interface()->setRollPWM(-30);
                break;
            case Qt::Key_S: // 31 S
                interface()->setPitchPWM(-30);
                break;
            case Qt::Key_D: // 32 D
                interface()->setRollPWM(30);
                break;
            case Qt::Key_Space: // 57 Space
                interface()->setThrottlePWM(75);
                break;
            case Qt::Key_Shift:
                interface()->setThrottlePWM(0);
                break;
            case Qt::Key_Left:
                interface()->setYawPWM(-80);
                break;
            case Qt::Key_Right:
                interface()->setYawPWM(80);
                break;
            default:
                break;
            }
            break;
        case QEvent::KeyRelease:
            if (static_cast<QKeyEvent*>(event)->isAutoRepeat()) return false;
            switch (static_cast<QKeyEvent*>(event)->key()) {
            case Qt::Key_W:
                interface()->setPitchPWM(0);
                break;
            case Qt::Key_A:
                interface()->setRollPWM(0);
                break;
            case Qt::Key_S:
                interface()->setPitchPWM(0);
                break;
            case Qt::Key_D:
                interface()->setRollPWM(0);
                break;
            case Qt::Key_Space:
                interface()->setThrottlePWM(45);
                break;
            case Qt::Key_Left:
                interface()->setYawPWM(0);
                break;
            case Qt::Key_Right:
                interface()->setYawPWM(0);
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
    }
    return false;
}

NetworkInterface *AppEventFilter::interface() const {
    return QQmlProperty::read(AppBackend::instance()->device(), "protocol").value<NetworkInterface*>();
}
