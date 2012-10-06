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

#ifndef BLURWINDOW_H
#define BLURWINDOW_H

#include <QMainWindow>

#include <opencv2/opencv.hpp>

namespace Ui {
  class BlurWindow;
}

class BlurWindow : public QMainWindow
{
    Q_OBJECT
    
  public:
    explicit BlurWindow(cv::Mat const&, cv::Mat const&, QWidget *parent = 0);
    ~BlurWindow();
    
  signals:
    void updatedImage();

  protected:
    void closeEvent(QCloseEvent *);

  private slots:
    void on_cancelPushButton_clicked();
    void on_okPushButton_clicked();

    void on_averageRadioButton_toggled(bool);
    void on_gaussianRadioButton_toggled(bool);
    void on_medianRadioButton_toggled(bool);

    void on_sizeSpinBox_valueChanged(int);

  private:
    Ui::BlurWindow *ui;
    cv::Mat image;
    cv::Mat backup;
    bool abort;

    void blur();
};

#endif // BLURWINDOW_H
