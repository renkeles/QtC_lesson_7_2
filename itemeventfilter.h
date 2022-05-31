#ifndef ITEMEVENTFILTER_H
#define ITEMEVENTFILTER_H

#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>

class ItemEventFilter : public QGraphicsItem {
public:
    QRectF boundingRect() const override
    {
        return {};
    }
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *style, QWidget* widget = nullptr) override
    {
        Q_UNUSED(painter);
        Q_UNUSED(style);
        Q_UNUSED(widget);
    }

protected:
    bool sceneEventFilter(QGraphicsItem *watched, QEvent *event) override {
        if (eventMove(watched, event))
        {
            return true;
        }
        return eventRotate(watched, event);
    }

private:

    bool checkerMove = false;
    bool checkerRotate = false;

    bool eventMove(QGraphicsItem *watched, QEvent *event)
    {
        if(event->type() == QEvent::GraphicsSceneMousePress)
        {
            QGraphicsSceneMouseEvent *mouseEvent = static_cast<QGraphicsSceneMouseEvent*>(event);
            if(mouseEvent->button() == Qt::LeftButton)
            {
                checkerMove = true;
                return true;
            }
        }
        if(event->type() == QEvent::GraphicsSceneMouseRelease)
        {
            QGraphicsSceneMouseEvent *mouseEvent = static_cast<QGraphicsSceneMouseEvent*>(event);
            if(mouseEvent->button() == Qt::LeftButton)
            {
                checkerMove = false;
                return true;
            }
        }
        if (event->type() == QEvent::GraphicsSceneMouseMove)
        {
            QGraphicsSceneMouseEvent *mouseEvent = static_cast<QGraphicsSceneMouseEvent*>(event);
            if (checkerMove)
            {
                QPointF movePos = mouseEvent->scenePos() - mouseEvent->lastScenePos();
                watched->moveBy(movePos.x(), movePos.y());
                return true;
            }
        }
        return false;
    }

    bool eventRotate(QGraphicsItem *watched, QEvent *event)
    {
        if (event->type() == QEvent::GraphicsSceneMousePress)
        {
            QGraphicsSceneMouseEvent* mouseEvent = static_cast<QGraphicsSceneMouseEvent*>(event);
            if (mouseEvent->button() == Qt::MidButton)
            {
                checkerRotate = true;
                return true;
            }
        }
        if (event->type() == QEvent::GraphicsSceneMouseRelease)
        {
            QGraphicsSceneMouseEvent* mouseEvent = static_cast<QGraphicsSceneMouseEvent*>(event);
            if (mouseEvent->button() == Qt::MidButton)
            {
                checkerRotate = false;
                return true;
            }
        }
        if (event->type() == QEvent::GraphicsSceneMouseMove) {
            QGraphicsSceneMouseEvent* mouseEvent = static_cast<QGraphicsSceneMouseEvent*>(event);
            if (checkerRotate)
            {
                QTransform transform;
                QPoint mousePos = mouseEvent->pos().toPoint();
                QPointF centerRect = watched->boundingRect().center();
                double angle = mousePos.x() - centerRect.x();
                transform.translate(centerRect.x(), centerRect.y()).rotate(angle).translate(-centerRect.x(), -centerRect.y());
                watched->setTransform(transform);
                return true;
            }
        }
        return false;
    }

};

#endif // ITEMEVENTFILTER_H
