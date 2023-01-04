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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDoubleValidator>
#include <QMainWindow>

#include "replenishments.h"
#include "s21_controller.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  MainWindow(const MainWindow& other) = delete;
  MainWindow& operator=(const MainWindow& other) = delete;
  MainWindow(MainWindow&& other) = delete;
  MainWindow& operator=(MainWindow&& other) = delete;

 private slots:
  // just writes text from button to the end of the string to be passed to
  // model. used by the majority of presented buttons.
  void on_add_clicked();
  // clears all data and sets settings to defaults.
  void on_AC_clicked();
  // passes the string to model via controller. model computes given expression
  // and passes result to model. result is shown to user.
  void on_result_clicked();
  // reads and validates x and y border values when cursor activated certain
  // fields and changed values in them.
  void on_x_y_editingFinished();
  // erases the last operator/operand from input.
  void on_erase_clicked();
  // draws the graph according to the input data.
  void on_draw_clicked();

  // reads and validates input from user. only positive integer or 0.
  void on_credit_sum_editingFinished();
  // reads input.
  void on_credit_percent_valueChanged(double arg1);
  // reads input(months).
  void on_credit_period_valueChanged(int arg1);
  // clears themseve, creates new table, pass input info to credit_model via
  // controller. gets the answer. uses it for filling table. output the result.
  void on_credit_evaluate_clicked();
  // changes data passed.
  void on_credit_type_currentIndexChanged(int index);
  // sets values to defaults and recreates table with 0 rows.
  void reset_credit();

  // reads and validates input from user. only positive integer or 0.
  void on_d_amount_editingFinished();
  // reads input(months).
  void on_d_term_valueChanged(int arg1);
  // reads input(%).
  void on_d_interest_valueChanged(double arg1);
  // reads input(%).
  void on_d_tax_valueChanged(double arg1);
  // reads periodicity(day, week, month, quarter, year).
  void on_d_cycle_currentIndexChanged(int index);
  // yes/no index
  void on_d_capitalzation_currentIndexChanged(int index);
  // invokes a window rep_window_. user has possibility to fill in
  // a table of replenlisments in format date/sum.
  void on_d_rep_list_clicked();
  // the same for withdrawals.
  void on_d_withdr_clicked();
  // reads depo start and converts it in std::time_t.
  void on_d_date_userDateChanged(const QDate &date);
  // passes data through controller. recieves results. clears table and set
  // default fills in the table. outputs it.
  void on_d_eval_clicked();

 private:
  Ui::MainWindow *ui;
  Replenishments rep_window_;
  Replenishments wdraw_window_;
  // -double_max, double_max.
  QDoubleValidator x_validator_;
  // 0, integer_max.
  QIntValidator money_validator_;
  // string to be transmitted to model via controller
  QString for_transmit_;
  // indicator for visual correctness.
  bool is_start_ = true;
  // variable value for computation
  long double var_ = 1;
  // indicator for internal mechanisms
  bool var_pressed_ = false;
  double x_left_ = -10;
  double x_right_ = 10;
  double y_bot_ = -10;
  double y_top_ = 10;
  // credit data
  size_t c_sum_ = 0;
  double c_interest_ = 0;
  size_t c_period_ = 0;
  bool c_type_ = false;
  // depo data wrapped by struct.
  s21::DepoInfo depo_;
};
#endif  // MAINWINDOW_H
