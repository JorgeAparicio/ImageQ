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
  class TextListWindow;
}

class TextListWindow : public QMainWindow
{
    Q_OBJECT
    
  public:
    explicit TextListWindow(QString const& title,
                            QString const& header,
                            QWidget *parent = 0);
    ~TextListWindow();
    void append(QString const& s);
    int size() const;
    
  private slots:
    void on_savePushButton_clicked();

    void on_closePushButton_clicked();

  private:
    Ui::TextListWindow *ui;
    QStringList text;
    int count;
};
