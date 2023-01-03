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

// MODEL is the engine for computation. Reverse Polish Notation (hereinafter
// RPN) is used. Dijkstra's algorithm is used for parsing of expression. NODE is
// a specific chunck for storing of operators and operands (hereinafter ELEM)
// with storage of relevant properties.

#ifndef S21_MODEL_H
#define S21_MODEL_H

#include <iostream>
#include <queue>
#include <stack>
#include <string>

namespace s21 {
// The main roperty of ELEM. Indicates it's nature.
enum class Action {
  kAdd,
  kSub,
  kMul,
  kDiv,
  kPow,
  kMod,
  kUnaryMinus,
  kCos,
  kSin,
  kTan,
  kAcos,
  kAsin,
  kAtan,
  kSqrt,
  kLn,
  kLog,
  kLeftBracket,
  kRightBracket,
  kNumber,
  kVariable
};
// The property for RPN creation. Used in Dijkstra's algorithm.
enum class Priority { kVal, kLow, kMedium, kHight };

// The property for RPN creation. Used in Dijkstra's algorithm.
enum class OperatorType { kUnary, kBin, kOperand, kBracket };

// NODE is used as elem for storing in containers during computation.
// Since expression to be computed is disassembled during convertion
// to RPN, NODE is exatly for saving of relevant data for all operators
// and operands.
class Node {
 public:
  Action act_ = Action::kNumber;
  Priority prior_ = Priority::kVal;
  OperatorType opnds_num_ = OperatorType::kOperand;
  long double val_ = 0;
  Node(Action act, Priority prior, OperatorType opnds_num);
  Node(long double val);
  ~Node();
};

// MODEL stores all methods of computation. It gives expression, parses it,
// sort it out into RPN and computes it, taking a certain answer.
class Model {
 public:
  Model();
  ~Model();
  // Recieves the expression in infix notation for further operations.
  // The first step in computing process
  void set_expr(const std::string out_expr);
  // Recognizes elements in given expression and creates queue of NODES
  // in the same order (infix notation).
  void ParseString();
  // Converts infix form of NODE queue to RPN using Dijkstra's algorithm.
  void ConvertToRpn();
  // Evaluate RPN expression with substitution of variable by the given number
  // In the case of variable absence (i.e. 2 + 2) any number can be passed.
  long double EvaluateRpn(long double num);

 private:
  // Stores input expression.
  std::string expr_;

  // Stores input expression in the form of NODE queue.
  std::queue<Node> parsed_;

  // Stores input expression in the RPN form.
  std::queue<Node> polish_;

  // Stores operators under certain rules while RPN convertation is going on.
  std::stack<Node> operators_;

  // These funcs create NODES.
  // &i - index in the given string "expr_". Can be incremented by theese funcs.
  void ParseUnary(size_t &i);
  // In addition checks the correctness of bracket usage.
  // l_br_num and r_br_num are counts of left and right brackets. Can be
  // incremented by func.
  void ParseBrackets(size_t &i, size_t &l_br_num, size_t &r_br_num);
  void ParseBinary(size_t &i);
  void ParseOperands(size_t &i);

  // Checks common omission of '*' in math expressions, which are clear for
  // human but does not understandable for machine. Supplements expression with
  // '*' where needed.
  void CheckUnForMulitplicatorAbsence();

  // Checks corectness of the expression regarding to binary operands usage.
  void CheckBinaryForErr(size_t &i);

  long double ComputeUnary(long double num, Action act);
  long double ComputeBinary(long double num_1, long double num_2, Action act);
};

}  // namespace s21

#endif  // S21_MODEL_H
