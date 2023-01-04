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

#ifndef S21_CONTROLLER_H
#define S21_CONTROLLER_H
#include <ctime>
#include <map>
#include <string>

#include "s21_credit_model.h"
#include "s21_depo_model.h"
#include "s21_model.h"

namespace s21 {
// stores all needed info for deposit data computation. Passed via
// controller to depo_model.
struct DepoInfo {
  // start of depo (default is 1 jan 2000, implicitly in seconds)
  std::time_t d_start = 60907568400;
  size_t d_sum = 0;
  size_t d_term = 0;
  // no/yes
  size_t d_capitalization = 0;
  // annual interest in %.
  double d_interest = 0;
  // government tax in %.
  double d_tax = 0;
  // periodicity of payments daily, weeekly monthly quarterly, anually
  size_t d_cycle = 0;
};
// connects view and model between each other. passes all info through.
class Controller {
 public:
  explicit Controller(Model *entity, CreditModel *credit_entity,
                      DepoModel *d_ent);
  ~Controller();
  Controller(const Controller& other) = default;
  Controller& operator=(const Controller& other) = default;
  Controller(Controller&& other) = delete;
  Controller& operator=(Controller&& other) = delete;
  // passes math expression from view to model
  int DelegateExpr(const std::string &expr) const;
  // recieves a computational answer and passes it to view.
  long double ComputeAndTransmit(long double val) const;
  // passes data for computation from view to credit_model
  void PassCreditInfo(size_t sum, size_t period, double percent);
  // passes computed data from credit_model to view
  void get_credit_data(bool payment_type, std::queue<double> *interest,
                       std::queue<double> *main);
  // struct with all the data &a, table of replenishments, table of withdrawals
  void PassDepoInfo(DepoInfo &a, std::multimap<std::time_t, size_t> rep,
                    std::multimap<std::time_t, size_t> draw) const;
  // passes computed queue "benefit at a certain time".
  std::queue<std::pair<std::tm, double>> get_depo_data();
  // the same "total on bank account". queue correlates with date of
  // get_depo_data().
  std::queue<double> get_depo_sum();

 private:
  Model *entity_ = nullptr;
  CreditModel *credit_entity_ = nullptr;
  DepoModel *depo_entity_ = nullptr;
};
}  // namespace s21

#endif  // S21_CONTROLLER_H
