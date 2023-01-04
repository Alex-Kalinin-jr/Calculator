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
#include "mainwindow.h"

#include <QDoubleSpinBox>
#include <QRegularExpression>
#include <QString>
#include <cfloat>
#include <iomanip>
#include <string>

#include "qcustomplot.h"
#include "s21_controller.h"
#include "s21_error_handler.h"
#include "ui_mainwindow.h"

s21::Model calc_model;
s21::CreditModel calc_credit_model;
s21::DepoModel calc_depo_model;
s21::Controller calc_controller(&calc_model, &calc_credit_model,
                                &calc_depo_model);
s21::ErrorHandler err_handler;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  x_validator_.setRange(-DBL_MAX, DBL_MAX);
  x_validator_.setDecimals(8);
  money_validator_.setRange(0, INT_MAX);
  ui->setupUi(this);

  rep_window_.setWindowTitle("List of replenishmets");
  wdraw_window_.setWindowTitle("List of withdrawals");
  ui->credit_table->setHorizontalHeaderLabels(QStringList() << "Month"
                                                            << "Sum"
                                                            << "Interest"
                                                            << "Main debt");
  connect(ui->b0, SIGNAL(clicked()), this, SLOT(on_add_clicked()));
  connect(ui->b1, SIGNAL(clicked()), this, SLOT(on_add_clicked()));
  connect(ui->b2, SIGNAL(clicked()), this, SLOT(on_add_clicked()));
  connect(ui->b3, SIGNAL(clicked()), this, SLOT(on_add_clicked()));
  connect(ui->b4, SIGNAL(clicked()), this, SLOT(on_add_clicked()));
  connect(ui->b5, SIGNAL(clicked()), this, SLOT(on_add_clicked()));
  connect(ui->b6, SIGNAL(clicked()), this, SLOT(on_add_clicked()));
  connect(ui->b7, SIGNAL(clicked()), this, SLOT(on_add_clicked()));
  connect(ui->b8, SIGNAL(clicked()), this, SLOT(on_add_clicked()));
  connect(ui->b9, SIGNAL(clicked()), this, SLOT(on_add_clicked()));
  connect(ui->dot, SIGNAL(clicked()), this, SLOT(on_add_clicked()));
  connect(ui->sin, SIGNAL(clicked()), this, SLOT(on_add_clicked()));
  connect(ui->cos, SIGNAL(clicked()), this, SLOT(on_add_clicked()));
  connect(ui->tan, SIGNAL(clicked()), this, SLOT(on_add_clicked()));
  connect(ui->asin, SIGNAL(clicked()), this, SLOT(on_add_clicked()));
  connect(ui->acos, SIGNAL(clicked()), this, SLOT(on_add_clicked()));
  connect(ui->atan, SIGNAL(clicked()), this, SLOT(on_add_clicked()));
  connect(ui->log, SIGNAL(clicked()), this, SLOT(on_add_clicked()));
  connect(ui->ln, SIGNAL(clicked()), this, SLOT(on_add_clicked()));
  connect(ui->sub, SIGNAL(clicked()), this, SLOT(on_add_clicked()));
  connect(ui->mul, SIGNAL(clicked()), this, SLOT(on_add_clicked()));
  connect(ui->div, SIGNAL(clicked()), this, SLOT(on_add_clicked()));
  connect(ui->mod, SIGNAL(clicked()), this, SLOT(on_add_clicked()));
  connect(ui->l_br, SIGNAL(clicked()), this, SLOT(on_add_clicked()));
  connect(ui->r_br, SIGNAL(clicked()), this, SLOT(on_add_clicked()));
  connect(ui->var, SIGNAL(clicked()), this, SLOT(on_add_clicked()));
  connect(ui->pow, SIGNAL(clicked()), this, SLOT(on_add_clicked()));
  connect(ui->sqrt, SIGNAL(clicked()), this, SLOT(on_add_clicked()));
  connect(ui->e, SIGNAL(clicked()), this, SLOT(on_add_clicked()));

  connect(ui->x_left, SIGNAL(editingFinished()), this,
          SLOT(on_x_y_editingFinished()));
  connect(ui->x_right, SIGNAL(editingFinished()), this,
          SLOT(on_x_y_editingFinished()));

  connect(ui->credit_sum, SIGNAL(editingFinished()), this,
          SLOT(on_credit_sum_editingFinished()));
  connect(ui->credit_percent, SIGNAL(valueChanged(double)), this,
          SLOT(on_credit_percent_valueChanged(double)));
  connect(ui->credit_period, SIGNAL(valueChanged(int)), this,
          SLOT(on_credit_period_valueChanged(int)));
  connect(ui->credit_type, SIGNAL(currentIndexChanged(int)), this,
          SLOT(on_credit_type_currentIndexChanged(int)));
  connect(ui->credit_clear, SIGNAL(clicked()), this, SLOT(reset_credit()));

  connect(ui->d_amount, SIGNAL(editingFinished()), this,
          SLOT(on_d_amount_editingFinished()));
  connect(ui->d_term, SIGNAL(valueChanged(int)), this,
          SLOT(on_d_term_valueChanged(int)));
  connect(ui->d_interest, SIGNAL(valueChanged(double)), this,
          SLOT(on_d_interest_valueChanged(double)));
  connect(ui->d_tax, SIGNAL(valueChanged(double)), this,
          SLOT(on_d_tax_valueChanged(double)));
  connect(ui->d_cycle, SIGNAL(currentIndexChanged(int)), this,
          SLOT(on_d_cycle_currentIndexChanged(int)));
  connect(ui->d_capitalzation, SIGNAL(currentIndexChanged(int)), this,
          SLOT(on_d_capitalzation_currentIndexChanged(int)));
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_add_clicked() {
  QString buff = "";
  if (!is_start_) buff = ui->screen_1->text();
  auto button = (QPushButton *)sender();
  if (button == ui->var) var_pressed_ = true;
  ui->screen_1->setText(buff.append(button->text()));
  for_transmit_ = ui->screen_1->text();
  is_start_ = false;
}
// resets all
void MainWindow::on_AC_clicked() {
  is_start_ = 1;
  var_ = 1;
  for_transmit_ = "";
  ui->screen_1->setText("0");
  var_pressed_ = false;
  ui->out_msg->setText("");
}

