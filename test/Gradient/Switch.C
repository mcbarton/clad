// RUN: %cladclang -Xclang -plugin-arg-clad -Xclang -disable-tbr %s -I%S/../../include -oSwitch.out 2>&1 -lm | %filecheck %s
// RUN: ./Switch.out | %filecheck_exec %s

#include "clad/Differentiator/Differentiator.h"
#include "../TestUtils.h"

double fn1(double i, double j) {
  double res = 0;
  int count = 1;
  switch (count) {
    case 0: res += i * j; break;
    case 1: res += i * i; {
        case 2: res += j * j;
      }
    default: res += i * i * j * j;
  }
  return res;
}

// CHECK: void fn1_grad(double i, double j, double *_d_i, double *_d_j) {
// CHECK-NEXT:     int _cond0;
// CHECK-NEXT:     double _t0;
// CHECK-NEXT:     clad::tape<unsigned {{int|long|long long}}> _t1 = {};
// CHECK-NEXT:     double _t2;
// CHECK-NEXT:     double _t3;
// CHECK-NEXT:     double _t4;
// CHECK-NEXT:     double _d_res = 0.;
// CHECK-NEXT:     double res = 0;
// CHECK-NEXT:     int _d_count = 0;
// CHECK-NEXT:     int count = 1;
// CHECK-NEXT:     {
// CHECK-NEXT:         _cond0 = count;
// CHECK-NEXT:         switch (_cond0) {
// CHECK-NEXT:             {
// CHECK-NEXT:               case 0:
// CHECK-NEXT:                 res += i * j;
// CHECK-NEXT:                 _t0 = res;
// CHECK-NEXT:             }
// CHECK-NEXT:             {
// CHECK-NEXT:                 clad::push(_t1, {{1U|1UL|1ULL}});
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:             {
// CHECK-NEXT:               case 1:
// CHECK-NEXT:                 res += i * i;
// CHECK-NEXT:                 _t2 = res;
// CHECK-NEXT:             }
// CHECK-NEXT:             {
// CHECK-NEXT:                 {
// CHECK-NEXT:                   case 2:
// CHECK-NEXT:                     res += j * j;
// CHECK-NEXT:                     _t3 = res;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             {
// CHECK-NEXT:               default:
// CHECK-NEXT:                 res += i * i * j * j;
// CHECK-NEXT:                 _t4 = res;
// CHECK-NEXT:             }
// CHECK-NEXT:             clad::push(_t1, {{2U|2UL|2ULL}});
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     _d_res += 1;
// CHECK-NEXT:     {
// CHECK-NEXT:         switch (clad::pop(_t1)) {
// CHECK-NEXT:           case {{2U|2UL|2ULL}}:
// CHECK-NEXT:             ;
// CHECK-NEXT:             {
// CHECK-NEXT:                 {
// CHECK-NEXT:                     res = _t4;
// CHECK-NEXT:                     double _r_d3 = _d_res;
// CHECK-NEXT:                     *_d_i += _r_d3 * j * j * i;
// CHECK-NEXT:                     *_d_i += i * _r_d3 * j * j;
// CHECK-NEXT:                     *_d_j += i * i * _r_d3 * j;
// CHECK-NEXT:                     *_d_j += i * i * j * _r_d3;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if (_cond0 != 0 && _cond0 != 1 && _cond0 != 2)
// CHECK-NEXT:                     break;
// CHECK-NEXT:             }
// CHECK-NEXT:             {
// CHECK-NEXT:                 {
// CHECK-NEXT:                     {
// CHECK-NEXT:                         res = _t3;
// CHECK-NEXT:                         double _r_d2 = _d_res;
// CHECK-NEXT:                         *_d_j += _r_d2 * j;
// CHECK-NEXT:                         *_d_j += j * _r_d2;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     if (2 == _cond0)
// CHECK-NEXT:                         break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             {
// CHECK-NEXT:                 {
// CHECK-NEXT:                     res = _t2;
// CHECK-NEXT:                     double _r_d1 = _d_res;
// CHECK-NEXT:                     *_d_i += _r_d1 * i;
// CHECK-NEXT:                     *_d_i += i * _r_d1;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if (1 == _cond0)
// CHECK-NEXT:                     break;
// CHECK-NEXT:             }
// CHECK-NEXT:           case {{1U|1UL|1ULL}}:
// CHECK-NEXT:             ;
// CHECK-NEXT:             {
// CHECK-NEXT:                 {
// CHECK-NEXT:                     res = _t0;
// CHECK-NEXT:                     double _r_d0 = _d_res;
// CHECK-NEXT:                     *_d_i += _r_d0 * j;
// CHECK-NEXT:                     *_d_j += i * _r_d0;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if (0 == _cond0)
// CHECK-NEXT:                     break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }

double fn2(double i, double j) {
  double res = 0;
  switch (int count = 2) {
    res += i * i * j * j;
    res += 50 * i;
    case 0: res += i; break;
    case 1: res += j;
    case 2: res += i * j; break;
    default: res += i + j;
  }
  return res;
}

// CHECK: void fn2_grad(double i, double j, double *_d_i, double *_d_j) {
// CHECK-NEXT:     int _d_count = 0;
// CHECK-NEXT:     int count = 0;
// CHECK-NEXT:     int _cond0;
// CHECK-NEXT:     double _t0;
// CHECK-NEXT:     double _t1;
// CHECK-NEXT:     double _t2;
// CHECK-NEXT:     clad::tape<unsigned {{int|long|long long}}> _t3 = {};
// CHECK-NEXT:     double _t4;
// CHECK-NEXT:     double _t5;
// CHECK-NEXT:     double _t6;
// CHECK-NEXT:     double _d_res = 0.;
// CHECK-NEXT:     double res = 0;
// CHECK-NEXT:     {
// CHECK-NEXT:         count = 2;
// CHECK-NEXT:         _cond0 = count;
// CHECK-NEXT:         switch (_cond0) {
// CHECK-NEXT:             _t0 = res;
// CHECK-NEXT:             res += i * i * j * j;
// CHECK-NEXT:             _t1 = res;
// CHECK-NEXT:             res += 50 * i;
// CHECK-NEXT:             {
// CHECK-NEXT:               case 0:
// CHECK-NEXT:                 res += i;
// CHECK-NEXT:                 _t2 = res;
// CHECK-NEXT:             }
// CHECK-NEXT:             {
// CHECK-NEXT:                 clad::push(_t3, {{1U|1UL|1ULL}});
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:             {
// CHECK-NEXT:               case 1:
// CHECK-NEXT:                 res += j;
// CHECK-NEXT:                 _t4 = res;
// CHECK-NEXT:             }
// CHECK-NEXT:             {
// CHECK-NEXT:               case 2:
// CHECK-NEXT:                 res += i * j;
// CHECK-NEXT:                 _t5 = res;
// CHECK-NEXT:             }
// CHECK-NEXT:             {
// CHECK-NEXT:                 clad::push(_t3, {{2U|2UL|2ULL}});
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:             {
// CHECK-NEXT:               default:
// CHECK-NEXT:                 res += i + j;
// CHECK-NEXT:                 _t6 = res;
// CHECK-NEXT:             }
// CHECK-NEXT:             clad::push(_t3, {{3U|3UL|3ULL}});
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     _d_res += 1;
// CHECK-NEXT:     {
// CHECK-NEXT:         switch (clad::pop(_t3)) {
// CHECK-NEXT:           case {{3U|3UL|3ULL}}:
// CHECK-NEXT:             ;
// CHECK-NEXT:             {
// CHECK-NEXT:                 {
// CHECK-NEXT:                     res = _t6;
// CHECK-NEXT:                     double _r_d5 = _d_res;
// CHECK-NEXT:                     *_d_i += _r_d5;
// CHECK-NEXT:                     *_d_j += _r_d5;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if (_cond0 != 0 && _cond0 != 1 && _cond0 != 2)
// CHECK-NEXT:                     break;
// CHECK-NEXT:             }
// CHECK-NEXT:           case {{2U|2UL|2ULL}}:
// CHECK-NEXT:             ;
// CHECK-NEXT:             {
// CHECK-NEXT:                 {
// CHECK-NEXT:                     res = _t5;
// CHECK-NEXT:                     double _r_d4 = _d_res;
// CHECK-NEXT:                     *_d_i += _r_d4 * j;
// CHECK-NEXT:                     *_d_j += i * _r_d4;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if (2 == _cond0)
// CHECK-NEXT:                     break;
// CHECK-NEXT:             }
// CHECK-NEXT:             {
// CHECK-NEXT:                 {
// CHECK-NEXT:                     res = _t4;
// CHECK-NEXT:                     double _r_d3 = _d_res;
// CHECK-NEXT:                     *_d_j += _r_d3;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if (1 == _cond0)
// CHECK-NEXT:                     break;
// CHECK-NEXT:             }
// CHECK-NEXT:           case {{1U|1UL|1ULL}}:
// CHECK-NEXT:             ;
// CHECK-NEXT:             {
// CHECK-NEXT:                 {
// CHECK-NEXT:                     res = _t2;
// CHECK-NEXT:                     double _r_d2 = _d_res;
// CHECK-NEXT:                     *_d_i += _r_d2;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if (0 == _cond0)
// CHECK-NEXT:                     break;
// CHECK-NEXT:             }
// CHECK-NEXT:             {
// CHECK-NEXT:                 res = _t1;
// CHECK-NEXT:                 double _r_d1 = _d_res;
// CHECK-NEXT:                 *_d_i += 50 * _r_d1;
// CHECK-NEXT:             }
// CHECK-NEXT:             {
// CHECK-NEXT:                 res = _t0;
// CHECK-NEXT:                 double _r_d0 = _d_res;
// CHECK-NEXT:                 *_d_i += _r_d0 * j * j * i;
// CHECK-NEXT:                 *_d_i += i * _r_d0 * j * j;
// CHECK-NEXT:                 *_d_j += i * i * _r_d0 * j;
// CHECK-NEXT:                 *_d_j += i * i * j * _r_d0;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }

double fn3(double i, double j) {
  double res = 0;
  int counter = 2;
  while (counter--) {
    switch (counter) {
      case 0: res += i * i * j * j;
      case 1: {
        res += i * i;
      } break;
      case 2: res += j * j;
      default: res += i + j;
    }
  }
  return res;
}

// CHECK: void fn3_grad(double i, double j, double *_d_i, double *_d_j) {
// CHECK-NEXT:     clad::tape<int> _cond0 = {};
// CHECK-NEXT:     clad::tape<double> _t1 = {};
// CHECK-NEXT:     clad::tape<double> _t2 = {};
// CHECK-NEXT:     clad::tape<unsigned {{int|long|long long}}> _t3 = {};
// CHECK-NEXT:     clad::tape<double> _t4 = {};
// CHECK-NEXT:     clad::tape<double> _t5 = {};
// CHECK-NEXT:     double _d_res = 0.;
// CHECK-NEXT:     double res = 0;
// CHECK-NEXT:     int _d_counter = 0;
// CHECK-NEXT:     int counter = 2;
// CHECK-NEXT:     unsigned {{int|long|long long}} _t0 = {{0U|0UL|0ULL}};
// CHECK-NEXT:     while (counter--)
// CHECK-NEXT:         {
// CHECK-NEXT:             _t0++;
// CHECK-NEXT:             {
// CHECK-NEXT:                 clad::push(_cond0, counter);
// CHECK-NEXT:                 switch (clad::back(_cond0)) {
// CHECK-NEXT:                     {
// CHECK-NEXT:                       case 0:
// CHECK-NEXT:                         res += i * i * j * j;
// CHECK-NEXT:                         clad::push(_t1, res);
// CHECK-NEXT:                     }
// CHECK-NEXT:                     {
// CHECK-NEXT:                       case 1:
// CHECK-NEXT:                         {
// CHECK-NEXT:                             clad::push(_t2, res);
// CHECK-NEXT:                             res += i * i;
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
// CHECK-NEXT:                     {
// CHECK-NEXT:                         clad::push(_t3, {{1U|1UL|1ULL}});
// CHECK-NEXT:                         break;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     {
// CHECK-NEXT:                       case 2:
// CHECK-NEXT:                         res += j * j;
// CHECK-NEXT:                         clad::push(_t4, res);
// CHECK-NEXT:                     }
// CHECK-NEXT:                     {
// CHECK-NEXT:                       default:
// CHECK-NEXT:                         res += i + j;
// CHECK-NEXT:                         clad::push(_t5, res);
// CHECK-NEXT:                     }
// CHECK-NEXT:                     clad::push(_t3, {{2U|2UL|2ULL}});
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     _d_res += 1;
// CHECK-NEXT:     while (_t0)
// CHECK-NEXT:         {
// CHECK-NEXT:             {
// CHECK-NEXT:                 {
// CHECK-NEXT:                     switch (clad::pop(_t3)) {
// CHECK-NEXT:                       case {{2U|2UL|2ULL}}:
// CHECK-NEXT:                         ;
// CHECK-NEXT:                         {
// CHECK-NEXT:                             {
// CHECK-NEXT:                                 res = clad::pop(_t5);
// CHECK-NEXT:                                 double _r_d3 = _d_res;
// CHECK-NEXT:                                 *_d_i += _r_d3;
// CHECK-NEXT:                                 *_d_j += _r_d3;
// CHECK-NEXT:                             }
// CHECK-NEXT:                             if (clad::back(_cond0) != 0 && clad::back(_cond0) != 1 && clad::back(_cond0) != 2)
// CHECK-NEXT:                                 break;
// CHECK-NEXT:                         }
// CHECK-NEXT:                         {
// CHECK-NEXT:                             {
// CHECK-NEXT:                                 res = clad::pop(_t4);
// CHECK-NEXT:                                 double _r_d2 = _d_res;
// CHECK-NEXT:                                 *_d_j += _r_d2 * j;
// CHECK-NEXT:                                 *_d_j += j * _r_d2;
// CHECK-NEXT:                             }
// CHECK-NEXT:                             if (2 == clad::back(_cond0))
// CHECK-NEXT:                                 break;
// CHECK-NEXT:                         }
// CHECK-NEXT:                       case {{1U|1UL|1ULL}}:
// CHECK-NEXT:                         ;
// CHECK-NEXT:                         {
// CHECK-NEXT:                             {
// CHECK-NEXT:                                 {
// CHECK-NEXT:                                     res = clad::pop(_t2);
// CHECK-NEXT:                                     double _r_d1 = _d_res;
// CHECK-NEXT:                                     *_d_i += _r_d1 * i;
// CHECK-NEXT:                                     *_d_i += i * _r_d1;
// CHECK-NEXT:                                 }
// CHECK-NEXT:                             }
// CHECK-NEXT:                             if (1 == clad::back(_cond0))
// CHECK-NEXT:                                 break;
// CHECK-NEXT:                         }
// CHECK-NEXT:                         {
// CHECK-NEXT:                             {
// CHECK-NEXT:                                 res = clad::pop(_t1);
// CHECK-NEXT:                                 double _r_d0 = _d_res;
// CHECK-NEXT:                                 *_d_i += _r_d0 * j * j * i;
// CHECK-NEXT:                                 *_d_i += i * _r_d0 * j * j;
// CHECK-NEXT:                                 *_d_j += i * i * _r_d0 * j;
// CHECK-NEXT:                                 *_d_j += i * i * j * _r_d0;
// CHECK-NEXT:                             }
// CHECK-NEXT:                             if (0 == clad::back(_cond0))
// CHECK-NEXT:                                 break;
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
// CHECK-NEXT:                     clad::pop(_cond0);
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             _t0--;
// CHECK-NEXT:         }
// CHECK-NEXT: }

double fn4(double i, double j) {
  double res = 0;
  switch (1) {
    case 0: res += i * i * j * j; break;
    case 1:
      int counter = 2;
      while (counter--) {
        res += i * j;
      }
      break;
  }
  return res;
}

// CHECK: void fn4_grad(double i, double j, double *_d_i, double *_d_j) {
// CHECK-NEXT:     double _t0;
// CHECK-NEXT:     clad::tape<unsigned {{int|long|long long}}> _t1 = {};
// CHECK-NEXT:     int _d_counter = 0;
// CHECK-NEXT:     int counter = 0;
// CHECK-NEXT:     unsigned {{int|long|long long}} _t2;
// CHECK-NEXT:     clad::tape<double> _t3 = {};
// CHECK-NEXT:     double _d_res = 0.;
// CHECK-NEXT:     double res = 0;
// CHECK-NEXT:     {
// CHECK-NEXT:         switch (1) {
// CHECK-NEXT:             {
// CHECK-NEXT:               case 0:
// CHECK-NEXT:                 res += i * i * j * j;
// CHECK-NEXT:                 _t0 = res;
// CHECK-NEXT:             }
// CHECK-NEXT:             {
// CHECK-NEXT:                 clad::push(_t1, {{1U|1UL|1ULL}});
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:             {
// CHECK-NEXT:               case 1:
// CHECK-NEXT:                 counter = 2;
// CHECK-NEXT:             }
// CHECK-NEXT:             _t2 = {{0U|0UL|0ULL}};
// CHECK-NEXT:             while (counter--)
// CHECK-NEXT:                 {
// CHECK-NEXT:                     _t2++;
// CHECK-NEXT:                     clad::push(_t3, res);
// CHECK-NEXT:                     res += i * j;
// CHECK-NEXT:                 }
// CHECK-NEXT:             {
// CHECK-NEXT:                 clad::push(_t1, {{2U|2UL|2ULL}});
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:             clad::push(_t1, {{3U|3UL|3ULL}});
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     _d_res += 1;
// CHECK-NEXT:     {
// CHECK-NEXT:         switch (clad::pop(_t1)) {
// CHECK-NEXT:           case {{3U|3UL|3ULL}}:
// CHECK-NEXT:             ;
// CHECK-NEXT:           case {{2U|2UL|2ULL}}:
// CHECK-NEXT:             ;
// CHECK-NEXT:             while (_t2)
// CHECK-NEXT:                 {
// CHECK-NEXT:                     {
// CHECK-NEXT:                         {
// CHECK-NEXT:                             res = clad::pop(_t3);
// CHECK-NEXT:                             double _r_d1 = _d_res;
// CHECK-NEXT:                             *_d_i += _r_d1 * j;
// CHECK-NEXT:                             *_d_j += i * _r_d1;
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
// CHECK-NEXT:                     _t2--;
// CHECK-NEXT:                 }
// CHECK-NEXT:             {
// CHECK-NEXT:                 if (1 == 1)
// CHECK-NEXT:                     break;
// CHECK-NEXT:             }
// CHECK-NEXT:           case {{1U|1UL|1ULL}}:
// CHECK-NEXT:             ;
// CHECK-NEXT:             {
// CHECK-NEXT:                 {
// CHECK-NEXT:                     res = _t0;
// CHECK-NEXT:                     double _r_d0 = _d_res;
// CHECK-NEXT:                     *_d_i += _r_d0 * j * j * i;
// CHECK-NEXT:                     *_d_i += i * _r_d0 * j * j;
// CHECK-NEXT:                     *_d_j += i * i * _r_d0 * j;
// CHECK-NEXT:                     *_d_j += i * i * j * _r_d0;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if (0 == 1)
// CHECK-NEXT:                     break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }

double fn5(double i, double j) {
  double res=0;
  switch(int count = 1)
    case 1:
      res += i*j;
  return res;
}

// CHECK: void fn5_grad(double i, double j, double *_d_i, double *_d_j) {
// CHECK-NEXT:     int _d_count = 0;
// CHECK-NEXT:     int count = 0;
// CHECK-NEXT:     int _cond0;
// CHECK-NEXT:     double _t0;
// CHECK-NEXT:     clad::tape<unsigned {{int|long}}> _t1 = {};
// CHECK-NEXT:     double _d_res = 0.;
// CHECK-NEXT:     double res = 0;
// CHECK-NEXT:     {
// CHECK-NEXT:         count = 1;
// CHECK-NEXT:         _cond0 = count;
// CHECK-NEXT:         switch (_cond0) {
// CHECK-NEXT:           case 1:
// CHECK-NEXT:             res += i * j;
// CHECK-NEXT:             _t0 = res;
// CHECK-NEXT:             clad::push(_t1, {{1U|1UL|1ULL}});
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     _d_res += 1;
// CHECK-NEXT:     {
// CHECK-NEXT:         switch (clad::pop(_t1)) {
// CHECK-NEXT:           case {{1U|1UL|1ULL}}:
// CHECK-NEXT:             ;
// CHECK-NEXT:             {
// CHECK-NEXT:                 res = _t0;
// CHECK-NEXT:                 double _r_d0 = _d_res;
// CHECK-NEXT:                 *_d_i += _r_d0 * j;
// CHECK-NEXT:                 *_d_j += i * _r_d0;
// CHECK-NEXT:             }
// CHECK-NEXT:             if (1 == _cond0)
// CHECK-NEXT:                 break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }

double fn6(double u, double v) {
  int res = 0;
  double temp = 0;
  switch(res = u * v) {
    default:
      temp = 1;
  }
  return res;
}

// CHECK: void fn6_grad(double u, double v, double *_d_u, double *_d_v) {
// CHECK-NEXT:     int _t0;
// CHECK-NEXT:     int _cond0;
// CHECK-NEXT:     double _t1;
// CHECK-NEXT:     clad::tape<unsigned {{int|long|long long}}> _t2 = {};
// CHECK-NEXT:     int _d_res = 0;
// CHECK-NEXT:     int res = 0;
// CHECK-NEXT:     double _d_temp = 0.;
// CHECK-NEXT:     double temp = 0;
// CHECK-NEXT:     {
// CHECK-NEXT:         _t0 = res;
// CHECK-NEXT:         res = u * v;
// CHECK-NEXT:         _cond0 = res = u * v;
// CHECK-NEXT:         switch (_cond0) {
// CHECK-NEXT:             {
// CHECK-NEXT:               default:
// CHECK-NEXT:                 temp = 1;
// CHECK-NEXT:                 _t1 = temp;
// CHECK-NEXT:             }
// CHECK-NEXT:             clad::push(_t2, {{1U|1UL|1ULL}});
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     _d_res += 1;
// CHECK-NEXT:     {
// CHECK-NEXT:         switch (clad::pop(_t2)) {
// CHECK-NEXT:           case {{1U|1UL|1ULL}}:
// CHECK-NEXT:             ;
// CHECK-NEXT:             {
// CHECK-NEXT:                 {
// CHECK-NEXT:                     temp = _t1;
// CHECK-NEXT:                     double _r_d1 = _d_temp;
// CHECK-NEXT:                     _d_temp = 0.;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if (true)
// CHECK-NEXT:                     break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         {
// CHECK-NEXT:             res = _t0;
// CHECK-NEXT:             int _r_d0 = _d_res;
// CHECK-NEXT:             _d_res = 0;
// CHECK-NEXT:             *_d_u += _r_d0 * v;
// CHECK-NEXT:             *_d_v += u * _r_d0;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }

double fn7(double u, double v) {
    double res = 0;
    for (int i=0; i < 5; ++i) {
        switch(i) {
            case 0:
            case 1:
            case 2:
                res += u;
                break;
            case 3:
            default:
                res += v;
                break;
        }
    }
    return res;
}

// CHECK: void fn7_grad(double u, double v, double *_d_u, double *_d_v) {
// CHECK-NEXT:     int _d_i = 0;
// CHECK-NEXT:     int i = 0;
// CHECK-NEXT:     clad::tape<int> _cond0 = {};
// CHECK-NEXT:     clad::tape<double> _t1 = {};
// CHECK-NEXT:     clad::tape<unsigned {{int|long|long long}}> _t2 = {};
// CHECK-NEXT:     clad::tape<double> _t3 = {};
// CHECK-NEXT:     double _d_res = 0.;
// CHECK-NEXT:     double res = 0;
// CHECK-NEXT:     unsigned {{int|long|long long}} _t0 = {{0U|0UL|0ULL}};
// CHECK-NEXT:     for (i = 0; ; ++i) {
// CHECK-NEXT:     {
// CHECK-NEXT:          if (!(i < 5))
// CHECK-NEXT:          break;
// CHECK-NEXT:     }
// CHECK-NEXT:         _t0++;
// CHECK-NEXT:         {
// CHECK-NEXT:             clad::push(_cond0, i)
// CHECK-NEXT:             switch (clad::back(_cond0)) {
// CHECK-NEXT:                 {
// CHECK-NEXT:                   case 0:
// CHECK-NEXT:                     {
// CHECK-NEXT:                       case 1:
// CHECK-NEXT:                         {
// CHECK-NEXT:                           case 2:
// CHECK-NEXT:                             res += u;
// CHECK-NEXT:                             clad::push(_t1, res);
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 {
// CHECK-NEXT:                     clad::push(_t2, {{1U|1UL|1ULL}});
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 {
// CHECK-NEXT:                   case 3:
// CHECK-NEXT:                     {
// CHECK-NEXT:                       default:
// CHECK-NEXT:                         res += v;
// CHECK-NEXT:                         clad::push(_t3, res);
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 {
// CHECK-NEXT:                     clad::push(_t2, {{2U|2UL|2ULL}});
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 clad::push(_t2, {{3U|3UL|3ULL}});
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     _d_res += 1;
// CHECK-NEXT:     for (;; _t0--) {
// CHECK-NEXT:         {
// CHECK-NEXT:             if (!_t0)
// CHECK-NEXT:                 break;
// CHECK-NEXT:         }
// CHECK-NEXT:         --i;
// CHECK-NEXT:         {
// CHECK-NEXT:             switch (clad::pop(_t2)) {
// CHECK-NEXT:               case {{3U|3UL|3ULL}}:
// CHECK-NEXT:                 ;
// CHECK-NEXT:               case {{2U|2UL|2ULL}}:
// CHECK-NEXT:                 ;
// CHECK-NEXT:                 {
// CHECK-NEXT:                     {
// CHECK-NEXT:                         {
// CHECK-NEXT:                             res = clad::pop(_t3);
// CHECK-NEXT:                             double _r_d1 = _d_res;
// CHECK-NEXT:                             *_d_v += _r_d1;
// CHECK-NEXT:                         }
// CHECK-NEXT:                         if (clad::back(_cond0) != 0 && clad::back(_cond0) != 1 && clad::back(_cond0) != 2 && clad::back(_cond0) != 3)
// CHECK-NEXT:                             break;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     if (3 == clad::back(_cond0))
// CHECK-NEXT:                         break;
// CHECK-NEXT:                 }
// CHECK-NEXT:               case {{1U|1UL|1ULL}}:
// CHECK-NEXT:                 ;
// CHECK-NEXT:                 {
// CHECK-NEXT:                     {
// CHECK-NEXT:                         {
// CHECK-NEXT:                             {
// CHECK-NEXT:                                 res = clad::pop(_t1);
// CHECK-NEXT:                                 double _r_d0 = _d_res;
// CHECK-NEXT:                                 *_d_u += _r_d0;
// CHECK-NEXT:                             }
// CHECK-NEXT:                             if (2 == clad::back(_cond0))
// CHECK-NEXT:                                 break;
// CHECK-NEXT:                         }
// CHECK-NEXT:                         if (1 == clad::back(_cond0))
// CHECK-NEXT:                             break;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     if (0 == clad::back(_cond0))
// CHECK-NEXT:                         break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             clad::pop(_cond0);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }

enum Op {
  Add,
  Sub,
  Mul,
  Div
};

double fn24(double x, double y, Op op) {
  double res = 0;
  switch (op) {
    case Add:
      res = x + y;
      break;
    case Sub:
      res = x - y;
      break;
    case Mul:
      res = x * y;
      break;
    case Div:
      res = x / y;
      break;
  }
  return res;
}

// CHECK: void fn24_grad_0_1(double x, double y, Op op, double *_d_x, double *_d_y) {
// CHECK-NEXT:    Op _d_op = static_cast<Op>(0U);
// CHECK-NEXT:    Op _cond0;
// CHECK-NEXT:    double _t0;
// CHECK-NEXT:    clad::tape<unsigned {{int|long}}> _t1 = {};
// CHECK-NEXT:    double _t2;
// CHECK-NEXT:    double _t3;
// CHECK-NEXT:    double _t4;
// CHECK-NEXT:    double _d_res = 0.;
// CHECK-NEXT:    double res = 0;
// CHECK-NEXT:    {
// CHECK-NEXT:        _cond0 = op;
// CHECK-NEXT:        switch (_cond0) {
// CHECK-NEXT:            {
// CHECK-NEXT:              case Add:
// CHECK-NEXT:                res = x + y;
// CHECK-NEXT:                _t0 = res;
// CHECK-NEXT:            }
// CHECK-NEXT:            {
// CHECK-NEXT:                clad::push(_t1, {{1U|1UL}});
// CHECK-NEXT:               break;
// CHECK-NEXT:            }
// CHECK-NEXT:            {
// CHECK-NEXT:              case Sub:
// CHECK-NEXT:                res = x - y;
// CHECK-NEXT:                _t2 = res;
// CHECK-NEXT:            }
// CHECK-NEXT:            {
// CHECK-NEXT:                clad::push(_t1, {{2U|2UL}});
// CHECK-NEXT:                break;
// CHECK-NEXT:            }
// CHECK-NEXT:            {
// CHECK-NEXT:              case Mul:
// CHECK-NEXT:                res = x * y;
// CHECK-NEXT:                _t3 = res;
// CHECK-NEXT:            }
// CHECK-NEXT:            {
// CHECK-NEXT:                clad::push(_t1, {{3U|3UL}});
// CHECK-NEXT:                break;
// CHECK-NEXT:            }
// CHECK-NEXT:            {
// CHECK-NEXT:              case Div:
// CHECK-NEXT:                res = x / y;
// CHECK-NEXT:                _t4 = res;
// CHECK-NEXT:            }
// CHECK-NEXT:            {
// CHECK-NEXT:                clad::push(_t1, {{4U|4UL}});
// CHECK-NEXT:                break;
// CHECK-NEXT:            }
// CHECK-NEXT:            clad::push(_t1, {{5U|5UL}});
// CHECK-NEXT:        }
// CHECK-NEXT:    }
// CHECK-NEXT:    _d_res += 1;
// CHECK-NEXT:    {
// CHECK-NEXT:        switch (clad::pop(_t1)) {
// CHECK-NEXT:          case {{5U|5UL}}:
// CHECK-NEXT:            ;
// CHECK-NEXT:          case {{4U|4UL}}:
// CHECK-NEXT:            ;
// CHECK-NEXT:            {
// CHECK-NEXT:                {
// CHECK-NEXT:                    res = _t4;
// CHECK-NEXT:                    double _r_d3 = _d_res;
// CHECK-NEXT:                    _d_res = 0.;
// CHECK-NEXT:                    *_d_x += _r_d3 / y;
// CHECK-NEXT:                    double _r0 = _r_d3 * -(x / (y * y));
// CHECK-NEXT:                    _d_y += _r0;
// CHECK-NEXT:                }
// CHECK-NEXT:                if (Div == _cond0)
// CHECK-NEXT:                    break;
// CHECK-NEXT:            }
// CHECK-NEXT:          case {{3U|3UL}}:
// CHECK-NEXT:            ;
// CHECK-NEXT:            {
// CHECK-NEXT:                {
// CHECK-NEXT:                    res = _t3;
// CHECK-NEXT:                    double _r_d2 = _d_res;
// CHECK-NEXT:                    _d_res = 0.;
// CHECK-NEXT:                    *_d_x += _r_d2 * y;
// CHECK-NEXT:                    _d_y += x * _r_d2;
// CHECK-NEXT:                }
// CHECK-NEXT:                if (Mul == _cond0)
// CHECK-NEXT:                    break;
// CHECK-NEXT:            }
// CHECK-NEXT:          case {{2U|2UL}}:
// CHECK-NEXT:            ;
// CHECK-NEXT:            {
// CHECK-NEXT:                {
// CHECK-NEXT:                    res = _t2;
// CHECK-NEXT:                    double _r_d1 = _d_res;
// CHECK-NEXT:                    _d_res = 0.;
// CHECK-NEXT:                    *_d_x += _r_d1;
// CHECK-NEXT:                    _d_y += -_r_d1;
// CHECK-NEXT:                }
// CHECK-NEXT:                if (Sub == _cond0)
// CHECK-NEXT:                    break;
// CHECK-NEXT:            }
// CHECK-NEXT:          case {{1U|1UL}}:
// CHECK-NEXT:            ;
// CHECK-NEXT:            {
// CHECK-NEXT:                {
// CHECK-NEXT:                    res = _t0;
// CHECK-NEXT:                    double _r_d0 = _d_res;
// CHECK-NEXT:                    _d_res = 0.;
// CHECK-NEXT:                    *_d_x += _r_d0;
// CHECK-NEXT:                    _d_y += _r_d0;
// CHECK-NEXT:                }
// CHECK-NEXT:                if (Add == _cond0)
// CHECK-NEXT:                    break;
// CHECK-NEXT:            }
// CHECK-NEXT:        }
// CHECK-NEXT:    }
// CHECK-NEXT:}


#define TEST_2(F, x, y)                                                        \
  {                                                                            \
    result[0] = result[1] = 0;                                                 \
    auto d_##F = clad::gradient(F);                                            \
    d_##F.execute(x, y, result, result + 1);                                   \
    printf("{%.2f, %.2f}\n", result[0], result[1]);                            \
  }

#define TEST_2_Op(F, x, y, op)                                                 \
{                                                                              \
  result[0] = result[1] = 0;                                                   \
  auto d_##F = clad::gradient(F, "x, y");                                      \
  d_##F.execute(x, y, op, result, result + 1);                                 \
  printf("{%.2f, %.2f}\n", result[0], result[1]);                              \
}

int main() {
  double result[2] = {};

  TEST_2(fn1, 3, 5);  // CHECK-EXEC: {156.00, 100.00}
  TEST_2(fn2, 3, 5);  // CHECK-EXEC: {5.00, 3.00}
  TEST_2(fn3, 3, 5);  // CHECK-EXEC: {162.00, 90.00}
  TEST_2(fn4, 3, 5);  // CHECK-EXEC: {10.00, 6.00}
  TEST_2(fn5, 3, 5);  // CHECK-EXEC: {5.00, 3.00}

  INIT_GRADIENT(fn6);
  INIT_GRADIENT(fn7);

  TEST_GRADIENT(fn6, 2, 3, 5, &result[0], &result[1]); // CHECK-EXEC: {5.00, 3.00}
  TEST_GRADIENT(fn7, 2, 3, 5, &result[0], &result[1]); // CHECK-EXEC: {3.00, 2.00}

  TEST_2_Op(fn24, 3, 5, Add);  // CHECK-EXEC: {1.00, 1.00}
  TEST_2_Op(fn24, 3, 5, Sub);  // CHECK-EXEC: {1.00, -1.00}
  TEST_2_Op(fn24, 3, 5, Mul);  // CHECK-EXEC: {5.00, 3.00}
  TEST_2_Op(fn24, 3, 5, Div);  // CHECK-EXEC: {0.20, -0.12}
}
