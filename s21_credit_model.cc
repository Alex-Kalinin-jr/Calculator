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

#include "s21_credit_model.h"

#include <cmath>
#include <iostream>
#include <queue>
#include <string>

namespace s21 {

CreditModel::CreditModel() {}
CreditModel::~CreditModel() {}

void CreditModel::get_credit_data(size_t sum, size_t period, double percent) {
  sum_ = sum;
  period_ = period;
  percent_ = percent;
}

void CreditModel::EvaluateDiff() {
  Clear(&main_debt_);
  Clear(&percents_);
  double month_perc = percent_ / 12.0 / 100.0;
  double main_dbt = sum_ / static_cast<double>(period_);
  period_ += 1;  //  for correct loop condition below
  while (--period_ > 0) {
    double a = static_cast<double>(sum_) * month_perc;
    percents_.push(a);
    main_debt_.push(main_dbt);
    sum_ -= main_dbt;
  }
}

void CreditModel::EvaluateAnnuity() {
  Clear(&main_debt_);
  Clear(&percents_);
  double month_perc = percent_ / 12.0 / 100.0;
  double k = month_perc * pow(1 + month_perc, period_) /
             (pow(1 + month_perc, period_) - 1);
  double monthly = static_cast<double>(sum_) * k;
  period_ += 1;  //  for correct loop condition below
  while (--period_ > 0) {
    double a = static_cast<double>(sum_) * month_perc;
    double b = monthly - a;
    percents_.push(a);
    main_debt_.push(b);
    sum_ -= b;
  }
}

std::queue<double> CreditModel::get_percents() const { return percents_; }

std::queue<double> CreditModel::get_main_debt() const { return main_debt_; }

void CreditModel::Clear(std::queue<double> *a) {
  while (!(a->empty())) {
    a->pop();
  }
}

}  // namespace s21
