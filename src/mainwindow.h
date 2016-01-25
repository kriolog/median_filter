#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>

class QAction;
class QMenu;
class QToolBar;
class QProgressBar;
class QWidget;
class QGraphicsPixmapItem;

namespace itk {
    class QtProgressBar;
}

namespace medianFilter {
    class ZoomGraphicsView;
    class OddSpinBox;
}

namespace medianFilter {

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(const QString& in_img_file = QString(), QWidget* parent = 0, Qt::WindowFlags flags = 0);

private:
    QImage _in_img;

    QAction* _open_act;
    QAction* _exit_act;
    QMenu* _file_menu;
    QToolBar* _file_tool_bar;
    QToolBar* _filter_tool_bar;
    itk::QtProgressBar* _progress_bar;

    QWidget* _filter_parameters_wgt;
    OddSpinBox* _filter_window_size_sb;

    ZoomGraphicsView* _in_img_view;
    QGraphicsPixmapItem* _in_img_item;

    ZoomGraphicsView* _out_img_view;
    QGraphicsPixmapItem* _out_img_item;

private:
    void createCentralWidget(const QString& in_img_file);
    void createActions();
    void createMenus();
    void createFilterParameters();
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
    void computeOutputImage();
    /// Set the output image in the the corresponding view.
    void setOutputImage(const QImage& out_img);

signals:
    void inputImageLoaded(const QImage& in_img);
};


} // namespace medianFilter

#endif // MAINWINDOW_H