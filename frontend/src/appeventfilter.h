#ifndef APPEVENTFILTER_H
#define APPEVENTFILTER_H

#include "networkinterface.h"
#include <QObject>

class AppEventFilter : public QObject
{
    Q_OBJECT
public:
    AppEventFilter(QObject *parent = nullptr);

private:
    bool eventFilter(QObject *target, QEvent *event) override;
    NetworkInterface *interface() const;
};

#endif // APPEVENTFILTER_H
