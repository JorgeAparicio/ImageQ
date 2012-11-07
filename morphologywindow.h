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

#include <opencv2/core/core.hpp>

class Image;

namespace Ui {
  class MorphologyWindow;
}

class MorphologyWindow : public QMainWindow
{
    Q_OBJECT
    
  public:
    explicit MorphologyWindow(Image* image,
                              QWidget *parent = 0);
    ~MorphologyWindow();
    
  signals:
    void update();

  protected:
    void closeEvent(QCloseEvent *);

  private slots:
    void on_cancelPushButton_clicked();
    void on_okPushButton_clicked();

    void on_closeRadioButton_toggled(bool);
    void on_crossRadioButton_toggled(bool);
    void on_dilateRadioButton_toggled(bool);
    void on_diskRadioButton_toggled(bool);
    void on_erodeRadioButton_toggled(bool);
    void on_openRadioButton_toggled(bool);
    void on_squareRadioButton_toggled(bool);
    void on_xRadioButton_toggled(bool);

    void on_iterationsSpinBox_valueChanged(int);
    void on_sizeSpinBox_valueChanged(int);

    void on_customRadioButton_toggled(bool checked);

  private:
    Ui::MorphologyWindow *ui;
    Image* image;
    cv::Mat structuringElement;
    bool abort;

    void morphology();
    void updateStructuringElement();
};
