#ifndef POINTTRACKER_H
#define POINTTRACKER_H

#include <QObject>

class PointTracker : public QObject
{
    Q_OBJECT
public:
    PointTracker(QObject *parent = nullptr);
    void setSetpoint(qreal x, qreal y, qreal z);
    void update(qreal x, qreal y, qreal z);

private:
    qreal m_targetX;
    qreal m_targetY;
    qreal m_targetZ;

signals:
    void reached();
};

#endif // POINTTRACKER_H
