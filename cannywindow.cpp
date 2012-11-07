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

#include "cannywindow.h"
#include "ui_cannywindow.h"

#include "image.h"

#include <opencv2/imgproc/imgproc.hpp>

CannyWindow::CannyWindow(Image* image,
                         QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::CannyWindow),
  image(image),
  abort(true)
{
  ui->setupUi(this);

  image->backup();

  connect(this,   SIGNAL(update()),
          image,  SLOT(update()));

  this->setAttribute(Qt::WA_DeleteOnClose);

  ui->manualGroupBox->hide();
  this->adjustSize();
  this->setFixedSize(this->size());

  this->show();

  canny();
}

CannyWindow::~CannyWindow()
{
  delete ui;
}

void CannyWindow::closeEvent(QCloseEvent *)
{
  if (abort)
    image->undo();
}

void CannyWindow::on_cancelPushButton_clicked()
{
  this->close();
}

void CannyWindow::on_okPushButton_clicked()
{
  abort = false;

  this->close();
}

void CannyWindow::on_l1RadioButton_toggled(bool checked)
{
  if (checked)
    canny();
}

void CannyWindow::on_l2RadioButton_toggled(bool checked)
{
  if (checked)
    canny();
}

void CannyWindow::on_manualRadioButton_toggled(bool checked)
{
  if (checked) {
    ui->manualGroupBox->show();

    canny();
  } else
    ui->manualGroupBox->hide();
}

void CannyWindow::on_meanRadioButton_toggled(bool checked)
{
  if (checked)
    canny();
}

void CannyWindow::on_medianRadioButton_toggled(bool checked)
{
  if (checked)
    canny();
}

void CannyWindow::on_semiAutomaticRadioButton_toggled(bool checked)
{
  if (checked) {
    ui->semiAutomaticGroupBox->show();

    canny();
  } else
    ui->semiAutomaticGroupBox->hide();
}

void CannyWindow::on_maximumSlider_valueChanged(int value)
{
  if (ui->minimumSlider->value() > value)
    ui->minimumSlider->setValue(value);

  canny();
}

void CannyWindow::on_minimumSlider_valueChanged(int value)
{
  if (ui->maximumSlider->value() < value)
    ui->maximumSlider->setValue(value);

  canny();
}


void CannyWindow::on_sizeSpinBox_valueChanged(int)
{
  canny();
}

void CannyWindow::canny()
{
  int size = ui->sizeSpinBox->value();
  bool l2norm = ui->l2RadioButton->isChecked();

  if (ui->manualRadioButton->isChecked()) {
    cv::Canny(image->previous,
              image->current,
              ui->minimumSlider->value(),
              ui->maximumSlider->value(),
              size,
              l2norm);
  } else if (ui->meanRadioButton->isChecked()) {
    double mean = calculateMean();

    cv::Canny(image->previous,
              image->current,
              2 * mean / 3,
              4 * mean / 3,
              size,
              l2norm);
  } else {
    double median = calculateMedian();

    cv::Canny(image->previous,
              image->current,
              2 * median / 3,
              4 * median / 3,
              size,
              l2norm);
  }

  emit update();
}

double CannyWindow::calculateMean()
{
  return cv::mean(image->previous)[0];
}

double CannyWindow::calculateMedian()
{
  cv::Mat histogram;
  int acc = 0;
  int const size = image->previous.rows * image->previous.cols;

  std::vector<cv::Mat> images;
  std::vector<int> channels;
  std::vector<int> histSize;
  std::vector<float> ranges;

  images.push_back(image->previous);
  channels.push_back(0);
  histSize.push_back(256);
  ranges.push_back(0);
  ranges.push_back(256);

  cv::calcHist(images, channels, cv::Mat(), histogram, histSize, ranges);

  for (int i = 0; i < 256; i++) {
    acc += histogram.at<float>(i);
    if (acc > size / 2)
      return i;
  }

  return 0;
}
