#include "median_filter_thread.h"

#include <algorithm>

#include <itkImage.h>
#include "itkMedianImageFilter.h"

#include <QElapsedTimer>
#include <QDebug>
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
    QElapsedTimer timer;
    timer.start();
    emit percentageComplete(0);
    if(_in_img.isNull()) {
        emit percentageComplete(100);
        emit resultReady(_out_img);
        return;
    }

    int ni = _in_img.width();
    int nj = _in_img.height();
    //int size = ni * nj;
    //Q_ASSERT(size > 0);
    bool is_color = !_in_img.isGrayscale();
    _nb_channels = is_color ? 4 : 2;

    typedef itk::Image<unsigned char> TImage;
    TImage::Pointer image = TImage::New();
    TImage::Pointer image2 = TImage::New();
    for(int i = 0; i < 4; ++i)
{
    itk::Index<2> corner = {{0,0}};
    itk::Size<2> size = {{static_cast<itk::SizeValueType>(_in_img.size().width()),
                            static_cast<itk::SizeValueType>(_in_img.size().height())}};
    itk::ImageRegion<2> region(corner, size);
    image->SetRegions(region);
    image->Allocate();
    image->FillBuffer(itk::NumericTraits<typename TImage::PixelType>::Zero);

    itk::ImageRegionIteratorWithIndex<TImage>
        imageIterator(image, image->GetLargestPossibleRegion());

    while(!imageIterator.IsAtEnd())
    {
        itk::Index<2> currentIndex = imageIterator.GetIndex();
        typename TImage::PixelType pixel;
        pixel = qRed(_in_img.pixel(currentIndex[0], currentIndex[1]));

        imageIterator.Set(pixel);

        ++imageIterator;
    }

    int w_halfsize = _w_size / 2;
    typedef itk::MedianImageFilter<TImage, TImage> FilterType;
    FilterType::Pointer medianFilter = FilterType::New();
    medianFilter->SetNumberOfThreads(8);
    FilterType::InputSizeType radius;
    radius.Fill(w_halfsize);
    medianFilter->SetRadius(radius);
    medianFilter->SetInput(image);
    medianFilter->Update();
    image2 = medianFilter->GetOutput();
}

    itk::ImageRegionIteratorWithIndex<TImage>
        imageIterator2( image2,  image2->GetLargestPossibleRegion());

    while(!imageIterator2.IsAtEnd())
    {
        itk::Index<2> currentIndex = imageIterator2.GetIndex();
        typename TImage::PixelType pixel;
        pixel = imageIterator2.Value();
//         qDebug()<<pixel;
        int red = pixel;
        QRgb pix = qRgb(red, red, red);
        _out_img.setPixel(currentIndex[0], currentIndex[1], pix);
        imageIterator2.Set(pixel);

        ++imageIterator2;
    }

    emit percentageComplete(100);
    emit resultReady(_out_img);
    qDebug()<<timer.elapsed();
}

} // namespace medianFilter