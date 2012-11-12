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

namespace Ui {
  class SetScaleWindow;
}

class Image;

class SetScaleWindow : public QMainWindow
{
    Q_OBJECT
    
  public:
    explicit SetScaleWindow(Image* image,
                            QWidget *parent = 0);
    ~SetScaleWindow();
    
  private slots:
    void on_lengthSpinBox_valueChanged(int);
    void on_unitLineEdit_textEdited(const QString &);

    void on_selectPushButton_clicked();
    void on_okPushButton_clicked();

    void on_lineSelected(QLine const& line);

  private:
    Ui::SetScaleWindow *ui;
    Image* image;
    float scale;

    void updateConversion();
    void showConversion() const;
};
