// ***********************************************************************************
// ** Smart Calculator - computation tool, easy to scale with new functionality     **
// ** Copyright 2023 Alexey Kalinin                                                 **
// **                                                                               **
// ** This program is free software: you can redistribute it and/or modify          **
// ** it under the terms of the GNU General Public License as published by          **
// ** the Free Software Foundation, either version 3 of the License, or             **
// ** (at your option) any later version.                                           **
// **                                                                               **
// ** This program is distributed in the hope that it will be useful,               **
// ** but WITHOUT ANY WARRANTY; without even the implied warranty of                **
// ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                 **
// ** GNU General Public License for more details.                                  **
// **                                                                               **
// ** You should have received a copy of the GNU General Public License             **
// ** along with this program.  If not, see <http://www.gnu.org/licenses/>.         **
// ***********************************************************************************

#include "s21_depo_model.h"

#include <ctime>
#include <iomanip>
#include <iostream>
#include <map>
#include <queue>

namespace s21 {

DepoModel::DepoModel() {}

DepoModel::~DepoModel() {}

void DepoModel::get_depo_data(time_t start, size_t sum, size_t term,
                               size_t cap, double intrst, double tax,
                               size_t cycle,
                               std::multimap<std::time_t, size_t> rep,
                               std::multimap<std::time_t, size_t> draw) {
    d_start_ = start;
    d_sum_ = sum;
    d_term_ = term;
    d_capitalization_ = cap;
    d_interest_ = intrst;
    d_tax_ = tax;
    d_cycle_ = cycle;
    repls_ = rep;
    withdraw_ = draw;
}

std::queue<std::pair<std::tm, double>> DepoModel::EvaluateDepoData() {
    ComputeEnd();
    ComputeCheckpoints();

    size_t one_day = 60 * 60 * 24;
    double daily_interest = d_interest_ / 100 / 365;
    double factor = 0;
    time_t counter = d_start_;
    double buff_sum = (double)d_sum_;

    while (counter < d_end_ && !zero_balance_) {
        counter += one_day;
        factor += daily_interest;
 // checks multimaps and if date finds itself in multimap the "answer_" adds or
 // looses money
        AddDelMoney(counter, &buff_sum);
 // counts current interest by adding daily interest in buffer and changing date by 1
 // in a loop. For every day checks with "d_chekpoints_" of payments.
 // zero balance is inserted to check if withdrawals after "AddDelMoney(..)"
 // were bigger than the sum on depo.
        if ((counter == d_checkpoints_.front() && !zero_balance_) ||
            counter == d_end_) {
            double plus = buff_sum * factor;
            plus *= 1 - d_tax_ / 100;
            factor = 0;
            d_checkpoints_.pop();
            answ_.push(
                std::pair<std::tm, double>(*(std::localtime(&counter)), plus));
            if (d_capitalization_ == 1) {
                buff_sum += plus;
            }
            depo_val_.push(buff_sum);
        }
    }
    return answ_;
}

void DepoModel::ComputeEnd() {
    std::tm *end_buff = std::localtime(&d_start_);
    end_buff->tm_mon += d_term_;
    d_end_ = std::mktime(end_buff);
}

void DepoModel::ComputeCheckpoints() {
    std::tm *buff = std::localtime(&d_start_);
    while (std::mktime(buff) < d_end_) {
        switch (d_cycle_) {
            case 0:
                buff->tm_mday += 1;
                break;
            case 1:
                buff->tm_mday += 7;
                break;
            case 2:
                buff->tm_mon += 1;
                break;
            case 3:
                buff->tm_mon += 3;
                break;
            case 4:
                buff->tm_year += 1;
        }
        if (std::mktime(buff) <= d_end_) {
            d_checkpoints_.push(std::mktime(buff));
        }
    }
}

void DepoModel::AddDelMoney(time_t counter, double *buff_sum) {
    auto b = repls_.find(counter);
    while (b != repls_.end()) {
        *buff_sum += b->second;
        repls_.erase(b);
        b = repls_.find(counter);
    }
    b = withdraw_.find(counter);
    while (b != withdraw_.end()) {
        *buff_sum -= b->second;
        withdraw_.erase(b);
        b = withdraw_.find(counter);
        if (*buff_sum <= 0) {
            zero_balance_ = true;
            *buff_sum = 0;
        }
    }
}

void DepoModel::ClearDepoData() {
    d_start_ = 0;
    d_sum_ = 0;
    d_term_ = 0;
    d_capitalization_ = 0;
    d_interest_ = 0;
    d_tax_ = 0;
    d_cycle_ = 0;
    repls_.clear();
    withdraw_.clear();
    d_end_ = 0;
    while (!d_checkpoints_.empty()) {
        d_checkpoints_.pop();
    }
    while (!answ_.empty()) {
        answ_.pop();
    }
    while (!depo_val_.empty()) {
        depo_val_.pop();
    }
    zero_balance_ = false;
}

std::queue<double> DepoModel::get_depo_vals() const { return depo_val_; }

}  // namespace s21
