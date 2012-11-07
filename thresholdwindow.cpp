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

#include "thresholdwindow.h"
#include "ui_thresholdwindow.h"

#include "image.h"

#include <opencv2/imgproc/imgproc.hpp>

#include <qwt_plot_marker.h>

ThresholdWindow::ThresholdWindow(Image* image,
                                 QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::ThresholdWindow),
  image(image),
  histogram(image->current, 256),
  yAxis(0),
  abort(true)
{
  ui->setupUi(this);

  image->backup();

  connect(this,   SIGNAL(update()),
          image,  SLOT(update()));

  histogram.attach(ui->histogramPlot);

  ui->histogramPlot->setAxisAutoScale(QwtPlot::xBottom, false);
  ui->histogramPlot->setAxisScale(QwtPlot::xBottom, 0, 256);

  ui->histogramPlot->enableAxis(QwtPlot::xBottom, false);
  ui->histogramPlot->enableAxis(QwtPlot::yLeft, false);

  this->setAttribute(Qt::WA_DeleteOnClose);

  ui->histogramPlot->setFixedSize(480, 200);
  ui->adaptativeFrame->hide();
  this->adjustSize();
  this->setFixedSize(this->size());

  this->show();

  threshold();
}

ThresholdWindow::~ThresholdWindow()
{
  delete ui;
}

void ThresholdWindow::closeEvent(QCloseEvent *)
{
  if (abort)
    image->undo();
}

void ThresholdWindow::on_cancelPushButton_clicked()
{
  this->close();
}

void ThresholdWindow::on_okPushButton_clicked()
{
  abort = false;

  this->close();
}

void ThresholdWindow::on_thresholdSlider_sliderMoved(int value)
{
  if (yAxis)  {
    delete yAxis;
    yAxis = 0;
  }

  if (!ui->adaptativeCheckBox->isChecked()) {
    yAxis = new QwtPlotMarker;

    yAxis->setLineStyle(QwtPlotMarker::VLine);
    yAxis->setLinePen(QPen(Qt::red));
    yAxis->setXValue(value + 0.5);
    yAxis->attach(ui->histogramPlot);

    ui->histogramPlot->replot();
  }
  threshold();
}

void ThresholdWindow::on_adaptativeCheckBox_toggled(bool checked)
{
  ui->otsuCheckBox->setDisabled(checked);

  if (checked) {
    if (yAxis)  {
      delete yAxis;
      yAxis = 0;

      ui->histogramPlot->replot();
    }

    ui->otsuCheckBox->setChecked(false);

    ui->adaptativeFrame->show();
    ui->nonAdaptativeFrame->hide();

    ui->thresholdSlider->setMinimum(-128);
    ui->thresholdSlider->setMaximum(127);
  } else {
    ui->adaptativeFrame->hide();
    ui->nonAdaptativeFrame->show();

    ui->thresholdSlider->setMinimum(0);
    ui->thresholdSlider->setMaximum(255);
  }

  ui->thresholdSlider->setValue(0);

  threshold();
}

void ThresholdWindow::on_invertedCheckBox_toggled(bool)
{
  threshold();
}

void ThresholdWindow::on_otsuCheckBox_toggled(bool checked)
{
  ui->thresholdSlider->setDisabled(checked);

  threshold();
}

void ThresholdWindow::on_binaryRadioButton_toggled(bool checked)
{
  if (checked)
    threshold();
}

void ThresholdWindow::on_gaussianRadioButton_toggled(bool checked)
{
  if (checked)
    threshold();
}

void ThresholdWindow::on_meanRadioButton_toggled(bool checked)
{
  if (checked)
    threshold();
}

void ThresholdWindow::on_toZeroRadioButton_toggled(bool checked)
{
  if (checked)
    threshold();
}

void ThresholdWindow::on_truncateRadioButton_toggled(bool checked)
{
  ui->invertedCheckBox->setDisabled(checked);

  if (checked) {
    ui->invertedCheckBox->setChecked(false);

    threshold();
  }
}

void ThresholdWindow::threshold()
{
  if (ui->adaptativeCheckBox->isChecked()) {
    int method = 0;
    int type = 0;

    if (ui->meanRadioButton->isChecked())
      method = cv::ADAPTIVE_THRESH_MEAN_C;
    else
      method = cv::ADAPTIVE_THRESH_GAUSSIAN_C;

    if (ui->invertedCheckBox->isChecked())
      type = cv::THRESH_BINARY_INV;
    else
      type = cv::THRESH_BINARY;

    cv::adaptiveThreshold(image->previous,
                          image->current,
                          255.0,
                          method,
                          type,
                          ui->sizeSpinBox->value(),
                          ui->thresholdSlider->value()
                          );
  } else {
    int type = 0;

    if (ui->binaryRadioButton->isChecked()) {
      if (ui->invertedCheckBox->isChecked())
        type = cv::THRESH_BINARY_INV;
      else
        type = cv::THRESH_BINARY;
    } else if (ui->truncateRadioButton->isChecked()) {
      type = cv::THRESH_TRUNC;
    } else if (ui->toZeroRadioButton->isChecked()) {
      if (ui->invertedCheckBox->isChecked())
        type = cv::THRESH_TOZERO_INV;
      else
        type = cv::THRESH_TOZERO;
    }

    if (ui->otsuCheckBox->isChecked())
      type |= cv::THRESH_OTSU;

    cv::threshold(image->previous,
                  image->current,
                  ui->thresholdSlider->value(),
                  255.0,
                  type);
  }

  emit update();
}

void ThresholdWindow::on_sizeSpinBox_valueChanged(int)
{
  threshold();
}
