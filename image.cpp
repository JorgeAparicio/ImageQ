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

#include "image.h"
#include "ui_image.h"

#include "mat2qimage.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <QScrollBar>
#include <QPainter>

Image::Image(QString pathToImage, QWidget *parent) :
  QWidget(parent),
  ui(new Ui::Image),
  first(cv::imread(pathToImage.toStdString()))
{
  initialize();
}

Image::Image(cv::Mat const& image, QWidget *parent) :
  QWidget(parent),
  ui(new Ui::Image),
  first(image)
{
  initialize();
}

void Image::initialize()
{
  ui->setupUi(this);

  first.copyTo(current);

  color = QColor(Qt::red);

  mousePressed = false;
  selectionMode = None;

  connect(ui->imageLabel, SIGNAL(mouseHover(QPoint)),
          this,           SLOT(pixelInfo(QPoint)));

  connect(ui->imageLabel, SIGNAL(mouseHover(QPoint)),
          this,           SLOT(mouseMove(QPoint)));

  connect(ui->imageLabel, SIGNAL(mouseDoubleClick(QPoint)),
          this,           SLOT(mouseDoubleClick(QPoint)));

  connect(ui->imageLabel, SIGNAL(mousePress(QPoint)),
          this,           SLOT(mousePress(QPoint)));

  connect(ui->imageLabel, SIGNAL(mouseRelease(QPoint)),
          this,           SLOT(mouseRelease(QPoint)));

  connect(ui->imageLabel, SIGNAL(resized()),
          this,           SLOT(rescale()));

  connect(ui->scrollArea->horizontalScrollBar(),  SIGNAL(rangeChanged(int,int)),
          this,                                   SLOT(rescale()));

  connect(ui->scrollArea->verticalScrollBar(),  SIGNAL(rangeChanged(int,int)),
          this,                                 SLOT(rescale()));
}

Image::~Image()
{
  delete ui;

  // FIXME: Is this necessary?
  current.release();
  previous.release();
  first.release();
}

void Image::backup()
{
  current.copyTo(previous);
}

void Image::HSV(std::vector<cv::Mat>& hsv) const
{
  cv::Mat tmp;

  cv::cvtColor(current, tmp, CV_BGR2HSV);

  cv::split(tmp, hsv);
}

void Image::revert()
{
  first.copyTo(current);
  previous.release();

  update();
}

void Image::RGB(std::vector<cv::Mat>& rgb) const
{
  cv::Mat tmp;

  cv::cvtColor(current, tmp, CV_BGR2RGB);

  cv::split(tmp, rgb);
}

void Image::undo()
{
  if (previous.data != 0) {
    previous.copyTo(current);
    update();
  }

}

void Image::display()
{
  pixmap = QPixmap::fromImage(Mat2QImage(current));

  if (ui->fitToScreenCheckBox->isChecked())
    pixmap = pixmap.scaled(ui->imageLabel->size(), Qt::KeepAspectRatio);

  ui->imageLabel->setPixmap(pixmap);
}

void Image::pixelInfo(QPoint p) const
{
  remapPoint(p);

  ui->xLabel->setText(QString::number(p.x()));
  ui->yLabel->setText(QString::number(p.y()));

  switch(current.channels()) {
    case 1:
      ui->valueLabel->setText(QString::number(current.at<quint8>(p.y(), p.x())));
      break;
    case 3:
      cv::Vec3b bgr = current.at<cv::Vec3b>(p.y(), p.x());
      ui->valueLabel->setText("(" + QString::number(bgr[2]) +
                              ", " + QString::number(bgr[1]) +
                              ", " + QString::number(bgr[0]) + ")");
      break;
  }
}

void Image::mouseDoubleClick(QPoint p)
{
  remapPoint(p);

  switch (selectionMode) {
    case None:
      break;

    case Line:
      break;

    case Rectangle:
      if (rect.topLeft().x() <= p.x() && p.x() <= rect.bottomRight().x() &&
          rect.topLeft().y() <= p.y() && p.y() <= rect.bottomRight().y()) {
        selectionMode = None;
        emit rectangleSelected(rect);
      } else {
        emit status("Drag-select an area.");
      }
      break;
  }

  p1 = QPoint(-1, -1);
  p2 = QPoint(-1, -1);

  ui->imageLabel->setPixmap(pixmap);
}

