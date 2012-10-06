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

// TODO: Adaptative thresholding
// TODO: Batch processing
// TODO: Color image processing
// TODO: Image "feedback"
// TODO: Macroing
// TODO: Particle counting
// TODO: Region of Interest (a.k.a. masking)
// TODO: Video processing...?
// TODO: Watershed
// TODO: Windows support

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

void MainWindow::showImage()
{
  double min, max;
  QPixmap pixmap;

  ui->imageLabel->clear();

  if (currentImage.data == 0) {
    ui->imageLabel->setText("Your image goes here.");

    ui->sizeLabel->setText("0x0");
    ui->channelsLabel->setText("-");
    ui->depthLabel->setText("-");
    ui->minLabel->setText("0");
    ui->maxLabel->setText("0");
  } else {
    cv::minMaxLoc(currentImage, &min, &max);

    pixmap = QPixmap::fromImage(Mat2QImage(currentImage));

    ui->sizeLabel->setText(QString::number(currentImage.rows) + "x"
                           + QString::number(currentImage.cols));
    ui->minLabel->setText(QString::number(min));
    ui->maxLabel->setText(QString::number(max));

    switch (currentImage.channels()) {
      case 1:
        ui->channelsLabel->setText("1");
        break;
      case 3:
        ui->channelsLabel->setText("3");
        break;
    }

    switch (currentImage.depth()) {
      case CV_8U:
        ui->depthLabel->setText("8 bits");
        break;
      case CV_32F:
        ui->depthLabel->setText("32 bits");
        break;
    }

    if (ui->fitToScreenCheckBox->isChecked())
      ui->imageLabel->setPixmap(pixmap.scaled(ui->imageLabel->size(),
                                              Qt::KeepAspectRatio));
    else
      ui->imageLabel->setPixmap(pixmap);
  }
}

void MainWindow::on_fitToScreenCheckBox_toggled(bool)
{
  showImage();
}

void MainWindow::on_actionAbout_triggered()
{
  aboutWindow = new AboutWindow(this);
}

void MainWindow::on_actionBlur_triggered()
{
  if (currentImage.data != 0) {
    currentImage.copyTo(previousImage);

    blurWindow = new BlurWindow(currentImage, previousImage, this);

    connect(blurWindow, SIGNAL(updatedImage()),
            this,       SLOT(showImage()));

    showImage();
  }
}

void MainWindow::on_actionCanny_triggered()
{
  if (currentImage.data != 0) {
    currentImage.copyTo(previousImage);

    cannyWindow = new CannyWindow(currentImage, previousImage, this);

    connect(cannyWindow,  SIGNAL(updatedImage()),
            this,         SLOT(showImage()));

    showImage();
  }
}

void MainWindow::on_actionClose_triggered()
{
  this->close();
}

void MainWindow::on_actionEqualize_triggered()
{
  if (currentImage.data != 0) {
    currentImage.copyTo(previousImage);

    cv::equalizeHist(currentImage, currentImage);

    showImage();
  }
}

void MainWindow::on_actionGradient_triggered()
{
  if (currentImage.data != 0) {
    currentImage.copyTo(previousImage);

    gradientWindow = new GradientWindow(currentImage, previousImage, this);

    connect(gradientWindow, SIGNAL(updatedImage()),
            this,           SLOT(showImage()));

    showImage();
  }
}

void MainWindow::on_actionHistogram_triggered()
{
  if (currentImage.data != 0)
    histogramWindow = new HistogramWindow(currentImage, this);
}

void MainWindow::on_actionInvert_triggered()
{
  if (currentImage.data != 0) {
    currentImage.copyTo(previousImage);

    currentImage = 255 - previousImage;

    showImage();
  }
}

void MainWindow::on_actionMorphology_triggered()
{
  if (currentImage.data != 0) {
    currentImage.copyTo(previousImage);

    morphologyWindow = new MorphologyWindow(currentImage, previousImage, this);

    connect(morphologyWindow, SIGNAL(updatedImage()),
            this,             SLOT(showImage()));

    showImage();
  }
}

void MainWindow::on_actionOpen_triggered()
{
  imagePath = QFileDialog::getOpenFileName(this,
                                           QLatin1String("Open"));

  if (!imagePath.isEmpty()) {
    previousImage.release();

    currentImage = cv::imread(imagePath.toStdString(), CV_LOAD_IMAGE_GRAYSCALE);

    showImage();
  }
}

void MainWindow::on_actionRevert_triggered()
{
  if (!imagePath.isEmpty()) {
    previousImage.release();

    currentImage = cv::imread(imagePath.toStdString(), CV_LOAD_IMAGE_GRAYSCALE);

    showImage();
  }
}

void MainWindow::on_actionSave_triggered()
{
  QString savePath = QFileDialog::getSaveFileName(this, QLatin1String("Save"));

  if (!savePath.isEmpty() && currentImage.data != 0) {
    std::vector<int> qualityType;

    qualityType.push_back(CV_IMWRITE_JPEG_QUALITY);
    qualityType.push_back(90);

    savePath += ".jpg";

    cv::imwrite(savePath.toStdString(), currentImage, qualityType);
  }
}

void MainWindow::on_actionStretch_triggered()
{
  if (currentImage.data != 0) {
    currentImage.copyTo(previousImage);

    cv::normalize(currentImage, currentImage, 0, 255, cv::NORM_MINMAX);

    showImage();
  }
}

void MainWindow::on_actionThreshold_triggered()
{
  if (currentImage.data != 0) {
    currentImage.copyTo(previousImage);

    thresholdWindow = new ThresholdWindow(currentImage, previousImage, this);

    connect(thresholdWindow,  SIGNAL(updatedImage()),
            this,             SLOT(showImage()));

    showImage();
  }
}

void MainWindow::on_actionUndo_triggered()
{
  if (previousImage.data != 0) {
    previousImage.copyTo(currentImage);

    showImage();
  }
}
