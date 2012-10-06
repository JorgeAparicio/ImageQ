ABOUT
=====

ImageQ is a Qt based image processing application.

ImageQ uses OpenCV library for the image processing backend and uses Qt and Qwt
libraries for the GUI frontend.

ImageQ currently provides the following operations:

+ Filtering
+ Histogram processing
+ Morphology operators
+ Edge detection

With ImageQ, you can see the effect of varying the parameters of these 
operations on the go.

For now, all images are converted to grayscale before processing.

DEPENDENCIES
============

+ OpenCV (http://opencv.org/) - Developed using version 2.3
+ Qt library (http://qt-project.org/) - Developed using version 4.8
+ Qwt library (http://sourceforge.net/projects/qwt/) - Developed using version 6

BUILDING
========

 $ qmake
 $ make

DEVELOPMENT
===========

ImageQ was developed using Qt Creator, which is the recommended IDE for further 
development.

LICENSE
=======

ImageQ source code is released under the terms of the GNU General Public
License (GPL) version 3.

See COPYING.GPL3.txt for more details.
