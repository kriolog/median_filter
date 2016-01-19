#include "mainwindow.h"

#include <QtWidgets>

#include "zoom_graphics_view.h"

namespace medianFilter {
    
MainWindow::MainWindow(const QString& img_file, QWidget* parent, Qt::WindowFlags flags)
    : QMainWindow(parent, flags)
{
    createCentralWidget(img_file);
    createActions();
    createMenus();
    createToolBars();
}

void MainWindow::createCentralWidget(const QString& img_file)
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

    loadInputImage(img_file); 
    
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

void MainWindow::createToolBars()
{
    _file_tool_bar = addToolBar(tr("File"));
    _file_tool_bar->addAction(_open_act);
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

bool MainWindow::loadInputImage(const QString &img_file)
{
    QPixmap img(img_file);
    if(img.isNull()) {
        return false;
    }
    
    _in_img_item->setPixmap(img);
    _in_img_view->setSceneRect(_in_img_item->boundingRect());
    _in_img_view->fitScene();
    
    // TODO: TMP
    _out_img_item->setPixmap(img);
    _out_img_view->setSceneRect(_out_img_item->boundingRect());
    _out_img_view->fitScene();
    
    return true;
}

} // namespace medianFilter