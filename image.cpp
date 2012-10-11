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

Image::Image(QString pathToImage, QWidget *parent) :
  QWidget(parent),
  ui(new Ui::Image)
{
  ui->setupUi(this);

  first = cv::imread(pathToImage.toStdString(), CV_LOAD_IMAGE_GRAYSCALE);
  first.copyTo(current);
}

Image::Image(cv::Mat const& image, QWidget *parent) :
  QWidget(parent),
  ui(new Ui::Image)
{
  ui->setupUi(this);

  image.copyTo(first);
  first.copyTo(current);
}

Image::~Image()
{
  delete ui;

  current.release();
  previous.release();
  first.release();
}

void Image::backup()
{
  current.copyTo(previous);
}

void Image::revert()
{
  first.copyTo(current);
  previous.release();

  update();
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

  QPixmap pixmap = QPixmap::fromImage(Mat2QImage(current));

  if (ui->fitToScreenCheckBox->isChecked())
    pixmap = pixmap.scaled(ui->displayLabel->size(), Qt::KeepAspectRatio);

  ui->displayLabel->clear();
  ui->displayLabel->setPixmap(pixmap);

}

void Image::on_fitToScreenCheckBox_toggled()
{
  update();
}
