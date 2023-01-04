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

#ifndef SRC_CALC_ERROR_HANDLER
#define SRC_CALC_ERROR_HANDLER

#include <iostream>
#include <map>
#include <string>

// self-sufficient class without relevant *.cc file. Implemented as a class
// for further scallability.
namespace s21 {
class ErrorHandler {
   public:
  ErrorHandler(){};
  ~ErrorHandler(){};
  ErrorHandler(const ErrorHandler& other) = default;
  ErrorHandler& operator=(const ErrorHandler& other) = default;
  ErrorHandler(ErrorHandler&& other) = delete;
  ErrorHandler& operator=(ErrorHandler&& other) = delete;
  std::string TransmitMsg(const int code) const;

 private:
  const std::map<int, std::string> kErrors_ = {
      {-11, "wrong brackets"},  // linux return-code style. if want - you can
      {-12,
       "wrong end of expression"},  // replace numbers with more clear ENUM.
      {-10, "expression is empty"},
      {-9, "wrong expression: conform operation and operand quantity"},
      {-8, "wrong expression"},
      {-7, "wrong expression"},
      {-6, "invalid variable"},
      {-5, "invalid value for X-range"},
      {-4, "rignt border of the range should be greater than left"}};
};

std::string ErrorHandler::TransmitMsg(const int code) const {
  std::string msg = "unknown error";
  auto search = kErrors_.std::map<int, std::string>::find(code);
  if (search != kErrors_.end()) {
    msg = search->second;
  }
  return msg;
}

}  // namespace s21

#endif  // SRC_CALC_ERROR_HANDLER
