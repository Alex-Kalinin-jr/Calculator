// ***********************************************************************************
// ** Smart Calculator - computation tool, easy to scale with new functionality
// **
// ** Copyright 2023 Alexey Kalinin **
// ** **
// ** This program is free software: you can redistribute it and/or modify **
// ** it under the terms of the GNU General Public License as published by **
// ** the Free Software Foundation, either version 3 of the License, or **
// ** (at your option) any later version. **
// ** **
// ** This program is distributed in the hope that it will be useful, **
// ** but WITHOUT ANY WARRANTY; without even the implied warranty of **
// ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the **
// ** GNU General Public License for more details. **
// ** **
// ** You should have received a copy of the GNU General Public License **
// ** along with this program.  If not, see <http://www.gnu.org/licenses/>. **
// ***********************************************************************************

#ifndef REPLENISHMENTS_H
#define REPLENISHMENTS_H

#include <QDoubleValidator>
#include <QWidget>
#include <ctime>
#include <map>
// This is just a table of data in the certain format(date/sum).
// Used for withdrawals too.
// Multiple actions are possible at the same date,
// i.e. 10 withdrawals at 1 Jan 2024.
QT_BEGIN_NAMESPACE
namespace Ui {
class Replenishments;
}
QT_END_NAMESPACE

class Replenishments : public QWidget {
  Q_OBJECT
  friend class MainWindow;

 public:
  explicit Replenishments(QWidget *parent = nullptr);
  ~Replenishments();
  Replenishments(const Replenishments& other) = default;
  Replenishments& operator=(const Replenishments& other) = default;
  Replenishments(Replenishments&& other) = delete;
  Replenishments& operator=(Replenishments&& other) = delete;

 private slots:
  // adds a row to QTableWidget in the form of date/sum. The data
  // is written to "repls_"
  void on_r_add_clicked();

  // clears repls_ and gui.
  void on_r_reset_clicked();

 private:
  Ui::Replenishments *ui;
  size_t rows_ = 0;
  // only integers are allowed.
  QIntValidator money_validator_;
  std::multimap<std::time_t, size_t> repls_;
};

#endif  // REPLENISHMENTS_H
