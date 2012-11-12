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


#include "setscalewindow.h"
#include "ui_setscalewindow.h"

#include "image.h"

SetScaleWindow::SetScaleWindow(Image* image,
                               QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::SetScaleWindow),
  image(image)
{
  ui->setupUi(this);

  ui->unitLineEdit->setValidator(new QRegExpValidator(QRegExp("[a-zA-Z]+")));

  scale = image->scale;

  showConversion();

  this->setAttribute(Qt::WA_DeleteOnClose);

  this->show();
}

SetScaleWindow::~SetScaleWindow()
{
  delete ui;
}

void SetScaleWindow::on_lengthSpinBox_valueChanged(int)
{
  updateConversion();
}

void SetScaleWindow::on_unitLineEdit_textEdited(const QString &)
{
  if (ui->unitLineEdit->hasAcceptableInput())
    updateConversion();
}

void SetScaleWindow::on_selectPushButton_clicked()
{
  image->setSelectionMode(Image::Line);

  connect(image,  SIGNAL(lineSelected(QLine const&)),
          this,   SLOT(on_lineSelected(QLine const&)));

  this->hide();
}

void SetScaleWindow::on_okPushButton_clicked()
{
  image->scale = scale;

  this->close();
}

void SetScaleWindow::on_lineSelected(QLine const& line)
{
  int distance = sqrt(pow(line.dx(), 2) + pow(line.dy(), 2)) + 1;

  ui->pixelsLabel->setText(QString::number(distance));

  this->show();
  image->update();

  disconnect(image, SIGNAL(lineSelected(QLine const&)),
             this,  SLOT(on_lineSelected(QLine const&)));

  updateConversion();
}

void SetScaleWindow::showConversion() const
{
  ui->scaleLabel->setText(QString::number(scale, 'g', 4) +
                          ' ' + image->unit);
}

void SetScaleWindow::updateConversion()
{
  scale = ui->lengthSpinBox->value() / ui->pixelsLabel->text().toFloat();
  image->unit = ui->unitLineEdit->text();

  showConversion();
}
