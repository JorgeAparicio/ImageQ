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

Image::Image(QString pathToImage, QWidget *parent) :
  QWidget(parent),
  ui(new Ui::Image),
  first(cv::imread(pathToImage.toStdString()))
{
  ui->setupUi(this);

  first.copyTo(current);

  connect(ui->imageLabel, SIGNAL(hover(int,int)),
          this,           SLOT(info(int,int)));

  connect(ui->imageLabel, SIGNAL(resized()),
          this,           SLOT(rescale()));

  connect(ui->scrollArea->horizontalScrollBar(),  SIGNAL(rangeChanged(int,int)),
          this,                                   SLOT(rescale()));

  connect(ui->scrollArea->verticalScrollBar(),  SIGNAL(rangeChanged(int,int)),
          this,                                   SLOT(rescale()));
}

Image::Image(cv::Mat const& image, QWidget *parent) :
  QWidget(parent),
  ui(new Ui::Image),
  first(image)
{
  ui->setupUi(this);

  first.copyTo(current);

  connect(ui->imageLabel, SIGNAL(hover(int,int)),
          this,           SLOT(info(int,int)));

  connect(ui->imageLabel, SIGNAL(resized()),
          this,           SLOT(rescale()));

  connect(ui->scrollArea->horizontalScrollBar(),  SIGNAL(rangeChanged(int,int)),
          this,                                   SLOT(dummy(int,int)));

  connect(ui->scrollArea->verticalScrollBar(),  SIGNAL(rangeChanged(int,int)),
          this,                                   SLOT(dummy(int,int)));
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

  draw();
}

void Image::draw()
{
  pixmap = QPixmap::fromImage(Mat2QImage(current));

  if (ui->fitToScreenCheckBox->isChecked())
    pixmap = pixmap.scaled(ui->imageLabel->size(), Qt::KeepAspectRatio);

  ui->imageLabel->setPixmap(pixmap);
}

void Image::rescale()
{
  if (ui->fitToScreenCheckBox->isChecked()) {
    if (ui->scrollArea->horizontalScrollBar()->maximum() == 0 &&
        ui->scrollArea->verticalScrollBar()->maximum() == 0 &&
        pixmap.width() != ui->imageLabel->width() &&
        pixmap.height() != ui->imageLabel->height()) {
        draw();
    } else if (pixmap.height() != 0 && pixmap.width() != 0) {
      ui->imageLabel->clear();
      pixmap = QPixmap();
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
    draw();
}

void Image::info(int x, int y) const
{
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

  if (x >= 0 && x < imageWidth && y >= 0 && y < imageHeight) {
    ui->xLabel->setText(QString::number(x));
    ui->yLabel->setText(QString::number(y));

    switch(current.channels()) {
      case 1:
        ui->valueLabel->setText(QString::number(current.at<quint8>(y, x)));
        break;
      case 3:
        cv::Vec3b bgr = current.at<cv::Vec3b>(y, x);
        ui->valueLabel->setText("(" + QString::number(bgr[2]) +
                                ", " + QString::number(bgr[1]) +
                                ", " + QString::number(bgr[0]) + ")");
        break;
    }
  } else {
    ui->xLabel->setText("-");
    ui->yLabel->setText("-");
    ui->valueLabel->setText("-");
  }
}
