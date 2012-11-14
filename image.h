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

#include <QWidget>
#include <QTimer>

#include <opencv2/core/core.hpp>

namespace Ui {
  class Image;
}

class Image : public QWidget
{
    Q_OBJECT
    
  public:
    enum SelectionMode {
      None, Line, Rectangle
    };

    explicit Image(QString pathToImage, QWidget *parent = 0);
    explicit Image(cv::Mat const& image, QWidget *parent = 0);
    ~Image();

    cv::Mat current;
    cv::Mat previous;
    float scale;
    QString unit;

    void backup();
    void HSV(std::vector<cv::Mat>& hsv) const;
    void revert();
    void RGB(std::vector<cv::Mat>& rgb) const;
    void undo();

  signals:
    void exitSelectionMode();
    void lineSelected(QLine const& line, QPoint center = QPoint());
    void rectangleSelected(QRect const& rect);
    void status(QString const& msg);

  public:
    void drawText(QPoint center, QString text);
    void setFitToScreenCheckboxEnabled(bool enabled);

  public slots:
    void display();
    void pixelInfo(QPoint p) const;
    void mouseDoubleClick(QPoint p);
    void mouseMove(QPoint p);
    void mousePress(QPoint p);
    void mouseRelease(QPoint p);
    void on_fitToScreenCheckBox_toggled(bool checked);
    void rescale();
    void setSelectionMode(SelectionMode mode);
    void update();

  private:
    Ui::Image *ui;
    cv::Mat first;
    QPixmap pixmap, tempPixmap, overlayedPixmap;
    QPoint p1, p2;
    QRect rect;
    QColor color;
    bool mousePressed;
    SelectionMode selectionMode;

    void remapPoint(QPoint &p) const;
    void initialize();
};