void MainWindow::on_result_clicked() {
  for_transmit_ = ui->screen_1->text();
  QString replaced = "√";
  QString replacing = "sqrt";
  for_transmit_.replace(replaced, replacing);
  auto return_code = calc_controller.DelegateExpr(for_transmit_.toStdString());
  if (return_code == -1) {
    QString buff = ui->var_val->text();
    int not_used = 0;
    if (x_validator_.validate(buff, not_used) == QValidator::Acceptable) {
      var_ = buff.toDouble();
    } else if (var_pressed_ == true) {
      ui->out_msg->setText(QString::fromStdString(err_handler.TransmitMsg(-6)));
      return;
    }
    auto result = calc_controller.ComputeAndTransmit(var_);
    ui->screen_1->setText(QString::number((double)result, 'g', 10));
  } else if (return_code >= 0) {
    ui->out_msg->setText(
        QString("Mistakes! (The first is at pos %1)").arg(return_code + 1));
  } else {
    ui->out_msg->setText(
        QString::fromStdString(err_handler.TransmitMsg(return_code)));
  }
}

void MainWindow::on_x_y_editingFinished() {
  auto button = (QLineEdit *)sender();
  int not_used = 0;
  QString buff = button->text();
  if (x_validator_.validate(buff, not_used) == QValidator::Acceptable) {
    if (button == ui->x_left) {
      x_left_ = buff.toDouble();
    } else if (button == ui->x_right) {
      x_right_ = buff.toDouble();
    } else if (button == ui->y_bot) {
      y_bot_ = buff.toDouble();
    } else if (button == ui->y_top) {
      y_top_ = buff.toDouble();
    }
  } else {
    ui->out_msg->setText(QString::fromStdString(err_handler.TransmitMsg(-5)));
  }
}

void MainWindow::on_erase_clicked() {
  QRegularExpression re1("sin$|cos$|tan$|log$|mod$");
  QRegularExpression re2("asin$|acos$|atan$");
  QString buff = ui->screen_1->text();
  QRegularExpressionMatch match_1 = re1.match(buff);
  QRegularExpressionMatch match_2 = re2.match(buff);
  if (match_2.hasMatch()) {
    buff.chop(4);
  } else if (match_1.hasMatch()) {
    buff.chop(3);
  } else if (!is_start_) {
    buff.chop(1);
  }
  if (buff == "") {
    buff = "0";
    is_start_ = true;
  }
  ui->screen_1->setText(buff);
}

