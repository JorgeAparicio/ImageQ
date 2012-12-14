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

#include "image.h"

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <opencv_future/imgproc/connectedcomponents.hpp>

#include "aboutwindow.h"
#include "blurwindow.h"
#include "cannywindow.h"
#include "gradientwindow.h"
#include "histogramwindow.h"
#include "morphologywindow.h"
#include "thresholdwindow.h"
#include "setscalewindow.h"

// TODO: Batch processing
// TODO: Channels merge
// TODO: Macroing
// TODO: Video processing...?
// TODO: Watershed

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  workingImage(0),
  aboutWindow(0),
  cannyWindow(0),
  gradientWindow(0),
  histogramWindow(0),
  morphologyWindow(0),
  thresholdWindow(0)
{
  ui->setupUi(this);

  ui->mainToolBar->hide();

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
  if (workingImage) {
    if (workingImage->current.channels() == 1) {
      disableOtherTabs();
      setOperationsEnabled(false);

      blurWindow = new BlurWindow(workingImage, this);

      connect(blurWindow, SIGNAL(destroyed()),
              this,       SLOT(enableAllTabs()));

      connect(blurWindow, SIGNAL(destroyed()),
              this,       SLOT(enableAllOperations()));
    }
  }
}

void MainWindow::on_actionCanny_triggered()
{
  if (workingImage) {
    if (workingImage->current.channels() == 1) {
      disableOtherTabs();
      setOperationsEnabled(false);

      cannyWindow = new CannyWindow(workingImage, this);

      connect(cannyWindow,  SIGNAL(destroyed()),
              this,         SLOT(enableAllTabs()));

      connect(cannyWindow,  SIGNAL(destroyed()),
              this,         SLOT(enableAllOperations()));
    }
  }
}

void MainWindow::on_actionCrop_triggered()
{
  if (workingImage) {
    disableOtherTabs();
    setOperationsEnabled(false);

    connect(workingImage, SIGNAL(rectangleSelected(QRect)),
            this,         SLOT(crop(QRect)));

    connect(workingImage,   SIGNAL(status(QString)),
            ui->statusBar,  SLOT(showMessage(QString)));

    workingImage->setSelectionMode(Image::Rectangle);
  }
}

void MainWindow::on_actionClear_triggered()
{
  workingImage->clearOverlay();
  workingImage->detachDistancesWindows();
}


void MainWindow::on_actionClose_triggered()
{
  this->close();
}

void MainWindow::on_actionDistance_triggered()
{
  if (workingImage) {
    if (workingImage->distances == 0) {
      disableOtherTabs();
      setOperationsEnabled(false);

      workingImage->clearOverlay();

      connect(workingImage,   SIGNAL(status(QString)),
              ui->statusBar,  SLOT(showMessage(QString)));

      connect(workingImage, SIGNAL(exitSelectionMode()),
              this,         SLOT(finishMeasuring()));

      workingImage->setSelectionMode(Image::Distance);
    }
  }
}

void MainWindow::on_actionEqualize_triggered()
{
  if (workingImage) {
    if (workingImage->current.channels() == 1) {
      workingImage->backup();

      cv::equalizeHist(workingImage->previous, workingImage->current);

      workingImage->update();
    }
  }
}

void MainWindow::on_actionGradient_triggered()
{
  if (workingImage) {
    if (workingImage->current.channels() == 1) {
      disableOtherTabs();
      setOperationsEnabled(false);

      gradientWindow = new GradientWindow(workingImage, this);

      connect(gradientWindow, SIGNAL(destroyed()),
              this,           SLOT(enableAllTabs()));

      connect(gradientWindow, SIGNAL(destroyed()),
              this,           SLOT(enableAllOperations()));
    }
  }
}

void MainWindow::on_actionGrayscale_triggered()
{
  if (workingImage) {
    if (workingImage->current.channels() == 3) {
      int index = ui->imagesTabWidget->currentIndex();
      QString name = ui->imagesTabWidget->tabText(index);
      cv::Mat grayscale;

      cv::cvtColor(workingImage->current, grayscale, CV_BGR2GRAY);

      Image* newImage = new Image(grayscale, this);

      images.push_back(newImage);

      ui->imagesTabWidget->insertTab(++index,
                                     newImage,
                                     name + " (gray)");
    }
  }
}

void MainWindow::on_actionHistogram_triggered()
{
  if (workingImage) {
    if (workingImage->current.channels() == 1) {
      disableOtherTabs();
      setOperationsEnabled(false);

      histogramWindow = new HistogramWindow(workingImage->current, this);

      connect(histogramWindow,  SIGNAL(destroyed()),
              this,             SLOT(enableAllTabs()));

      connect(histogramWindow,  SIGNAL(destroyed()),
              this,             SLOT(enableAllOperations()));
    }
  }
}

