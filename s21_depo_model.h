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

#ifndef S21_DEPO_MODEL_H
#define S21_DEPO_MODEL_H

#include <ctime>
#include <iomanip>
#include <map>
#include <queue>

namespace s21 {

// DEPO-MODEL is the engine for computation of deposit benefits.
class DepoModel {
 public:
  DepoModel();
  ~DepoModel();
  DepoModel(const DepoModel& other) = default;
  DepoModel& operator=(const DepoModel& other) = default;
  DepoModel(DepoModel&& other) = delete;
  DepoModel& operator=(DepoModel&& other) = delete;
  // start - date of deposit(depo), sum of depo, term of depo(months),
  // capitalization(no/yes), intetest of depo(%), government tax(%),
  // periodicity of payments(Daily, Weekly, Monthly, Quarterly, Anually),
  // multimap of needed replenisments(date/sum), the same for withdrawals)
  void get_depo_data(time_t start, size_t sum, size_t term, size_t d_cap,
                     double intrst, double tax, size_t cycle,
                     std::multimap<std::time_t, size_t> rep,
                     std::multimap<std::time_t, size_t> draw);
  // computes resultat of having depo under certain conditions.
  // as a result forms the answ_.
  std::queue<std::pair<std::tm, double>> EvaluateDepoData();
  // zeroes all parameters and clears all containers.
  void ClearDepoData();

  std::queue<double> get_depo_vals() const;

 private:
  // adds term to start and converts in necessary time format.
  void ComputeEnd();

  // adds members in queue d_chekpoints_ according to d_cycle_ -
  // periodicity of payments. For instanse d_cycle_=1(weekly),
  // depo was started 1 Jan 2023. Queue would be 8 Jan, 15 Jan, ...
  void ComputeCheckpoints();

  // parses two multimaps repls_(table of replenishments date/sum),
  // and withdraw_ (another table, name is clear) according to passed "counter".
  // Here counter means today, ant this today is compared with tables.
  // Has access to buff_sum, changes it in accordance with tables.
  void AddDelMoney(time_t, double *);

  time_t d_start_ = 0;           // start - date of deposit(depo)
  size_t d_sum_ = 0;             // sum of depo
  size_t d_term_ = 0;            // term of depo(months)
  size_t d_capitalization_ = 0;  // capitalization(no/yes)
  double d_interest_ = 0;        // intetest of depo(%)
  double d_tax_ = 0;             // government tax(%)
  size_t d_cycle_ = 0;  // periodicity of payments(Daily, Weekly, Monthly,
                        // Quarterly, Anually)
  std::multimap<std::time_t, size_t> repls_;     // multimap of needed
                                                 // replenisments(date/sum)
  std::multimap<std::time_t, size_t> withdraw_;  // the same for withdrawals

  std::time_t d_end_ = 0;
  std::queue<std::time_t> d_checkpoints_;
  // answer to be passed to view.
  std::queue<std::pair<std::tm, double>> answ_;
  // to be passed to view.
  std::queue<double> depo_val_;
  bool zero_balance_ = false;
};

}  // namespace s21
#endif  // S21_DEPO_MODEL_H