void MainWindow::on_draw_clicked() {
  if (x_left_ >= x_right_ || y_top_ <= y_bot_) {
    ui->out_msg->setText(QString::fromStdString(err_handler.TransmitMsg(-4)));
  } else {
    QVector<double> y_vals;
    QVector<double> x_vals;
    for_transmit_ = ui->screen_1->text();
    QString replaced = "√";
    QString replacing = "sqrt";
    for_transmit_.replace(replaced, replacing);
    auto return_code =
        calc_controller.DelegateExpr(for_transmit_.toStdString());
    if (return_code == -1) {
      double step = (x_right_ - x_left_) / 100000;
      double val = static_cast<double>(x_left_);
      double top_brdr = 1;
      double bottom_brdr = 0;
      long double result = 0;
      while (val < x_right_) {
        result = calc_controller.ComputeAndTransmit(val);
        if (result <= y_top_ && result >= y_bot_) {
          if (result > top_brdr) top_brdr = 1.2 * result;
          if (result < bottom_brdr) bottom_brdr = 1.2 * result;
          x_vals.append(val);
          y_vals.append(result);
        }
        val += step;
      }
      ui->plot->addGraph();
      ui->plot->graph()->setLineStyle(QCPGraph::lsNone);
      ui->plot->graph()->setScatterStyle(
          QCPScatterStyle(QCPScatterStyle::ssCircle, 1));
      ui->plot->setInteraction(QCP::iRangeDrag, true);
      ui->plot->setInteraction(QCP::iRangeZoom, true);
      ui->plot->xAxis->setRange(x_left_, x_right_);
      ui->plot->yAxis->setRange(bottom_brdr, top_brdr);
      ui->plot->graph(0)->setData(x_vals, y_vals);
      ui->plot->xAxis->setLabel("x");
      ui->plot->yAxis->setLabel("y");
      ui->plot->replot();
      ui->out_msg->setText(QString("Drawn!"));
    } else if (return_code >= 0) {
      ui->out_msg->setText(
          QString("Mistakes! (The first is at pos %1)").arg(return_code + 1));
    } else {
      ui->out_msg->setText(
          QString::fromStdString(err_handler.TransmitMsg(return_code)));
    }
  }
}

void MainWindow::on_credit_sum_editingFinished() {
  int not_used = 0;
  QString buff = ui->credit_sum->text();
  if (money_validator_.validate(buff, not_used) == QValidator::Acceptable) {
    c_sum_ = buff.toInt();
  }
}

void MainWindow::on_credit_percent_valueChanged(double arg1) {
  c_interest_ = arg1;
}

void MainWindow::on_credit_period_valueChanged(int arg1) { c_period_ = arg1; }

void MainWindow::on_credit_evaluate_clicked() {
  ui->credit_table->clear();
  ui->credit_table->setRowCount(0);
  if (c_sum_ != 0 && c_interest_ != 0 && c_period_ != 0) {
    calc_controller.PassCreditInfo(c_sum_, c_period_, c_interest_);
    std::queue<double> percents;
    std::queue<double> main_debt;
    calc_controller.get_credit_data(c_type_, &percents, &main_debt);
    ui->credit_table->setRowCount(percents.size());
    ui->credit_table->setHorizontalHeaderLabels(QStringList() << "Month"
                                                              << "Sum"
                                                              << "Interest"
                                                              << "Main debt");
    QTableWidgetItem *cell;
    double overpay = 0;
    for (int i = 0; i < ui->credit_table->rowCount(); ++i) {
      for (int j = 0; j < ui->credit_table->columnCount(); ++j) {
        cell = new QTableWidgetItem();
        switch (j) {
          case 0:
            cell->setText(QString("Month %1").arg(QString::number(i + 1)));
            break;
          case 2:
            cell->setText(QString::number(percents.front(), 'f', 2));
            overpay += percents.front();
            percents.pop();
            break;
          case 3:
            cell->setText(QString::number(main_debt.front(), 'f', 2));
            main_debt.pop();
            break;
          case 1:
            cell->setText(
                QString::number(percents.front() + main_debt.front(), 'f', 2));
            break;
        }
        ui->credit_table->setItem(i, j, cell);
      }
    }
    ui->out_msg_2->setText(QString("You would overpay near %1")
                               .arg(QString::number(overpay, 'f', 2)));
  } else {
    ui->out_msg_2->setText("Check corectness of the data inserted");
  }
}

