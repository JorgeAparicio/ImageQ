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

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>

#include "histogram.h"
#include "mat2qimage.h"

// TODO: Batch processing
// TODO: Color image processing
// TODO: Image "feedback"
// TODO: Macroing
// TODO: Particle counting
// TODO: Region of Interest (a.k.a. masking)
// TODO: Video processing...?
// TODO: Watershed

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  aboutWindow(0),
  cannyWindow(0),
  gradientWindow(0),
  histogramWindow(0),
  morphologyWindow(0),
  thresholdWindow(0)
{
  ui->setupUi(this);

  ui->mainToolBar->hide();
  ui->statusBar->hide();

  this->showMaximized();
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::on_actionAbout_triggered()
{
  aboutWindow = new AboutWindow(this);
}

void MainWindow::on_actionBlur_triggered()
{
  if (images.size() > 0) {
    Image* image = (Image*)ui->imagesTabWidget->currentWidget();

    if (image->current.channels() == 1)
      blurWindow = new BlurWindow(image, this);
  }
}

void MainWindow::on_actionCanny_triggered()
{
  if (images.size() > 0) {
    Image* image = (Image*)ui->imagesTabWidget->currentWidget();

    if (image->current.channels() == 1)
      cannyWindow = new CannyWindow(image, this);
  }
}

void MainWindow::on_actionClose_triggered()
{
  this->close();
}

void MainWindow::on_actionEqualize_triggered()
{
  if (images.size() > 0) {
    Image* image = (Image*)ui->imagesTabWidget->currentWidget();

    if (image->current.channels() == 1) {
      image->backup();

      cv::equalizeHist(image->previous, image->current);

      image->update();
    }
  }
}

void MainWindow::on_actionGradient_triggered()
{
  if (images.size() > 0) {
    Image* image = (Image*)ui->imagesTabWidget->currentWidget();

    if (image->current.channels() == 1)
      gradientWindow = new GradientWindow(image, this);
  }
}

void MainWindow::on_actionHistogram_triggered()
{
  if (images.size() > 0) {
    Image* image = (Image*)ui->imagesTabWidget->currentWidget();

    if (image->current.channels() == 1)
      histogramWindow = new HistogramWindow(image->current, this);
  }
}

void MainWindow::on_actionInvert_triggered()
{
  if (images.size() > 0) {
    Image* image = (Image*)ui->imagesTabWidget->currentWidget();

    if (image->current.channels() == 1) {
      image->backup();

      image->current = 255 - image->current;

      image->update();
    }
  }
}

void MainWindow::on_actionMorphology_triggered()
{
  if (images.size() > 0) {
    Image* image = (Image*)ui->imagesTabWidget->currentWidget();

    if (image->current.channels() == 1)
      morphologyWindow = new MorphologyWindow(image, this);
  }
}

void MainWindow::on_actionOpen_triggered()
{
  QString filename = QFileDialog::getOpenFileName(this, "Open");

  if (!filename.isEmpty()) {
    images.push_back(new Image(filename, this));

#ifdef Q_OS_WIN32
    int index = filename.lastIndexOf("\\");
#else
    int index = filename.lastIndexOf("/");
#endif

    if (index != -1)
      filename = filename.right(filename.length() - 1 - index);

    ui->imagesTabWidget->addTab(images.last(), filename);
  }
}

void MainWindow::on_actionRevert_triggered()
{
  Image* image = (Image*)ui->imagesTabWidget->currentWidget();

  image->revert();
}

void MainWindow::on_actionSave_triggered()
{
  if (images.size() > 0) {
    Image* image = (Image*)(ui->imagesTabWidget->currentWidget());

    QString filename = QFileDialog::getSaveFileName(this, QLatin1String("Save"));

    if (!filename.isEmpty()) {
      std::vector<int> qualityType;

      qualityType.push_back(CV_IMWRITE_JPEG_QUALITY);
      qualityType.push_back(90);

      filename += ".jpg";

      cv::imwrite(filename.toStdString(), image->current, qualityType);
    }
  }
}

void MainWindow::on_actionStretch_triggered()
{
  if (images.size() > 0) {
    Image* image = (Image*)(ui->imagesTabWidget->currentWidget());

    image->backup();

    cv::normalize(image->previous, image->current, 0, 255, cv::NORM_MINMAX);

    image->update();
  }
}

void MainWindow::on_actionThreshold_triggered()
{
  if (images.size() > 0) {
    Image* image = (Image*)(ui->imagesTabWidget->currentWidget());

    if (image->current.channels() == 1)
      thresholdWindow = new ThresholdWindow(image, this);
  }
}

void MainWindow::on_actionUndo_triggered()
{
  Image* image = (Image*)ui->imagesTabWidget->currentWidget();

  image->undo();
}

void MainWindow::on_imagesTabWidget_currentChanged(QWidget *image)
{
  ((Image*)image)->update();
}

void MainWindow::on_imagesTabWidget_tabCloseRequested(int index)
{
  // FIXME: Can't delete last element of QTabWidget.
  if (images.size() > 1) {
    index = images.indexOf((Image*)(ui->imagesTabWidget->widget(index)));

    delete images.at(index);
    images.remove(index);
  }
}
