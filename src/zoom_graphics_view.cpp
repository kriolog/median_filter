#include "zoom_graphics_view.h"

#include <QWheelEvent>
#include <QShowEvent>
#include <QMouseEvent>
#include <QDebug>

namespace medianFilter {

ZoomGraphicsView::ZoomGraphicsView(QWidget* parent)
    : QGraphicsView(parent)
{}

ZoomGraphicsView::ZoomGraphicsView(QGraphicsScene* scene, QWidget* parent)
    : QGraphicsView(scene,parent)
{}


void ZoomGraphicsView::wheelEvent(QWheelEvent* event)
{
    if(event->modifiers() & Qt::ControlModifier) {
        QGraphicsView::wheelEvent(event);
        return;
    }
    
    double factor = 1.25;
    if(event->delta() < 0) {
        factor = 1. / factor;
    }
    scale(factor, factor);
}

void ZoomGraphicsView::mouseDoubleClickEvent(QMouseEvent* event)
{
    if(!(event->button() & Qt::RightButton) || event->modifiers() & Qt::ControlModifier) {
        QGraphicsView::mouseDoubleClickEvent(event);
        return;
    }
    
    fitInView(sceneRect(), Qt::KeepAspectRatio);
}

void ZoomGraphicsView::showEvent(QShowEvent *event) {
    // fitInView takes effect only after the widget has been shown.
    fitInView(sceneRect(), Qt::KeepAspectRatio);
    
    QGraphicsView::showEvent(event);
}

} // namespace medianFilter