void MainWindow::on_credit_type_currentIndexChanged(int index) {
  if (index == 0) c_type_ = false;
  if (index == 1) c_type_ = true;
}

void MainWindow::reset_credit() {
  c_sum_ = 0;
  c_interest_ = 0;
  c_period_ = 0;
  c_type_ = false;
  ui->credit_sum->clear();
  ui->credit_percent->setValue(0);
  ui->credit_period->setValue(0);
  ui->credit_type->setCurrentIndex(0);
  ui->credit_table->clear();
  ui->credit_table->setRowCount(0);
  ui->credit_table->setHorizontalHeaderLabels(QStringList() << "Month"
                                                            << "Sum"
                                                            << "Interest"
                                                            << "Main debt");
  ui->out_msg_2->setText("");
}

void MainWindow::on_d_amount_editingFinished() {
  int not_used = 0;
  QString buff = ui->d_amount->text();
  if (money_validator_.validate(buff, not_used) == QValidator::Acceptable) {
    depo_.d_sum = buff.toInt();
  }
}

void MainWindow::on_d_term_valueChanged(int arg1) { depo_.d_term = arg1; }

void MainWindow::on_d_interest_valueChanged(double arg1) {
  depo_.d_interest = arg1;
}

void MainWindow::on_d_tax_valueChanged(double arg1) { depo_.d_tax = arg1; }

void MainWindow::on_d_cycle_currentIndexChanged(int index) {
  depo_.d_cycle = index;
}

void MainWindow::on_d_capitalzation_currentIndexChanged(int index) {
  depo_.d_capitalization = index;
}

void MainWindow::on_d_rep_list_clicked() { rep_window_.show(); }

void MainWindow::on_d_withdr_clicked() { wdraw_window_.show(); }

void MainWindow::on_d_date_userDateChanged(const QDate &date) {
  std::tm start{};
  start.tm_mday = date.day();
  start.tm_mon = date.month();
  start.tm_year = date.year();
  depo_.d_start = std::mktime(&start);
}

void MainWindow::on_d_eval_clicked() {
  ui->d_table->clear();
  ui->d_table->setRowCount(0);
  ui->d_table->setHorizontalHeaderLabels(QStringList() << "date"
                                                       << "benefit"
                                                       << "Your depo");
  if (depo_.d_start != 0 && depo_.d_sum != 0 && depo_.d_term != 0 &&
      depo_.d_interest != 0) {
    ui->out_msg_2->setText("");
    calc_controller.PassDepoInfo(depo_, rep_window_.repls_,
                                 wdraw_window_.repls_);
    std::queue<std::pair<std::tm, double>> depo_queue =
        calc_controller.get_depo_data();
    std::queue<double> depo_vals = calc_controller.get_depo_sum();
    ui->d_table->setRowCount(depo_queue.size());
    QTableWidgetItem *cell;
    int i = 0;
    long double benefit = 0;
    while (!depo_queue.empty()) {
      cell = new QTableWidgetItem();
      std::tm buff = depo_queue.front().first;
      QDate date_buff(buff.tm_year, buff.tm_mon, buff.tm_mday);
      cell->setText(date_buff.toString("d MMM yyyy"));
      ui->d_table->setItem(i, 0, cell);
      cell = new QTableWidgetItem();
      cell->setText(QString::number(depo_queue.front().second, 'f', 2));
      ui->d_table->setItem(i, 1, cell);
      benefit += depo_queue.front().second;
      depo_queue.pop();
      cell = new QTableWidgetItem();
      cell->setText(QString::number(depo_vals.front(), 'f', 2));
      ui->d_table->setItem(i, 2, cell);
      depo_vals.pop();
      ++i;
    }
    QString out = QString::number(benefit, 'f', 2);
    out.prepend("Your benefit is ");
    ui->out_msg_3->setText(out);
  } else {
    ui->out_msg_3->setText("Please check the corectness of the data written");
  }
}
