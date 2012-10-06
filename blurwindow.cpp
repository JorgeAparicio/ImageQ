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

#include "blurwindow.h"
#include "ui_blurwindow.h"

BlurWindow::BlurWindow(cv::Mat const& image,
                       cv::Mat const& backup,
                       QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::BlurWindow),
  image(image),
  backup(backup),
  abort(true)
{
  ui->setupUi(this);

  this->setAttribute(Qt::WA_DeleteOnClose);
  this->setFixedSize(this->size());

  this->show();

  blur();
}

BlurWindow::~BlurWindow()
{
  delete ui;
}

void BlurWindow::closeEvent(QCloseEvent *)
{
  if (abort)
    backup.copyTo(image);

  emit updatedImage();
}

void BlurWindow::on_cancelPushButton_clicked()
{
  this->close();
}

void BlurWindow::on_okPushButton_clicked()
{
  abort = false;

  this->close();
}

void BlurWindow::on_averageRadioButton_toggled(bool checked)
{
  if (checked)
    blur();
}

void BlurWindow::on_gaussianRadioButton_toggled(bool checked)
{
  if (checked)
    blur();
}

void BlurWindow::on_medianRadioButton_toggled(bool checked)
{
  if (checked)
    blur();
}

void BlurWindow::on_sizeSpinBox_valueChanged(int)
{
  blur();
}

void BlurWindow::blur()
{
  int size = ui->sizeSpinBox->value();

  if (ui->averageRadioButton->isChecked()) {
    cv::blur(backup, image, cv::Size(size, size));
  } else if (ui->gaussianRadioButton->isChecked()) {
    cv::GaussianBlur(backup, image, cv::Size(size, size), 0);
  } else {
    cv::medianBlur(backup, image, size);
  }

  emit updatedImage();
}