void MainWindow::on_actionHSV_triggered()
{
  if (workingImage) {
    if (workingImage->current.channels() == 3) {
      int index = ui->imagesTabWidget->currentIndex();
      QString name = ui->imagesTabWidget->tabText(index);
      std::vector<cv::Mat> hsv;

      workingImage->HSV(hsv);

      for (int i = 0; i < 3; i++) {
        QString newName;
        Image* newImage = new Image(hsv.at(i), this);
        images.push_back(newImage);

        switch (i) {
          case 0:
            newName = name + " (H)";
            break;
          case 1:
            newName = name + " (S)";
            break;
          case 2:
            newName = name + " (V)";
            break;
        }

        ui->imagesTabWidget->insertTab(++index, newImage, newName);
      }
    }
  }
}

void MainWindow::on_actionInvert_triggered()
{
  if (workingImage) {
    std::vector<cv::Mat> channels;

    workingImage->backup();

    cv::split(workingImage->current, channels);

    for (size_t i = 0; i < channels.size(); i++)
      channels.at(i) = 255 - channels.at(i);

    cv::merge(channels, workingImage->current);

    workingImage->update();
  }
}

void MainWindow::on_actionMorphology_triggered()
{
  if (workingImage) {
    if (workingImage->current.channels() == 1) {
      disableOtherTabs();
      setOperationsEnabled(false);

      morphologyWindow = new MorphologyWindow(workingImage, this);

      connect(morphologyWindow, SIGNAL(destroyed()),
              this,             SLOT(enableAllTabs()));

      connect(morphologyWindow, SIGNAL(destroyed()),
              this,             SLOT(enableAllOperations()));
    }
  }
}