void Image::mouseMove(QPoint p)
{
  if (selectionMode != None) {
    remapPoint(p);

    if (mousePressed) {
      p2 = p;

      overlayedPixmap = pixmap;
      color.setAlpha(128);

      QPainter painter(&overlayedPixmap);
      painter.setPen(color);

      switch (selectionMode) {
        case Rectangle:
        {
          QRect rect;

          if (p1.x() < p2.x() || p1.y() < p2.y())
            rect = QRect((p1 * pixmap.width()) / current.cols,
                         (p2 * pixmap.height() / current.rows));
          else
            rect = QRect((p2 * pixmap.width()) / current.cols,
                         (p1 * pixmap.height() / current.rows));

          painter.drawRect(rect);
          color.setAlpha(64);
          painter.fillRect(rect, color);
          break;
        }

        case Line:
        {
          QLine line((p1 * pixmap.width()) / current.cols,
                     (p2 * pixmap.height() / current.rows));

          painter.drawLine(line);
          break;
        }

        case None:
          break;
      }

      ui->imageLabel->setPixmap(overlayedPixmap);
    }
  }
}

void Image::mousePress(QPoint p)
{
  if (selectionMode != None) {
    remapPoint(p);

    mousePressed = true;

    p1 = p;
    p2 = QPoint(-1, -1);
  }
}

void Image::mouseRelease(QPoint p)
{
  if (selectionMode != None) {
    remapPoint(p);

    mousePressed = false;

    p2 = p;

    if (p1.x() >= 0 && p1.y() >= 0 && p1.x() != p2.x() && p1.y() != p2.y())
      switch (selectionMode) {
        case None:
          break;

        case Line:
          break;

        case Rectangle:
          emit status("Double-click inside the area to crop.");

          if (p1.x() < p2.x() || p1.y() < p2.y())
            rect = QRect(p1, p2);
          else
            rect = QRect(p2, p1);
          break;
      }
  }
}

void Image::on_fitToScreenCheckBox_toggled(bool checked)
{
  if (checked &&
      (ui->scrollArea->horizontalScrollBar()->maximum() != 0 ||
       ui->scrollArea->verticalScrollBar()->maximum() != 0))
    ui->imageLabel->clear();
  else
    display();
}

void Image::rescale()
{
  if (ui->fitToScreenCheckBox->isChecked()) {
    if (ui->scrollArea->horizontalScrollBar()->maximum() == 0 &&
        ui->scrollArea->verticalScrollBar()->maximum() == 0) {
      if (pixmap.width() != ui->imageLabel->width() &&
          pixmap.height() != ui->imageLabel->height()) {
        display();
      }
    } else if (pixmap.height() != 0 && pixmap.width() != 0) {
      ui->imageLabel->clear();
      pixmap = QPixmap();
    }
  }
}

void Image::setSelectionMode(SelectionMode mode)
{
  selectionMode = mode;

  switch (mode) {
    case None:
      break;

    case Line:
      break;

    case Rectangle:
      emit status("Drag-select an area.");
      break;
  }
}

void Image::update()
{
  double min, max;
  cv::minMaxLoc(current, &min, &max);

  ui->minimumLabel->setText(QString::number(min));
  ui->maximumLabel->setText(QString::number(max));

  ui->heightLabel->setText(QString::number(current.rows));
  ui->widthLabel->setText(QString::number(current.cols));

  switch (current.channels()) {
    case 1:
      ui->channelLabel->setText("1");
      break;
    case 3:
      ui->channelLabel->setText("3");
      break;
  }

  switch (current.depth()) {
    case CV_8U:
      ui->depthLabel->setText("8 bits");
      break;
    case CV_32F:
      ui->depthLabel->setText("32 bits");
      break;
  }

  display();
}

void Image::remapPoint(QPoint &p) const
{
  int x = p.x();
  int y = p.y();

  int imageHeight = current.rows;
  int imageWidth = current.cols;

  int labelHeight = ui->imageLabel->height();
  int labelWidth = ui->imageLabel->width();

  if (ui->fitToScreenCheckBox->isChecked()) {
    int scaledImageWidth, scaledImageHeight;

    if (labelWidth * imageHeight > imageWidth * labelHeight) {
      scaledImageWidth = imageWidth * labelHeight / imageHeight;
      scaledImageHeight = labelHeight;
    } else {
      scaledImageWidth = labelWidth;
      scaledImageHeight = imageHeight * labelWidth / imageWidth;
    }

    x = (x - (labelWidth - scaledImageWidth) / 2) * imageWidth / scaledImageWidth;
    y = (y - (labelHeight - scaledImageHeight) / 2) * imageHeight / scaledImageHeight;
  } else if (imageWidth < labelWidth && imageHeight < labelHeight) {
    x -= (labelWidth - imageWidth) / 2;
    y -= (labelHeight - imageHeight) / 2;
  }

  if (x < 0)
    x = 0;
  else if (x >= imageWidth)
    x = imageWidth - 1;

  if (y < 0)
    y = 0;
  else if (y >= imageHeight)
    y = imageHeight - 1;

  p.setX(x);
  p.setY(y);
}
