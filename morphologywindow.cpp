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

#include "morphologywindow.h"
#include "ui_morphologywindow.h"

#include "mat2qimage.h"

MorphologyWindow::MorphologyWindow(cv::Mat const& image,
                                   cv::Mat const& backup,
                                   QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MorphologyWindow),
  image(image),
  backup(backup),
  abort(true)
{
  ui->setupUi(this);

  this->setAttribute(Qt::WA_DeleteOnClose);
  this->setFixedSize(this->size());

  this->show();

  updateStructuringElement();
  morphology();
}

MorphologyWindow::~MorphologyWindow()
{
  delete ui;
}

void MorphologyWindow::closeEvent(QCloseEvent *)
{
  if (abort)
    backup.copyTo(image);

  emit updatedImage();
}

void MorphologyWindow::on_cancelPushButton_clicked()
{
  this->close();
}

void MorphologyWindow::on_okPushButton_clicked()
{
  abort = false;

  this->close();
}

void MorphologyWindow::on_closeRadioButton_toggled(bool checked)
{
  ui->iterationsSpinBox->setDisabled(checked);

  if (checked) {
    ui->iterationsSpinBox->setValue(1);

    morphology();
  }
}

void MorphologyWindow::on_crossRadioButton_toggled(bool checked)
{
  if (checked) {
    updateStructuringElement();

    morphology();
  }
}

void MorphologyWindow::on_customRadioButton_toggled(bool checked)
{
  if (checked) {
    updateStructuringElement();

    morphology();
  }
}

void MorphologyWindow::on_dilateRadioButton_toggled(bool checked)
{
  ui->iterationsSpinBox->setEnabled(checked);

  if (checked)
    morphology();
}

void MorphologyWindow::on_diskRadioButton_toggled(bool checked)
{
  if (checked) {
    updateStructuringElement();

    morphology();
  }
}

void MorphologyWindow::on_erodeRadioButton_toggled(bool checked)
{
  ui->iterationsSpinBox->setEnabled(checked);

  if (checked)
    morphology();
}

void MorphologyWindow::on_openRadioButton_toggled(bool checked)
{
  ui->iterationsSpinBox->setDisabled(checked);

  if (checked) {
    ui->iterationsSpinBox->setValue(1);

    morphology();
  }
}

void MorphologyWindow::on_squareRadioButton_toggled(bool checked)
{
  if (checked) {
    updateStructuringElement();

    morphology();
  }
}

void MorphologyWindow::on_xRadioButton_toggled(bool checked)
{
  if (checked) {
    updateStructuringElement();

    morphology();
  }
}

void MorphologyWindow::on_iterationsSpinBox_valueChanged(int)
{
  morphology();
}

void MorphologyWindow::on_sizeSpinBox_valueChanged(int)
{
  updateStructuringElement();

  morphology();
}

void MorphologyWindow::morphology()
{
  int iterations = ui->iterationsSpinBox->value();

  if (ui->closeRadioButton->isChecked()) {
    cv::dilate(backup, image, structuringElement);
    cv::erode(image, image, structuringElement);
  } else if (ui->dilateRadioButton->isChecked()) {
    cv::dilate(backup,
               image,
               structuringElement,
               cv::Point(-1, -1),
               iterations);
  } else if (ui->erodeRadioButton->isChecked()) {
    cv::erode(backup,
              image,
              structuringElement,
              cv::Point(-1, -1),
              iterations);
  } else if (ui->openRadioButton->isChecked()) {
    cv::erode(backup, image, structuringElement);
    cv::dilate(image, image, structuringElement);
  }

  emit updatedImage();
}

void MorphologyWindow::updateStructuringElement()
{
  int size = ui->sizeSpinBox->value();

  if (ui->squareRadioButton->isChecked()) {
    structuringElement = cv::Mat::ones(size, size, CV_8U) * 255;
  } else {
    structuringElement = cv::Mat::zeros(size, size, CV_8U);

    int center = (size - 1) / 2;

    if (ui->customRadioButton->isChecked()) {
      // TODO
      structuringElement.at<quint8>(center, center) = 255;
    } else if (ui->crossRadioButton->isChecked()) {
      for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
          if ((i == center) || (j == center))
            structuringElement.at<quint8>(i, j) = 255;
    } else if (ui->diskRadioButton->isChecked()) {
      int squaredRadius = center * center;

      for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
          if (((i - center) * (i - center) +
               (j - center) * (j - center)) <=
              squaredRadius)
            structuringElement.at<quint8>(i, j) = 255;
    } else if (ui->xRadioButton->isChecked()) {
      for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
          if ((i == j) || (i == (size - j - 1)))
            structuringElement.at<quint8>(i, j) = 255;
    }
  }

  QPixmap pixmap = QPixmap::fromImage(Mat2QImage(structuringElement));
  QSize labelSize = ui->structuringElementLabel->size();

  ui->structuringElementLabel->setPixmap(pixmap.scaled(labelSize,
                                                       Qt::KeepAspectRatio));
}
