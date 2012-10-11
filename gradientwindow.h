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

#ifndef GRADIENTWINDOW_H
#define GRADIENTWINDOW_H

#include <QMainWindow>

#include "image.h"

namespace Ui {
  class GradientWindow;
}

class GradientWindow : public QMainWindow
{
    Q_OBJECT
    
  public:
    explicit GradientWindow(Image* image,
                            QWidget *parent = 0);
    ~GradientWindow();
    
  signals:
    void update();

  protected:
    void closeEvent(QCloseEvent *);

  private slots:
    void on_cancelPushButton_clicked();
    void on_okPushButton_clicked();

    void on_absoluteCheckBox_toggled(bool);
    void on_laplacianRadioButton_toggled(bool);
    void on_magnitudeCheckBox_toggled(bool);
    void on_scharrRadioButton_toggled(bool);
    void on_sobelRadioButton_toggled(bool);

    void on_sizeSpinBox_valueChanged(int);
    void on_xSpinBox_valueChanged(int);
    void on_ySpinBox_valueChanged(int);

  private:
    Ui::GradientWindow *ui;
    Image* image;
    bool abort;

    void gradient();
};

#endif // GRADIENTWINDOW_H
