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

#include "textlistwindow.h"
#include "ui_textlistwindow.h"

#include <QFileDialog>
#include <QTextStream>

TextListWindow::TextListWindow(QString const& title,
                               QString const& header,
                               QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::TextListWindow),
  text(QStringList()),
  count(0)
{
  ui->setupUi(this);

  this->setWindowTitle(title);

  ui->headerLabel->setText(header);

  this->setAttribute(Qt::WA_DeleteOnClose);
  this->show();
}

TextListWindow::~TextListWindow()
{
  delete ui;
}

void TextListWindow::append(QString const& s)
{
  count++;

  text << s;

  ui->textLabel->setText(text.join(QString('\n')));
}

int TextListWindow::size() const
{
  return count;
}

void TextListWindow::on_pushButton_clicked()
{
  QString filename = QFileDialog::getSaveFileName(this,
                                                  QLatin1String("Save"),
                                                  QString(),
                                                  "Text Files (*.txt)");

  if (!filename.endsWith(".txt"))
    filename += ".txt";

  QFile f(filename);
  f.open(QIODevice::WriteOnly);
  QTextStream stream(&f);

  stream << ui->headerLabel->text() + "\n" + text.join("\n");

  f.close();

  this->close();
}

void TextListWindow::on_pushButton_2_clicked()
{
  this->close();
}
