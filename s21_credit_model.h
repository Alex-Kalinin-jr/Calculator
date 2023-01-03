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

#ifndef S21_CREDIT_MODEL_H
#define S21_CREDIT_MODEL_H
#include <queue>
#include <string>

namespace s21 {

class CreditModel {
 private:
  size_t sum_ = 0;
  size_t period_ = 0;  // must be passed in months
  double percent_ = 0;
  std::queue<double> main_debt_;
  std::queue<double> percents_;

  void Clear(std::queue<double> *a);

 public:
  CreditModel();
  ~CreditModel();
  void get_credit_data(size_t sum, size_t period, double percent);
  void EvaluateAnnuity();
  void EvaluateDiff();
  std::queue<double> get_percents() const;
  std::queue<double> get_main_debt() const;
};

}  // namespace s21

#endif  // S21_CREDIT_MODEL_H
