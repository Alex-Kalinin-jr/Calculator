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

#include "s21_model.h"

#include <cmath>
#include <queue>
#include <stack>
#include <string>

namespace s21 {

std::string digits = "0123456789.,";

Node::Node(Action act, Priority prior, OperatorType opnd)
    : act_(act), prior_(prior), opnds_num_(opnd) {}

Node::Node(long double val) : val_(val){};

Node::~Node() {}

Model::Model() : expr_("") {}

Model::~Model(){};

void Model::set_expr(const std::string out_expr) { expr_ = out_expr; }

void Model::ParseString() {
  size_t i = 0;
  size_t l_br_num = 0;  // stores the number of left brackets for check of
                        // correct brackets usage
  size_t r_br_num = 0;  // stores the number of right brackets for check of
                        // correct brackets usage

  while (i < expr_.std::string::size()) {
    ParseUnary(i);
    ParseOperands(i);
    ParseBinary(i);
    ParseBrackets(i, l_br_num, r_br_num);
  }
  if (l_br_num != r_br_num) {
    throw -11;
  }
  Action check = parsed_.std::queue<Node>::back().act_;
  if (check != Action::kVariable && check != Action::kNumber &&
      check != Action::kRightBracket) {
    throw -12;
  }
}

// duplication of "CheckUnForMulitplicatorAbsence()" is because ONE function
// call can find and create NODES of several consecutive unary operands. Can be
// rewritten into more compact and readable form (using "ELIF") but would create
// only 1 operand during one call.
void Model::ParseUnary(size_t &i) {
  if (expr_[i] == 'c' && expr_[i + 1] == 'o' && expr_[i + 2] == 's') {
    Model::CheckUnForMulitplicatorAbsence();
    parsed_.std::queue<Node>::push(
        Node(Action::kCos, Priority::kHight, OperatorType::kUnary));
    i += 3;
  }
  if (expr_[i] == 's' && expr_[i + 1] == 'i' && expr_[i + 2] == 'n') {
    Model::CheckUnForMulitplicatorAbsence();
    parsed_.std::queue<Node>::push(
        Node(Action::kSin, Priority::kHight, OperatorType::kUnary));
    i += 3;
  }
  if (expr_[i] == 't' && expr_[i + 1] == 'a' && expr_[i + 2] == 'n') {
    Model::CheckUnForMulitplicatorAbsence();
    parsed_.std::queue<Node>::push(
        Node(Action::kTan, Priority::kHight, OperatorType::kUnary));
    i += 3;
  }
  if (expr_[i] == 'a' && expr_[i + 1] == 'c' && expr_[i + 2] == 'o' &&
      expr_[i + 3] == 's') {
    Model::CheckUnForMulitplicatorAbsence();
    parsed_.std::queue<Node>::push(
        Node(Action::kAcos, Priority::kHight, OperatorType::kUnary));
    i += 4;
  }
  if (expr_[i] == 'a' && expr_[i + 1] == 's' && expr_[i + 2] == 'i' &&
      expr_[i + 3] == 'n') {
    Model::CheckUnForMulitplicatorAbsence();
    parsed_.std::queue<Node>::push(
        Node(Action::kAsin, Priority::kHight, OperatorType::kUnary));
    i += 4;
  }
  if (expr_[i] == 'a' && expr_[i + 1] == 't' && expr_[i + 2] == 'a' &&
      expr_[i + 3] == 'n') {
    Model::CheckUnForMulitplicatorAbsence();
    parsed_.std::queue<Node>::push(
        Node(Action::kAtan, Priority::kHight, OperatorType::kUnary));
    i += 4;
  }
  if (expr_[i] == 's' && expr_[i + 1] == 'q' && expr_[i + 2] == 'r' &&
      expr_[i + 3] == 't') {
    Model::CheckUnForMulitplicatorAbsence();
    parsed_.std::queue<Node>::push(
        Node(Action::kSqrt, Priority::kHight, OperatorType::kUnary));
    i += 4;
  }
  if (expr_[i] == 'l' && expr_[i + 1] == 'n') {
    Model::CheckUnForMulitplicatorAbsence();
    parsed_.std::queue<Node>::push(
        Node(Action::kLn, Priority::kHight, OperatorType::kUnary));
    i += 2;
  }
  if (expr_[i] == 'l' && expr_[i + 1] == 'o' && expr_[i + 2] == 'g') {
    Model::CheckUnForMulitplicatorAbsence();
    parsed_.std::queue<Node>::push(
        Node(Action::kLog, Priority::kHight, OperatorType::kUnary));
    i += 3;
  }
}

void Model::ParseOperands(size_t &i) {
  if (digits.std::string::find(expr_[i]) < digits.std::string::length()) {
    size_t new_pos = 0;
    long double val = std::stold(expr_.substr(i), &new_pos);
    CheckUnForMulitplicatorAbsence();
    parsed_.std::queue<Node>::push(Node(val));
    i += new_pos;
  }
  if (expr_[i] == 'x') {
    CheckUnForMulitplicatorAbsence();
    parsed_.std::queue<Node>::push(
        Node(Action::kVariable, Priority::kVal, OperatorType::kOperand));
    ++i;
  }
  if (expr_[i] == 'e') {
    CheckUnForMulitplicatorAbsence();
    long double val = 2.71828182846;
    parsed_.std::queue<Node>::push(Node(val));
    ++i;
  }
}

// contains some clearly visible conditions to define "+" or "-" as unary
// operands.
void Model::ParseBinary(size_t &i) {
  if (expr_[i] == '+') {
    if (!parsed_.std::queue<Node>::empty()) {
      Action check = parsed_.std::queue<Node>::back().act_;
      if (check != Action::kNumber && check != Action::kRightBracket &&
          check != Action::kVariable) {
        ++i;
      } else {
        parsed_.std::queue<Node>::push(
            Node(Action::kAdd, Priority::kLow, OperatorType::kBin));
      }
    }
  }
  if (expr_[i] == '-') {
    if (parsed_.std::queue<Node>::empty()) {
      parsed_.std::queue<Node>::push(
          Node(Action::kUnaryMinus, Priority::kHight, OperatorType::kUnary));
    } else {
      Action check = parsed_.std::queue<Node>::back().act_;
      if (check != Action::kNumber && check != Action::kRightBracket &&
          check != Action::kVariable) {
        parsed_.std::queue<Node>::push(
            Node(Action::kUnaryMinus, Priority::kHight, OperatorType::kUnary));
      } else {
        parsed_.std::queue<Node>::push(
            Node(Action::kSub, Priority::kLow, OperatorType::kBin));
      }
    }
    ++i;
  }
  if (expr_[i] == '*') {
    CheckBinaryForErr(i);
    parsed_.std::queue<Node>::push(
        Node(Action::kMul, Priority::kMedium, OperatorType::kBin));
    ++i;
  }
  if (expr_[i] == '/') {
    CheckBinaryForErr(i);
    parsed_.std::queue<Node>::push(
        Node(Action::kDiv, Priority::kMedium, OperatorType::kBin));
    ++i;
  }
  if (expr_[i] == '^') {
    CheckBinaryForErr(i);
    parsed_.std::queue<Node>::push(
        Node(Action::kPow, Priority::kHight, OperatorType::kBin));
    ++i;
  }
  if (expr_[i] == 'm' && expr_[i + 1] == 'o' && expr_[i + 2] == 'd') {
    CheckBinaryForErr(i);
    parsed_.std::queue<Node>::push(
        Node(Action::kMod, Priority::kMedium, OperatorType::kBin));
    i += 3;
  }
}

void Model::CheckBinaryForErr(size_t &i) {
  if (i == 0) {
    throw static_cast<int>(i);
  }
  Action check = parsed_.std::queue<Node>::back().act_;
  if (check != Action::kNumber && check != Action::kRightBracket &&
      check != Action::kVariable) {
    throw static_cast<int>(i);
  }
}

void Model::CheckUnForMulitplicatorAbsence() {
  if (!parsed_.std::queue<Node>::empty()) {
    Action check = parsed_.std::queue<Node>::back().act_;
    if (check == Action::kNumber || check == Action::kRightBracket ||
        check == Action::kVariable) {
      parsed_.std::queue<Node>::push(
          Node(Action::kMul, Priority::kMedium, OperatorType::kBin));
    }
  }
}

void Model::ParseBrackets(size_t &i, size_t &l_br_num, size_t &r_br_num) {
  if (expr_[i] == ')') {
    if (l_br_num <= r_br_num) {
      throw static_cast<int>(i);
    }
    Action check = parsed_.std::queue<Node>::back().act_;
    if (check != Action::kNumber && check != Action::kVariable &&
        check != Action::kRightBracket) {
      throw static_cast<int>(i);
    }
    parsed_.std::queue<Node>::push(
        Node(Action::kRightBracket, Priority::kLow, OperatorType::kBracket));
    r_br_num += 1;
    ++i;
  }
  if (expr_[i] == '(') {
    CheckUnForMulitplicatorAbsence();
    parsed_.std::queue<Node>::push(
        Node(Action::kLeftBracket, Priority::kLow, OperatorType::kBracket));
    ++i;
    l_br_num += 1;
  }
}

void Model::ConvertToRpn() {
  size_t i = parsed_.std::queue<Node>::size();
  while (i > 0) {
    Node buff = parsed_.std::queue<Node>::front();
    parsed_.std::queue<Node>::pop();
    if (buff.act_ == Action::kNumber || buff.act_ == Action::kVariable) {
      polish_.std::queue<Node>::push(buff);
    } else if (buff.act_ == Action::kLeftBracket) {
      operators_.std::stack<Node>::push(buff);
    } else if (buff.act_ == Action::kRightBracket) {
      while (operators_.std::stack<Node>::top().act_ != Action::kLeftBracket) {
        polish_.std::queue<Node>::push(operators_.std::stack<Node>::top());
        operators_.std::stack<Node>::pop();
      }
      operators_.pop();
    } else if (buff.opnds_num_ == OperatorType::kUnary) {
      operators_.push(buff);
    } else {
      if (!operators_.empty() &&
          // the sec condition is for "right-to-left" computation of
          // sequentional powers (2^3^3 = 2^[3^3])
          !(buff.act_ == Action::kPow &&
            operators_.top().act_ == Action::kPow)) {
        while (!operators_.empty() && operators_.top().prior_ >= buff.prior_ &&
               operators_.top().act_ != Action::kLeftBracket) {
          polish_.std::queue<Node>::push(operators_.std::stack<Node>::top());
          operators_.std::stack<Node>::pop();
        }
      }
      operators_.std::stack<Node>::push(buff);
    }
    --i;
  }
  while (!operators_.std::stack<Node>::empty()) {
    polish_.std::queue<Node>::push(operators_.std::stack<Node>::top());
    operators_.std::stack<Node>::pop();
  }
}

long double Model::EvaluateRpn(long double num) {
  std::stack<Node> eval_stack;
  std::queue<Node> eval_queue(polish_);
  while (!eval_queue.std::queue<Node>::empty()) {
    Node buff = eval_queue.std::queue<Node>::front();
    eval_queue.std::queue<Node>::pop();
    if (buff.act_ == Action::kVariable) {
      buff.val_ = num;
    }
    if (buff.act_ == Action::kNumber || buff.act_ == Action::kVariable) {
      eval_stack.std::stack<Node>::push(buff);
    } else {
      if (buff.opnds_num_ == OperatorType::kUnary) {
        Node buff_num = eval_stack.std::stack<Node>::top();
        eval_stack.std::stack<Node>::pop();
        buff_num.val_ = ComputeUnary(buff_num.val_, buff.act_);
        eval_stack.std::stack<Node>::push(buff_num);
      } else if (buff.opnds_num_ == OperatorType::kBin) {
        Node buff_num_2 = eval_stack.std::stack<Node>::top();
        eval_stack.std::stack<Node>::pop();
        Node buff_num_1 = eval_stack.std::stack<Node>::top();
        eval_stack.std::stack<Node>::pop();
        buff_num_1.val_ =
            ComputeBinary(buff_num_1.val_, buff_num_2.val_, buff.act_);
        eval_stack.std::stack<Node>::push(buff_num_1);
      }
    }
  }
  return eval_stack.std::stack<Node>::top().val_;
}

long double Model::ComputeBinary(long double num_1, long double num_2,
                                 Action act) {
  long double result = 0;
  switch (act) {
    case Action::kAdd: {
      result = num_1 + num_2;
      break;
    }
    case Action::kSub: {
      result = num_1 - num_2;
      break;
    }
    case Action::kMul: {
      result = num_1 * num_2;
      break;
    }
    case Action::kDiv: {
      result = num_1 / num_2;
      break;
    }
    case Action::kPow: {
      result = powl(num_1, num_2);
      break;
    }
    case Action::kMod: {
      result = fmodl(num_1, num_2);
      break;
    }
    default: {
      break;
    }
  }
  return result;
}

long double Model::ComputeUnary(long double num, Action act) {
  long double result = 0;
  switch (act) {
    case Action::kCos: {
      result = cosl(num);
      break;
    }
    case Action::kSin: {
      result = sinl(num);
      break;
    }
    case Action::kTan: {
      result = tanl(num);
      break;
    }
    case Action::kAcos: {
      result = acosl(num);
      break;
    }
    case Action::kAsin: {
      result = asinl(num);
      break;
    }
    case Action::kAtan: {
      result = atanl(num);
      break;
    }
    case Action::kSqrt: {
      result = sqrtl(num);
      break;
    }
    case Action::kLn: {
      result = logl(num);
      break;
    }
    case Action::kLog: {
      result = log10l(num);
      break;
    }
    case Action::kUnaryMinus: {
      result = -num;
      break;
    }
    default: {
      break;
    }
  }
  return result;
}

}  // namespace s21
