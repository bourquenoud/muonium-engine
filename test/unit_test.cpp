/*
 * unit_test.cpp
 *
 *  Created on: 7 Apr 2021
 *      Author: mathieu
 */

/* TODO: FINISH UNIT TEST */

#include <iostream>
#include <cassert>

#include "engine/ue_config.hpp"
#include "engine/ue_engine.hpp"

#define unit_test(X) cout << endl << "-------------"#X"-------------"\
  << endl << "Line:\tTest:\t\t\tResult:" << endl; cout <<\
  ((unittest::X())?"\t\t\t\t<<OK>>": "\t\t\t\t<<FAILED>>") << endl

#define test(X, Y) if (!(X)){Y = false;};cout << __LINE__ << "\t"\
  << #X << "\t" << ((X)?" <OK>":(" <FAILED>")) << endl

using namespace ue;
using namespace std;
namespace unittest
{

  bool addition()
  {
    bool res = true;

    Real a = R(10.0);
    Real b = R(45.0);
    Real c = R(54.125);
    Real d = R(-854.0);

    test(a + a == R(20.0), res);
    test(a + b == R(55.0), res);
    test(b + c == R(99.125), res);
    test(d + b == R(-809.0), res);
    test(c + d == R(-799.875), res);

    return res;
  }

  bool subtraction()
  {
    bool res = true;

    Real a = R(10.0);
    Real b = R(45.0);
    Real c = R(54.125);
    Real d = R(-854.0);

    test(a - a == R(0.0), res);
    test(a - b == R(-35.0), res);
    test(b - c == R(-9.125), res);
    test(d - b == R(-899.0), res);
    test(c - d == R(908.125), res);

    return res;
  }

  bool multiplication()
  {
    bool res = true;

    Real a = R(10.0);
    Real b = R(45.0);
    Real c = R(54.125);
    Real d = R(-512.0);

    test(a * a == R(100.0), res);
    test(a * b == R(450.0), res);
    test(b * c == R(2435.625), res);
    test(d * b == R(-23040.0), res);
    test(c * d == R(-27712.0), res);

    return res;
  }

  bool division()
  {
    bool res = true;

    Real a = R(10.0);
    Real b = R(45.0);
    Real c = R(54.125);
    Real d = R(-512.0);

    test(a / a == R(1.0), res);
    test(b / a == R(4.5), res);
    test(b / c == R(0.83140877), res);
    test(d / b == R(-11.37777778), res);
    test(c / d == R(-0.10571289), res);

    return res;
  }

  bool vector3_addition()
  {
    bool res = true;

    Vector3 a = {R(12.4),R(874.0),R(51.0)};
    Vector3 b = {R(-24.7),R(6.2408),R(-0.00152587)};
    Vector3 c = {R(0.75),R(-84.324),R(-10.0844)};

    Vector3 resA ={R(-12.3),R(880.241),R(50.998476)};
    Vector3 resB ={R(-23.95),R(-78.082),R(10.085924)};
    Vector3 resC ={R(13.15),R(789.676),R(40.9156)};

    test(a + b == resA, res);
    test(b + c == resB, res);
    test(a + c == resC, res);

    return res;
  }

  bool matrix2_index()
  {
    bool res = true;

    Matrix2 M = {{{R(12.4),R(-54.9)},{R(1.21),R(68.54)}}};

    test(M[0][0] == R(12.4), res);
    test(M[0][1] == R(-54.9), res);
    test(M[1][0] == R(1.21), res);
    test(M[1][1] == R(68.54), res);

    return res;
  }
}

int TEST_MAIN(void)
{
  cout << endl << "**********Arithmetic*******************" << endl;
  unit_test(addition);
  unit_test(subtraction);
  unit_test(multiplication);
  unit_test(division);

  cout << endl << "**********Vector***********************" << endl;

  //Rounding error
  unit_test(vector3_addition);

  cout << endl << "**********Matrix***********************" << endl;

  unit_test(matrix2_index);

  cout << endl << "**********Quaternion*******************" << endl;
}
