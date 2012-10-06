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

#ifndef THRESHOLDWINDOW_H
#define THRESHOLDWINDOW_H

#include <QMainWindow>

#include <opencv2/opencv.hpp>

#include <qwt_plot_marker.h>

#include "histogram.h"

namespace Ui {
  class ThresholdWindow;
}

class ThresholdWindow : public QMainWindow
{
    Q_OBJECT
    
  public:
    explicit ThresholdWindow(cv::Mat const&,
                             cv::Mat const&,
                             QWidget *parent = 0);
    ~ThresholdWindow();
    
  signals:
    void updatedImage();

  protected:
    void closeEvent(QCloseEvent *);

  private slots:
    void on_cancelPushButton_clicked();
    void on_okPushButton_clicked();

    void on_thresholdSlider_sliderMoved(int);

    void on_invertedCheckBox_toggled(bool);
    void on_otsuCheckBox_toggled(bool checked);

    void on_normalRadioButton_toggled(bool);
    void on_toZeroRadioButton_toggled(bool);
    void on_truncateRadioButton_toggled(bool);

  private:
    Ui::ThresholdWindow *ui;
    cv::Mat image;
    cv::Mat backup;
    Histogram histogram;
    QwtPlotMarker *yAxis;
    bool abort;

    void threshold();
};

#endif // THRESHOLDWINDOW_H
