#ifndef MEDIAN_FILTER_THREAD_H
#define MEDIAN_FILTER_THREAD_H

#include <QThread>
#include <QImage>
#include <vector>

namespace medianFilter {

class MedianFilterThread : public QThread
{
    Q_OBJECT

public:
    MedianFilterThread(const QImage& in_img, int w_size, QObject * parent = 0);
    void run();

private:
    QImage _in_img;
    QImage _out_img;
    int _w_size;
    // current channel number
    int _channel;
    // number of channels : 4 or 2 if image is grayscale
    int _nb_channels;

private:
    // ...

signals:
    void percentageComplete(int);
    void resultReady(const QImage& out_img);
};

} // namespace medianFilter

#endif // MEDIAN_FILTER_THREAD_H
