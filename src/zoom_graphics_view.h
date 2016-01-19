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

protected:
    virtual void wheelEvent(QWheelEvent* event);
    virtual void mouseDoubleClickEvent(QMouseEvent* event);
    virtual void showEvent(QShowEvent *event);
};

} // namespace medianFilter

#endif // ZOOM_GRAPHICS_VIEW