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

#ifndef CANNYWINDOW_H
#define CANNYWINDOW_H

#include <QMainWindow>

#include "image.h"

namespace Ui {
  class CannyWindow;
}

class CannyWindow : public QMainWindow
{
    Q_OBJECT
    
  public:
    explicit CannyWindow(Image* image,
                         QWidget *parent = 0);
    ~CannyWindow();

  signals:
    void update();

  protected:
    void closeEvent(QCloseEvent *);

  private slots:
    void on_cancelPushButton_clicked();
    void on_okPushButton_clicked();

    void on_l1RadioButton_toggled(bool);
    void on_l2RadioButton_toggled(bool);
    void on_manualRadioButton_toggled(bool);
    void on_meanRadioButton_toggled(bool);
    void on_medianRadioButton_toggled(bool);
    void on_semiAutomaticRadioButton_toggled(bool);

    void on_maximumSlider_valueChanged(int);
    void on_minimumSlider_valueChanged(int);
    void on_sizeSpinBox_valueChanged(int);
    
  private:
    Ui::CannyWindow *ui;
    Image* image;
    bool abort;

    void canny();
    double calculateMean();
    double calculateMedian();
};

#endif // CANNYWINDOW_H
