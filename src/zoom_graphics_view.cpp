#include "zoom_graphics_view.h"

#include <QWheelEvent>
#include <QShowEvent>
#include <QMouseEvent>
#include <QScrollBar>

namespace medianFilter {

ZoomGraphicsView::ZoomGraphicsView(QWidget* parent)
    : QGraphicsView(parent)
{
    setDragMode(QGraphicsView::ScrollHandDrag);
}

ZoomGraphicsView::ZoomGraphicsView(QGraphicsScene* scene, QWidget* parent)
    : QGraphicsView(scene,parent)
{
    setDragMode(QGraphicsView::ScrollHandDrag);
}

void ZoomGraphicsView::fitScene()
{
    qreal old_m11 = transform().m11();
    qreal old_m22 = transform().m22();
    fitInView(sceneRect(), Qt::KeepAspectRatio);
    Q_ASSERT(old_m11 != 0);
    Q_ASSERT(old_m22 != 0);
    emit scaled(transform().m11() / old_m11, transform().m22() / old_m22) ;
}

void ZoomGraphicsView::setScale(qreal sx, qreal sy)
{
    this->scale(sx, sy);
}

void ZoomGraphicsView::setTranslate(qreal dx, qreal dy)
{
    horizontalScrollBar()->setValue(dx + horizontalScrollBar()->value());
    verticalScrollBar()->setValue(dy + verticalScrollBar()->value());
}

void ZoomGraphicsView::wheelEvent(QWheelEvent* event)
{
    if(event->modifiers() & Qt::ControlModifier) {
        QGraphicsView::wheelEvent(event);
        return;
    }
    
    qreal factor = 1.25;
    if(event->delta() < 0) {
        factor = 1. / factor;
    }

    QPointF scene_pos = mapToScene(event->pos());
    scale(factor, factor);
    QPointF translate = mapFromScene(scene_pos) - event->pos();
    setTranslate(translate.x(), translate.y());

    emit scaled(factor, factor);
    emit translated(translate.x(), translate.y());
    
    event->ignore();
}

void ZoomGraphicsView::mouseMoveEvent(QMouseEvent* event)
{   
    QPointF scene_pos = mapToScene(event->pos());
    QGraphicsView::mouseMoveEvent(event);
    QPointF translate = event->pos() - mapFromScene(scene_pos);
    
    emit translated(translate.x(), translate.y());
}

void ZoomGraphicsView::mouseDoubleClickEvent(QMouseEvent* event)
{
    if(event->button() & Qt::RightButton && !(event->modifiers() & Qt::ControlModifier)) {
        fitScene();
        event->accept();
        return;
    }
    event->ignore();    
}

void ZoomGraphicsView::showEvent(QShowEvent *event) {
    // fitScene takes effect only after the widget has been shown.
    fitScene();
    event->ignore();
}

} // namespace medianFilter