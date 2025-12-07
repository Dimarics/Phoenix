#include "abstractblock.h"

AbstractBlock::AbstractBlock(QQuickItem *parent) : QQuickItem(parent)
{
    setCursorShape(Qt::OpenHandCursor);
    setAcceptedMouseButtons(Qt::LeftButton);
    setKeepMouseGrab(true);
    setAcceptHoverEvents(true);
    //setAcceptTouchEvents(true);
    setFlag(QQuickItem::ItemHasContents);
    setFlag(QQuickItem::ItemIsFocusScope);
    setFlag(QQuickItem::ItemAcceptsDrops, false);
}
