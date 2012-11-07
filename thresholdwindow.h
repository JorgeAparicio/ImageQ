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

#pragma once

#include <QMainWindow>

#include "histogram.h"

class Image;

class QwtPlotMarker;

namespace Ui {
  class ThresholdWindow;
}

class ThresholdWindow : public QMainWindow
{
    Q_OBJECT
    
  public:
    explicit ThresholdWindow(Image*,
                             QWidget *parent = 0);
    ~ThresholdWindow();
    
  signals:
    void update();

  protected:
    void closeEvent(QCloseEvent *);

  private slots:
    void on_cancelPushButton_clicked();
    void on_okPushButton_clicked();

    void on_thresholdSlider_sliderMoved(int);

    void on_adaptativeCheckBox_toggled(bool);
    void on_invertedCheckBox_toggled(bool);
    void on_otsuCheckBox_toggled(bool checked);

    void on_binaryRadioButton_toggled(bool);
    void on_gaussianRadioButton_toggled(bool);
    void on_meanRadioButton_toggled(bool);
    void on_toZeroRadioButton_toggled(bool);
    void on_truncateRadioButton_toggled(bool);

    void on_sizeSpinBox_valueChanged(int);

  private:
    Ui::ThresholdWindow *ui;
    Image* image;
    Histogram histogram;
    QwtPlotMarker *yAxis;
    bool abort;

    void threshold();
};
