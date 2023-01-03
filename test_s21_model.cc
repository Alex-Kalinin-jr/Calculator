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
#include <gtest/gtest.h>

#include "s21_model.h"

#define EPS 1e-6

TEST(simple_engine, a1) {
  std::string a = "345*246.5";
  s21::Model myModel;
  myModel.set_expr(a);
  myModel.ParseString();
  myModel.ConvertToRpn();
  long double val = myModel.EvaluateRpn(0);
  ASSERT_NEAR(val, 85042.5, EPS);
}

TEST(simple_engine, a2) {
  std::string a = "345*246.5(sin5)";
  s21::Model myModel;
  myModel.set_expr(a);
  myModel.ParseString();
  myModel.ConvertToRpn();
  long double val = myModel.EvaluateRpn(0);
  ASSERT_NEAR(val, -81549.317628, EPS);
}

TEST(simple_engine, a3) {
  std::string a = "sqrt(ln345*-246.5(sin5))*(tan(3-x)+4)";
  s21::Model myModel;
  long double val = 4;
  myModel.set_expr(a);
  myModel.ParseString();
  myModel.ConvertToRpn();
  val = myModel.EvaluateRpn(val);
  ASSERT_NEAR(val, 90.7799150942, EPS);
}

TEST(simple_engine, a4) {
  std::string a = "14-+6sinx";
  s21::Model myModel;
  long double val = 3;
  myModel.set_expr(a);
  myModel.ParseString();
  myModel.ConvertToRpn();
  val = myModel.EvaluateRpn(val);
  ASSERT_NEAR(val, 13.1532799516, EPS);
}

TEST(simple_engine, a5) {
  std::string a = "14+-6sin-x";
  s21::Model myModel;
  long double val = 3;
  myModel.set_expr(a);
  myModel.ParseString();
  myModel.ConvertToRpn();
  val = myModel.EvaluateRpn(val);
  ASSERT_NEAR(val, 14.8467200484, EPS);
}

TEST(simple_engine, a6) {
  std::string a = "14+-6sin(-xcosx)";
  s21::Model myModel;
  long double val = 3;
  myModel.set_expr(a);
  myModel.ParseString();
  myModel.ConvertToRpn();
  val = myModel.EvaluateRpn(val);
  ASSERT_NEAR(val, 12.9753559489, EPS);
}

TEST(simple_engine, a7) {
  std::string a = "-xcos(x+5)log(x(x+6))";
  s21::Model myModel;
  long double val = 3;
  myModel.set_expr(a);
  myModel.ParseString();
  myModel.ConvertToRpn();
  val = myModel.EvaluateRpn(val);
  ASSERT_NEAR(val, 0.62479042823, EPS);
}

TEST(simple_engine, a8) {
  std::string a = "x^tan(x(x(xsqrtx)))";
  s21::Model myModel;
  long double val = 3;
  myModel.set_expr(a);
  myModel.ParseString();
  myModel.ConvertToRpn();
  val = myModel.EvaluateRpn(val);
  ASSERT_NEAR(val, 0.662548, EPS);
}

TEST(simple_engine, a9) {
  std::string a = "x2.34acos(3.26-x)(atanx+asin(-sqrtx+1))";
  s21::Model myModel;
  long double val = 3;
  myModel.set_expr(a);
  myModel.ParseString();
  myModel.ConvertToRpn();
  val = myModel.EvaluateRpn(val);
  ASSERT_NEAR(val, 3.92669975505, EPS);
}

TEST(simple_engine, a10) {
  std::string a = "(((((50mod17)))))/4";
  s21::Model myModel;
  myModel.set_expr(a);
  myModel.ParseString();
  myModel.ConvertToRpn();
  long double val = myModel.EvaluateRpn(1);
  ASSERT_NEAR(val, 4, EPS);
}

TEST(simple_engine, a11) {
  std::string a = "e/2";
  s21::Model myModel;
  myModel.set_expr(a);
  myModel.ParseString();
  myModel.ConvertToRpn();
  long double val = myModel.EvaluateRpn(1);
  ASSERT_NEAR(val, 1.35914091423, EPS);
}

TEST(simple_engine, a12) {
  std::string a = "2^3^2";
  s21::Model myModel;
  myModel.set_expr(a);
  myModel.ParseString();
  myModel.ConvertToRpn();
  long double val = myModel.EvaluateRpn(1);
  ASSERT_NEAR(val, 512, EPS);
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