void MainWindow::on_actionOpen_triggered()
{
  setOperationsEnabled(false);

  QString filename =
      QFileDialog::getOpenFileName(this, "Open", QString(),
                                   "Image Files (*.png *.jpg *.bmp *.tif)");

  setOperationsEnabled(true);

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

void MainWindow::on_actionParticles_triggered()
{
  if (workingImage) {
    if (workingImage->current.channels() == 1 && workingImage->areas == 0)  {
      workingImage->clearOverlay();

      cv::Mat labels(workingImage->current.rows,
                     workingImage->current.cols,
                     CV_32S);
      std::vector<cv::ConnectedComponentStats> stats;

      cv::connectedComponentsWithStats(labels,
                                       workingImage->current,
                                       stats);

      workingImage->overlayAreas(stats);
    }
  }
}

void MainWindow::on_actionRevert_triggered()
{
  if (workingImage)
    workingImage->revert();
}

void MainWindow::on_actionRGB_triggered()
{
  if (workingImage) {
    if (workingImage->current.channels() == 3) {
      int index = ui->imagesTabWidget->currentIndex();
      QString name = ui->imagesTabWidget->tabText(index);
      std::vector<cv::Mat> rgb;

      workingImage->RGB(rgb);

      for (int i = 0; i < 3; i++) {
        QString newName;
        Image* newImage = new Image(rgb.at(i), this);
        images.push_back(newImage);

        switch (i) {
          case 0:
            newName = name + " (R)";
            break;
          case 1:
            newName = name + " (G)";
            break;
          case 2:
            newName = name + " (B)";
            break;
        }

        ui->imagesTabWidget->insertTab(++index, newImage, newName);
      }
    }
  }
}

void MainWindow::on_actionSave_triggered()
{
  if (workingImage) {
    setOperationsEnabled(false);

    QString filename = QFileDialog::getSaveFileName(this, QLatin1String("Save"));

    setOperationsEnabled(true);

    if (!filename.isEmpty()) {
      std::vector<int> qualityType;

      qualityType.push_back(CV_IMWRITE_JPEG_QUALITY);
      qualityType.push_back(90);

      filename += ".jpg";

      cv::imwrite(filename.toStdString(), workingImage->current, qualityType);
    }
  }
}

void MainWindow::on_actionSet_Scale_triggered()
{
  if (workingImage) {
    disableOtherTabs();
    setOperationsEnabled(false);

    setScaleWindow = new SetScaleWindow(workingImage, this);

    connect(workingImage,   SIGNAL(status(QString)),
            ui->statusBar,  SLOT(showMessage(QString)));

    connect(setScaleWindow, SIGNAL(destroyed()),
            this,           SLOT(releaseStatusBar()));

    connect(setScaleWindow, SIGNAL(destroyed()),
            this,           SLOT(enableAllTabs()));

    connect(setScaleWindow, SIGNAL(destroyed()),
            this,           SLOT(enableAllOperations()));
  }
}

void MainWindow::on_actionStretch_triggered()
{
  if (workingImage) {
    workingImage->backup();

    cv::normalize(workingImage->previous,
                  workingImage->current,
                  0,
                  255,
                  cv::NORM_MINMAX);

    workingImage->update();
  }
}

void MainWindow::on_actionThreshold_triggered()
{
  if (workingImage) {
    if (workingImage->current.channels() == 1) {
      disableOtherTabs();
      setOperationsEnabled(false);

      thresholdWindow = new ThresholdWindow(workingImage, this);

      connect(thresholdWindow,  SIGNAL(destroyed()),
              this,             SLOT(enableAllTabs()));

      connect(thresholdWindow,  SIGNAL(destroyed()),
              this,             SLOT(enableAllOperations()));
    }
  }
}

void MainWindow::on_actionUndo_triggered()
{
  if (workingImage)
    workingImage->undo();
}

void MainWindow::on_imagesTabWidget_currentChanged(QWidget *image)
{
  if (workingImage)
    workingImage->clearOverlay();

  workingImage = (Image*)image;

  if (workingImage != 0)
    workingImage->update();
}

void MainWindow::on_imagesTabWidget_tabCloseRequested(int index)
{
  index = images.indexOf((Image*)(ui->imagesTabWidget->widget(index)));

  delete images.at(index);
  images.remove(index);
}

void MainWindow::crop(QRect rect)
{
  int index = ui->imagesTabWidget->currentIndex();
  QString name = ui->imagesTabWidget->tabText(index);

  if (rect.width() != 0 && rect.height() != 0) {
    cv::Rect roi(rect.x(), rect.y(), rect.width(), rect.height());
    cv::Mat mat = workingImage->current(roi);

    Image* newImage = new Image(mat, this);
    images.push_back(newImage);

    ui->imagesTabWidget->insertTab(++index, newImage, name + " (crop)");
  }

  enableAllTabs();
  enableAllOperations();

  disconnect(workingImage,  SIGNAL(rectangleSelected(QRect)),
             this,          SLOT(crop(QRect)));

  disconnect(workingImage,  SIGNAL(status(QString)),
             ui->statusBar, SLOT(showMessage(QString)));

  ui->statusBar->clearMessage();
}

void MainWindow::disableOtherTabs()
{
  if (workingImage) {
    int index = ui->imagesTabWidget->currentIndex();

    ui->imagesTabWidget->setTabsClosable(false);

    for (int i = 0; i < ui->imagesTabWidget->count(); i++)
      if (i != index)
        ui->imagesTabWidget->setTabEnabled(i, false);
  }
}

void MainWindow::enableAllOperations()
{
  setOperationsEnabled(true);
}

void MainWindow::enableAllTabs()
{
  ui->imagesTabWidget->setTabsClosable(true);

  for (int i = 0; i < ui->imagesTabWidget->count(); i++)
    ui->imagesTabWidget->setTabEnabled(i, true);
}

void MainWindow::finishMeasuring()
{
  enableAllTabs();
  enableAllOperations();

  ui->statusBar->clearMessage();

  disconnect(workingImage,  SIGNAL(status(QString)),
             ui->statusBar, SLOT(showMessage(QString)));

  disconnect(workingImage,  SIGNAL(exitSelectionMode()),
             this,          SLOT(finishMeasuring()));
}

void MainWindow::releaseStatusBar()
{
  disconnect(workingImage,  SIGNAL(status(QString)),
             ui->statusBar, SLOT(showMessage(QString)));
}

void MainWindow::setOperationsEnabled(bool enable)
{
  ui->actionBlur->setEnabled(enable);
  ui->actionCanny->setEnabled(enable);
  ui->actionCrop->setEnabled(enable);
  ui->actionDistance->setEnabled(enable);
  ui->actionEqualize->setEnabled(enable);
  ui->actionGradient->setEnabled(enable);
  ui->actionGrayscale->setEnabled(enable);
  ui->actionHistogram->setEnabled(enable);
  ui->actionHSV->setEnabled(enable);
  ui->actionInvert->setEnabled(enable);
  ui->actionMorphology->setEnabled(enable);
  ui->actionOpen->setEnabled(enable);
  ui->actionParticles->setEnabled(enable);
  ui->actionRevert->setEnabled(enable);
  ui->actionRGB->setEnabled(enable);
  ui->actionSave->setEnabled(enable);
  ui->actionSet_Scale->setEnabled(enable);
  ui->actionStretch->setEnabled(enable);
  ui->actionThreshold->setEnabled(enable);
  ui->actionUndo->setEnabled(enable);
  ui->actionClear->setEnabled(enable);
}
