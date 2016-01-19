#ifndef ZOOM_GRAPHICS_VIEW
#define ZOOM_GRAPHICS_VIEW

#include <QGraphicsView>

namespace medianFilter {

class ZoomGraphicsView : public QGraphicsView
{
    Q_OBJECT
    
public:
    ZoomGraphicsView (QWidget* parent = 0);
    ZoomGraphicsView (QGraphicsScene* scene, QWidget* parent = 0);
    void fitScene();

public slots:
    void setScale(qreal sx, qreal sy);
    void setTranslate(qreal dx, qreal dy);
    
protected:
    virtual void wheelEvent(QWheelEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void mouseDoubleClickEvent(QMouseEvent* event);
    virtual void showEvent(QShowEvent *event);
    
signals:
    void scaled(qreal sx, qreal sy);
    void translated(qreal dx, qreal dy);
};

} // namespace medianFilter

#endif // ZOOM_GRAPHICS_VIEW