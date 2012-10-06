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

#include "gradientwindow.h"
#include "ui_gradientwindow.h"

GradientWindow::GradientWindow(cv::Mat const& image,
                               cv::Mat const& backup,
                               QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::GradientWindow),
  image(image),
  backup(backup),
  abort(true)
{
  ui->setupUi(this);

  this->setAttribute(Qt::WA_DeleteOnClose);
  this->setFixedSize(this->size());

  this->show();

  gradient();
}

GradientWindow::~GradientWindow()
{
  delete ui;
}

void GradientWindow::closeEvent(QCloseEvent *)
{
  if (abort)
    backup.copyTo(image);

  emit updatedImage();
}

void GradientWindow::on_cancelPushButton_clicked()
{
  this->close();
}

void GradientWindow::on_okPushButton_clicked()
{
  abort = false;

  this->close();
}

void GradientWindow::on_absoluteCheckBox_toggled(bool)
{
  gradient();
}

void GradientWindow::on_laplacianRadioButton_toggled(bool checked)
{
  ui->magnitudeCheckBox->setDisabled(checked);
  ui->xSpinBox->setDisabled(checked);
  ui->ySpinBox->setDisabled(checked);

  if (checked) {
    ui->magnitudeCheckBox->setChecked(false);
    ui->sizeSpinBox->setMinimum(1);

    gradient();
  } else {
    ui->sizeSpinBox->setMinimum(3);
  }
}

void GradientWindow::on_magnitudeCheckBox_toggled(bool checked)
{
  ui->absoluteCheckBox->setDisabled(checked);
  ui->xSpinBox->setDisabled(checked);
  ui->ySpinBox->setDisabled(checked);

  if (checked) {
    ui->absoluteCheckBox->setChecked(false);
  }

  gradient();
}

void GradientWindow::on_scharrRadioButton_toggled(bool checked)
{
  ui->sizeSpinBox->setDisabled(checked);

  if (checked) {
    ui->xSpinBox->setMaximum(1);
    ui->ySpinBox->setMaximum(1);

    ui->sizeSpinBox->setValue(3);

    ui->xSpinBox->setValue(1);

    gradient();
  }
}

void GradientWindow::on_sobelRadioButton_toggled(bool checked)
{
  if (checked) {
    ui->xSpinBox->setMaximum(2);
    ui->ySpinBox->setMaximum(2);

    gradient();
  }
}

void GradientWindow::on_xSpinBox_valueChanged(int value)
{
  if (ui->sobelRadioButton->isChecked()) {
    if (value)
      ui->ySpinBox->setMinimum(0);
    else
      ui->ySpinBox->setMinimum(1);
  } else {
    ui->ySpinBox->setMinimum(0);
    ui->ySpinBox->setValue(1 - value);
  }

  gradient();
}

void GradientWindow::on_ySpinBox_valueChanged(int value)
{
  if (ui->sobelRadioButton->isChecked()) {
    if (value)
      ui->xSpinBox->setMinimum(0);
    else
      ui->xSpinBox->setMinimum(1);
  } else {
    ui->xSpinBox->setMinimum(0);
    ui->xSpinBox->setValue(1 - value);
  }

  gradient();
}

void GradientWindow::on_sizeSpinBox_valueChanged(int)
{
  gradient();
}

void GradientWindow::gradient()
{
  cv::Mat tmp;
  int size = ui->sizeSpinBox->value();
  int dx = ui->xSpinBox->value();
  int dy = ui->ySpinBox->value();

  backup.convertTo(tmp, CV_32F, 1.0/255.0);

  if (ui->laplacianRadioButton->isChecked()) {
    cv::Laplacian(tmp, tmp, CV_32F, size);
  } else if (ui->magnitudeCheckBox->isChecked()) {
    cv::Mat derivative;
    cv::Mat magnitude = cv::Mat::zeros(tmp.rows, tmp.cols, CV_32F);

    if (ui->sobelRadioButton->isChecked())
      cv::Sobel(tmp, derivative, CV_32F, 1, 0, size);
    else
      cv::Scharr(tmp, derivative, CV_32F, 1, 0);

    cv::accumulateSquare(derivative, magnitude);

    if (ui->sobelRadioButton->isChecked())
      cv::Sobel(tmp, derivative, CV_32F, 0, 1, size);
    else
      cv::Scharr(tmp, derivative, CV_32F, 0, 1);

    cv::accumulateSquare(derivative, magnitude);

    cv::sqrt(magnitude, tmp);
  } else if (ui->scharrRadioButton->isChecked()) {
    cv::Scharr(tmp, tmp, CV_32F, dx, dy);
  } else if (ui->sobelRadioButton->isChecked()) {
    cv::Sobel(tmp, tmp, CV_32F, dx, dy, size);
  }

  if (ui->absoluteCheckBox->isChecked())
    tmp = cv::abs(tmp);

  cv::normalize(tmp, tmp, 0, 1, cv::NORM_MINMAX);

  tmp.convertTo(image, CV_8U, 255.0);

  emit updatedImage();
}
