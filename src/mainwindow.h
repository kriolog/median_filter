#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QAction;
class QMenu;
class QToolBar;
class QGraphicsPixmapItem;

namespace medianFilter {
    class ZoomGraphicsView;
}

namespace medianFilter {
  
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(const QString& img_file = QString(), QWidget* parent = 0, Qt::WindowFlags flags = 0);
    
private:
    QAction* _open_act;
    QAction* _exit_act;
    QMenu* _file_menu;
    QToolBar* _file_tool_bar;
    
    ZoomGraphicsView* _in_img_view;
    QGraphicsPixmapItem* _in_img_item;
    
    ZoomGraphicsView* _out_img_view;
    QGraphicsPixmapItem* _out_img_item;

private slots:
    void open();
    
private:
    void createCentralWidget(const QString& img_file);
    void createActions();
    void createMenus();
    void createToolBars();
    
    bool loadInputImage(const QString& img_file);
};

} // namespace medianFilter

#endif // MAINWINDOW_H