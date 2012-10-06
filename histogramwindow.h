/*
* Copyright (C) 2012 Jorge Aparicio <jorge.aparicio.r@gmail.com>
*
* This file is part of ImageQ.
*
* ImageQ is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.

* ImageQ is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.

* You should have received a copy of the GNU General Public License
* along with ImageQ. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef HISTOGRAMWINDOW_H
#define HISTOGRAMWINDOW_H

#include <QMainWindow>

#include <opencv2/opencv.hpp>

#include "histogram.h"

namespace Ui {
  class HistogramWindow;
}

class HistogramWindow : public QMainWindow
{
    Q_OBJECT
    
  public:
    explicit HistogramWindow(cv::Mat const&, QWidget *parent = 0);
    ~HistogramWindow();
    
  private:
    Ui::HistogramWindow *ui;
    Histogram histogram;
};

#endif // HISTOGRAMWINDOW_H
