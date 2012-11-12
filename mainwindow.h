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

class Image;

class AboutWindow;
class BlurWindow;
class CannyWindow;
class GradientWindow;
class HistogramWindow;
class MorphologyWindow;
class ThresholdWindow;
class SetScaleWindow;

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
  public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

  private slots:
    void on_actionAbout_triggered();
    void on_actionBlur_triggered();
    void on_actionCanny_triggered();
    void on_actionCrop_triggered();
    void on_actionClose_triggered();
    void on_actionEqualize_triggered();
    void on_actionGradient_triggered();
    void on_actionGrayscale_triggered();
    void on_actionHistogram_triggered();
    void on_actionHSV_triggered();
    void on_actionInvert_triggered();
    void on_actionMorphology_triggered();
    void on_actionOpen_triggered();
    void on_actionRevert_triggered();
    void on_actionRGB_triggered();
    void on_actionSave_triggered();
    void on_actionSet_Scale_triggered();
    void on_actionStretch_triggered();
    void on_actionThreshold_triggered();
    void on_actionUndo_triggered();

    void on_imagesTabWidget_currentChanged(QWidget *image);
    void on_imagesTabWidget_tabCloseRequested(int index);

    void crop(QRect rect);

  private:
    Ui::MainWindow *ui;
    QVector<Image*> images;
    AboutWindow *aboutWindow;
    BlurWindow *blurWindow;
    CannyWindow *cannyWindow;
    GradientWindow *gradientWindow;
    HistogramWindow *histogramWindow;
    MorphologyWindow *morphologyWindow;
    ThresholdWindow *thresholdWindow;
    SetScaleWindow *setScaleWindow;
};
