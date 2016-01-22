A median image filter with a simple viewer which allows to compare the result with the original image.

**Usage**

Image zoom, pan and fit to window are available in viewer via mouse navigation (scroll, left button and right button double click respectively).
Filter has one parameter of window size which corresponds to the "strength" of its effect.

**Building and launch**

Qt5 and cmake(>= 2.8.11) should be installed

```
#!bash
mkdir build
cd build
cmake /path/to/project
make
./median_filter test.png
```