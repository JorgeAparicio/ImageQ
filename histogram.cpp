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

#include "histogram.h"

Histogram::Histogram(cv::Mat const& image, int const numberOfBins)
{
  cv::Mat histogram;

  std::vector<cv::Mat> images;
  std::vector<int> channels;
  std::vector<int> histSize;
  std::vector<float> ranges;

  images.push_back(image);
  channels.push_back(0);
  histSize.push_back(numberOfBins);
  ranges.push_back(0);
  ranges.push_back(256);

  cv::calcHist(images, channels, cv::Mat(), histogram, histSize, ranges);

  QVector<QwtIntervalSample> samples(numberOfBins);

  for (int i = 0; i < numberOfBins; i++)
    samples[i] = QwtIntervalSample(histogram.at<float>(i),
                                   QwtInterval(i, i + 1));

  QColor color(Qt::blue);

  color.setAlpha(0);
  setPen(QPen(color));
  color.setAlpha(96);
  setBrush(QBrush(color));

  setData(new QwtIntervalSeriesData(samples));


}
