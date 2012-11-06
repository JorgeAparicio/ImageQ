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

#ifndef IMAGE_H
#define IMAGE_H

#include <QWidget>
#include <QTimer>

#include <opencv2/opencv.hpp>

namespace Ui {
  class Image;
}

class Image : public QWidget
{
    Q_OBJECT
    
  public:
    explicit Image(QString pathToImage, QWidget *parent = 0);
    explicit Image(cv::Mat const& image, QWidget *parent = 0);
    ~Image();

    cv::Mat current;
    cv::Mat previous;

    void backup();
    void HSV(std::vector<cv::Mat>& hsv) const;
    void revert();
    void RGB(std::vector<cv::Mat>& rgb) const;
    void undo();

  public slots:
    void update() const;
    void rescale() const;
    
  private slots:
    void on_fitToScreenCheckBox_toggled();

  private:
    Ui::Image *ui;
    cv::Mat first;
    QTimer timer;
};

#endif // IMAGE_H
