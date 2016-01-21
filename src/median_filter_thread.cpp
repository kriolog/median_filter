#include "median_filter_thread.h"

#include <algorithm>

namespace medianFilter {

MedianFilterThread::MedianFilterThread(const QImage& in_img, int w_size, QObject * parent) :
    QThread(parent),
    _in_img(in_img),
    _out_img(in_img.copy()),
    _w_size(w_size),
    _channel(0),
    _nb_channels(4)
{
    Q_ASSERT(_w_size >= 3 && _w_size % 2 == 1);
}

void MedianFilterThread::run()
{
    emit percentageComplete(0);
    if(_in_img.isNull()) {
        emit percentageComplete(100);
        emit resultReady(_out_img);
        return;
    }

    int ni = _in_img.width();
    int nj = _in_img.height();
    int size = ni * nj;
    Q_ASSERT(size > 0);
    bool is_color = !_in_img.isGrayscale();
    _nb_channels = is_color ? 4 : 2;

    // Copy input image to arrays of colors
    std::vector<int> red(size);
    std::vector<int> green (size);
    std::vector<int> blue(size);
    std::vector<int> alpha(size);
    for(int i = 0; i < ni; ++i) {
        for(int j = 0; j < nj; ++j) {
            QRgb pixel = _in_img.pixel(i, j);

            int ij = i + ni * j;
            red[ij] = qRed(pixel);
            if(is_color) {
                green[ij] = qGreen(pixel);
                blue[ij] = qBlue(pixel);
                alpha[ij] = qAlpha(pixel);
            }
        }
    }

    // Median filter
    std::vector<int> result_red(size);
    std::vector<int> result_green(size);
    std::vector<int> result_bleu(size);
    std::vector<int> result_alpha(size);
    _channel = 0;
    result_red = medianFilter(red, ni, nj, _w_size);
    ++_channel;
    if(is_color) {
        result_green = medianFilter(green, ni, nj, _w_size);
        ++_channel;
        result_bleu = medianFilter(blue, ni, nj, _w_size);
        ++_channel;
    } else {
        result_green = result_red;
        result_bleu = result_red;
    }
    result_alpha = medianFilter(alpha, ni, nj, _w_size);

    // Get result from four arrays of colors
    for(int i = 0; i < ni; ++i) {
        for(int j = 0; j < nj; ++j) {
            int ij = i + ni * j;
            QRgb pixel = qRgba(
                result_red[ij],
                result_green[ij],
                result_bleu[ij],
                result_alpha[ij]
            );
            _out_img.setPixel(i, j, pixel);
        }
    }

    emit percentageComplete(100);
    emit resultReady(_out_img);
}

std::vector<int> MedianFilterThread::medianFilter(
    const std::vector<int>& in_arr,
    int ni, int nj, int w_size
)
{
    Q_ASSERT(in_arr.size() == ni * nj);

    int w_halfsize = w_size / 2;
    int w_sqr_size = w_size * w_size;
    /// array of neighbors in window (including central pixel)
    std::vector<int> result(in_arr.size());

    for(int ci = 0; ci < ni; ++ci) {
        if(ci % 10 == 0) {
            // 100 signals for 1000x1000 image
            emit percentageComplete((_channel * 100) / _nb_channels + (ci * 100) / (ni * _nb_channels));
        }
        std::vector<int> w_items(w_sqr_size);
        for(int cj = 0; cj < nj; ++cj) {
            int w_item_nb = 0;
            for(int i = ci - w_halfsize; i <= ci + w_halfsize; ++i) {
                for(int j = cj - w_halfsize; j <= cj + w_halfsize; ++j) {
                    if(i < 0 || i >= ni || j < 0 || j >= nj) {
                        continue;
                    }
                    w_items[w_item_nb++] = in_arr[i + ni * j];
                }
            }
            Q_ASSERT(w_item_nb <= w_sqr_size);
            int mi = w_item_nb / 2;
            std::nth_element(w_items.begin(), w_items.begin() + mi, w_items.begin() + w_item_nb);
            result[ci + ni * cj] = w_items[mi];
        }
    }

    return result;
}

} // namespace medianFilter