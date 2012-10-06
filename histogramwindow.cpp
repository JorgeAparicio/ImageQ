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

#include "histogramwindow.h"
#include "ui_histogramwindow.h"

HistogramWindow::HistogramWindow(cv::Mat const& image, QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::HistogramWindow),
  histogram(image, 256)
{
  ui->setupUi(this);

  histogram.attach(ui->histogramPlot);

  ui->histogramPlot->setAxisAutoScale(QwtPlot::xBottom, false);
  ui->histogramPlot->setAxisScale(QwtPlot::xBottom, 0, 256);

  ui->histogramPlot->enableAxis(QwtPlot::xBottom, false);
  ui->histogramPlot->enableAxis(QwtPlot::yLeft, false);

  this->setAttribute(Qt::WA_DeleteOnClose);
  this->setFixedSize(this->size());

  this->show();
}

HistogramWindow::~HistogramWindow()
{
  delete ui;
}
