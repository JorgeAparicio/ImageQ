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

#include "imagelabel.h"

#include <QMouseEvent>
#include <QDebug>
ImageLabel::ImageLabel(QWidget *parent) :
  QLabel(parent)
{
  this->setMouseTracking(true);
}

void ImageLabel::mouseDoubleClickEvent(QMouseEvent *ev)
{
  emit mouseDoubleClick(ev->pos());
}

void ImageLabel::mouseMoveEvent(QMouseEvent *ev)
{
  emit mouseHover(ev->pos());
}

void ImageLabel::mousePressEvent(QMouseEvent *ev)
{
  emit mousePress(ev->pos());
}

void ImageLabel::mouseReleaseEvent(QMouseEvent *ev)
{
  emit mouseRelease(ev->pos());
}

void ImageLabel::resizeEvent(QResizeEvent *)
{
  emit resized();
}
