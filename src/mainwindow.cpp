#include "mainwindow.h"

#include <QtWidgets>

#include "../3rdparty/ITKApps/QtITK/itkQtProgressBar.h"

#include "zoom_graphics_view.h"
#include "median_filter_thread.h"
#include "odd_spin_box.h"

namespace medianFilter {

MainWindow::MainWindow(const QString& in_img_file, QWidget* parent, Qt::WindowFlags flags)
    : QMainWindow(parent, flags), _in_img()
{
    createActions();
    createMenus();
    createFilterParameters();
    createToolBars();
    createProgressBar();
    createCentralWidget(in_img_file);

    connect(this, &MainWindow::inputImageLoaded, this, &MainWindow::computeOutputImage);
    loadInputImage(in_img_file);
}

void MainWindow::createCentralWidget(const QString& in_img_file)
{
    _in_img_view = new ZoomGraphicsView(new QGraphicsScene());
    _in_img_view->setBackgroundBrush(Qt::black);
    _in_img_item = new QGraphicsPixmapItem();
    _in_img_view->scene()->addItem(_in_img_item);

    _out_img_view = new ZoomGraphicsView(new QGraphicsScene());
    _out_img_view->setBackgroundBrush(Qt::black);
    _out_img_item = new QGraphicsPixmapItem();
    _out_img_view->scene()->addItem(_out_img_item);

    connect(_in_img_view, &ZoomGraphicsView::scaled, _out_img_view, &ZoomGraphicsView::setScale);
    connect(_in_img_view, &ZoomGraphicsView::translated, _out_img_view, &ZoomGraphicsView::setTranslate);
    connect(_out_img_view, &ZoomGraphicsView::scaled, _in_img_view, &ZoomGraphicsView::setScale);
    connect(_out_img_view, &ZoomGraphicsView::translated, _in_img_view, &ZoomGraphicsView::setTranslate);

    QWidget* main_wgt = new QWidget();
    QHBoxLayout* main_lo = new QHBoxLayout();
    main_lo->setContentsMargins(0,0,0,0);
    main_wgt->setLayout(main_lo);
    main_lo->addWidget(_in_img_view);
    main_lo->addWidget(_out_img_view);
    setCentralWidget(main_wgt);
}

void MainWindow::createActions()
{
    _open_act = new QAction(QIcon::fromTheme("document-open"), tr("&Open..."), this);
    _open_act->setShortcuts(QKeySequence::Open);
    _open_act->setStatusTip(tr("Open an image"));
    connect(_open_act, &QAction::triggered, this, &MainWindow::open);

    _exit_act = new QAction(QIcon::fromTheme("application-exit"), tr("E&xit"), this);
    _exit_act->setShortcuts(QKeySequence::Quit);
    _exit_act->setStatusTip(tr("Exit the application"));
    connect(_exit_act,  &QAction::triggered, this, &MainWindow::close);
}

void MainWindow::createMenus()
{
    _file_menu = menuBar()->addMenu(tr("&File"));
    _file_menu->addAction(_open_act);
    _file_menu->addSeparator();
    _file_menu->addAction(_exit_act);
}

void MainWindow::createFilterParameters()
{
    _filter_window_size_sb = new OddSpinBox();
    _filter_window_size_sb->setMinimum(3);
    _filter_window_size_sb->setMaximum(99);
    _filter_window_size_sb->setValue(5);

    QLabel* lbl = new QLabel(tr("Window &Size"));
    lbl->setBuddy(_filter_window_size_sb);

    QPushButton* run_bn = new QPushButton(tr("&Run"));
    connect(run_bn, &QPushButton::clicked, this, &MainWindow::computeOutputImage);

    _filter_parameters_wgt = new QWidget();
    QHBoxLayout* lo = new QHBoxLayout();
    _filter_parameters_wgt->setLayout(lo);
    lo->addWidget(lbl);
    lo->addWidget(_filter_window_size_sb);
    lo->addWidget(run_bn);
}

void MainWindow::createToolBars()
{
    _file_tool_bar = addToolBar(tr("File"));
    _file_tool_bar->addAction(_open_act);
    _filter_tool_bar = addToolBar(tr("Filter Parameters"));
    _filter_tool_bar->addWidget(_filter_parameters_wgt);
}

void MainWindow::createProgressBar()
{
    _progress_bar = new itk::QtProgressBar(nullptr);
    statusBar()->addPermanentWidget(_progress_bar);
    _progress_bar->setMaximum(100);
    _progress_bar->hide();

}

// Copypasted from Qt doc
void MainWindow::open()
{
    QStringList mimeTypeFilters;
    foreach (const QByteArray &mimeTypeName, QImageReader::supportedMimeTypes())
        mimeTypeFilters.append(mimeTypeName);
    mimeTypeFilters.sort();
    const QStringList picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
    QFileDialog dialog(this, tr("Open Image"),
        picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.last());
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setMimeTypeFilters(mimeTypeFilters);
    dialog.selectMimeTypeFilter("image/jpeg");

    while(dialog.exec() == QDialog::Accepted && !loadInputImage(dialog.selectedFiles().first())) {}
}

void MainWindow::enableOpen()
{
    _open_act->setEnabled(true);
}

void MainWindow::disableOpen()
{
    _open_act->setDisabled(true);
}

bool MainWindow::loadInputImage(const QString &in_img_file)
{
    if(in_img_file.isEmpty()) {
        statusBar()->showMessage(tr("Open a file"));
        return false;
    }

    QImage in_img(in_img_file);
    if(in_img.isNull()) {
        statusBar()->showMessage("Can't open file " + in_img_file);
        return false;
    }

    _in_img.swap(in_img);
    statusBar()->showMessage(in_img_file);

    _in_img_item->setPixmap(QPixmap::fromImage(_in_img));
    _in_img_view->setSceneRect(_in_img_item->boundingRect());

    // Show an empty pixmap during the output image compution.
    _out_img_item->setPixmap(QPixmap());
    _out_img_view->setSceneRect(_in_img_view->sceneRect());

    // Fit one of two images is enough.
    _in_img_view->fitScene();

    emit inputImageLoaded(_in_img);

    return true;
}

void MainWindow::computeOutputImage()
{
    if(_in_img.isNull()){
        return;
    }

    disableOpen();
    _progress_bar->reset();
    _progress_bar->show();

    MedianFilterThread* median_filter_thread = new MedianFilterThread(_in_img, _filter_window_size_sb->value(), _progress_bar, this);
    connect(median_filter_thread, &MedianFilterThread::finished, _progress_bar, &itk::QtProgressBar::hide);
    connect(median_filter_thread, &MedianFilterThread::resultReady, this, &MainWindow::setOutputImage);
    connect(median_filter_thread, &MedianFilterThread::finished, median_filter_thread, &QObject::deleteLater);
    connect(median_filter_thread, &MedianFilterThread::finished, this, &MainWindow::enableOpen);
    median_filter_thread->start();
}

void MainWindow::setOutputImage(const QImage& out_img)
{
    _out_img_item->setPixmap(QPixmap::fromImage(out_img));
    _out_img_view->setSceneRect(_in_img_view->sceneRect());
    _out_img_view->fitScene();
}

} // namespace medianFilter