// // ***********************************************************************************
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

#include "s21_controller.h"

#include <string>

#include "s21_credit_model.h"

namespace s21 {
// class for transmitting data and instructions between model and view.
// for internal funcs of controller api see apporpriate class header file.
Controller::Controller(Model *entity, CreditModel *cr_entity,
                       DepoModel *d_ent)
    : entity_(entity), credit_entity_(cr_entity), depo_entity_(d_ent) {}
Controller::~Controller() {}

int Controller::DelegateExpr(const std::string &expr) const {
    int answ = -1;
    try {
        entity_->Model::set_expr(expr);
        entity_->Model::ParseString();
        entity_->Model::ConvertToRpn();
    } catch (int i) {
        answ = i;
    }
    return answ;
}

long double Controller::ComputeAndTransmit(long double val) const {
    long double answ = entity_->Model::EvaluateRpn(val);
    return answ;
}

void Controller::PassCreditInfo(size_t sum, size_t period, double percent) {
    if (credit_entity_ != nullptr) {
        credit_entity_->get_credit_data(sum, period, percent);
    }
}

void Controller::get_credit_data(bool payment_type,
                                 std::queue<double> *interest,
                                 std::queue<double> *main) {
    if (!payment_type) {
        credit_entity_->EvaluateAnnuity();
    } else {
        credit_entity_->EvaluateDiff();
    }
    *interest = credit_entity_->get_percents();
    *main = credit_entity_->get_main_debt();
}

void Controller::PassDepoInfo(DepoInfo &a,
                                std::multimap<std::time_t, size_t> rep,
                                std::multimap<std::time_t, size_t> draw) const {
    depo_entity_->ClearDepoData();
    depo_entity_->get_depo_data(a.d_start, a.d_sum, a.d_term, a.d_capitalization,
                               a.d_interest, a.d_tax, a.d_cycle, rep, draw);
}

std::queue<std::pair<std::tm, double>> Controller::get_depo_data() {
    return depo_entity_->EvaluateDepoData();
}

std::queue<double> Controller::get_depo_sum() {
    return depo_entity_->get_depo_vals();
}

}  // namespace s21
