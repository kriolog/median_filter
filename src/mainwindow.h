#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>

class QAction;
class QMenu;
class QToolBar;
class QProgressBar;
class QGraphicsPixmapItem;

namespace medianFilter {
    class ZoomGraphicsView;
}

namespace medianFilter {

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(const QString& in_img_file = QString(), QWidget* parent = 0, Qt::WindowFlags flags = 0);

private:
    QAction* _open_act;
    QAction* _exit_act;
    QMenu* _file_menu;
    QToolBar* _file_tool_bar;
    QProgressBar* _progress_bar;

    ZoomGraphicsView* _in_img_view;
    QGraphicsPixmapItem* _in_img_item;

    ZoomGraphicsView* _out_img_view;
    QGraphicsPixmapItem* _out_img_item;

private:
    void createCentralWidget(const QString& in_img_file);
    void createActions();
    void createMenus();
    void createToolBars();
    void createProgressBar();

    /// Load an input image from a file and set it in the corresponding view;
    /// an empty output image is set in the corresponding view.
    bool loadInputImage(const QString& in_img_file);

private slots:
    void open();
    void enableOpen();
    void disableOpen();

    /// Compute an output image from the input image applying a filter.
    /// Launch the filter in a different thread and set the result via
    /// signal-slot mechanism.
    void computeOutputImage(const QImage& in_img);
    /// Set an output image is set in the corresponding view.
    void setOutputImage(const QImage& out_img);

signals:
    void inputImageLoaded(const QImage& in_img);
};


} // namespace medianFilter

#endif // MAINWINDOW_H