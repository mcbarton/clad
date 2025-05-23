// RUN: %cladclang -Xclang -plugin-arg-clad -Xclang -disable-tbr %s -I%S/../../include -oPointers.out 2>&1 | %filecheck %s
// RUN: ./Pointers.out | %filecheck_exec %s
// RUN: %cladclang -Xclang -plugin-arg-clad -Xclang -enable-tbr %s -I%S/../../include -oPointers.out
// RUN: ./Pointers.out | %filecheck_exec %s

#include "clad/Differentiator/Differentiator.h"

double nonMemFn(double i, double j) {
  return i*j;
}

// CHECK: double nonMemFn_darg0(double i, double j);
// CHECK: void nonMemFn_darg0_grad(double i, double j, double *_d_i, double *_d_j);
// CHECK: double nonMemFn_darg1(double i, double j);
// CHECK: void nonMemFn_darg1_grad(double i, double j, double *_d_i, double *_d_j);

// CHECK: void nonMemFn_hessian(double i, double j, double *hessianMatrix) {
// CHECK-NEXT:     nonMemFn_darg0_grad(i, j, hessianMatrix + {{0U|0UL|0ULL}}, hessianMatrix + {{1U|1UL|1ULL}});
// CHECK-NEXT:     nonMemFn_darg1_grad(i, j, hessianMatrix + {{2U|2UL|2ULL}}, hessianMatrix + {{3U|3UL|3ULL}});
// CHECK-NEXT: }

// CHECK: double nonMemFn_darg0(double i, double j) {
// CHECK-NEXT:     double _d_i = 1;
// CHECK-NEXT:     double _d_j = 0;
// CHECK-NEXT:     return _d_i * j + i * _d_j;
// CHECK-NEXT: }

// CHECK: void nonMemFn_darg0_grad(double i, double j, double *_d_i, double *_d_j) {
// CHECK-NEXT:     double _d__d_i = 0.;
// CHECK-NEXT:     double _d_i0 = 1;
// CHECK-NEXT:     double _d__d_j = 0.;
// CHECK-NEXT:     double _d_j0 = 0;
// CHECK-NEXT:     {
// CHECK-NEXT:         _d__d_i += 1 * j;
// CHECK-NEXT:         *_d_j += _d_i0 * 1;
// CHECK-NEXT:         *_d_i += 1 * _d_j0;
// CHECK-NEXT:         _d__d_j += i * 1;
// CHECK-NEXT:     }
// CHECK-NEXT: }

// CHECK: double nonMemFn_darg1(double i, double j) {
// CHECK-NEXT:     double _d_i = 0;
// CHECK-NEXT:     double _d_j = 1;
// CHECK-NEXT:     return _d_i * j + i * _d_j;
// CHECK-NEXT: }

// CHECK: void nonMemFn_darg1_grad(double i, double j, double *_d_i, double *_d_j) {
// CHECK-NEXT:     double _d__d_i = 0.;
// CHECK-NEXT:     double _d_i0 = 0;
// CHECK-NEXT:     double _d__d_j = 0.;
// CHECK-NEXT:     double _d_j0 = 1;
// CHECK-NEXT:     {
// CHECK-NEXT:         _d__d_i += 1 * j;
// CHECK-NEXT:         *_d_j += _d_i0 * 1;
// CHECK-NEXT:         *_d_i += 1 * _d_j0;
// CHECK-NEXT:         _d__d_j += i * 1;
// CHECK-NEXT:     }
// CHECK-NEXT: }

#define NON_MEM_FN_TEST(var)\
res[0]=res[1]=res[2]=res[3]=0;\
var.execute(3, 4, res);\
printf("{%.2f %.2f %.2f %.2f}\n", res[0], res[1], res[2], res[3]);

int main() {
  auto nonMemFnPtr = &nonMemFn;
  auto nonMemFnPtrToPtr = &nonMemFnPtr;
  auto nonMemFnPtrToPtrToPtr = &nonMemFnPtrToPtr;
  auto nonMemFnIndirectPtr = nonMemFnPtr;
  auto nonMemFnIndirectIndirectPtr = nonMemFnIndirectPtr;

  double res[4];

  auto d_nonMemFn = clad::hessian(nonMemFn);
  auto d_nonMemFnPar = clad::hessian((nonMemFn));
  auto d_nonMemFnPtr = clad::hessian(nonMemFnPtr);
  auto d_nonMemFnPtrToPtr = clad::hessian(*nonMemFnPtrToPtr);
  auto d_nonMemFnPtrToPtrPar = clad::hessian((*(nonMemFnPtrToPtr)));
  auto d_nonMemFnPtrToPtr_1 = clad::hessian(**&nonMemFnPtrToPtr);
  auto d_nonMemFnPtrToPtr_1Par = clad::hessian(**(&nonMemFnPtrToPtr));
  auto d_nonMemFnPtrToPtr_1ParPar = clad::hessian(*(*(&nonMemFnPtrToPtr)));
  auto d_nonMemFnPtrToPtrToPtr = clad::hessian(**nonMemFnPtrToPtrToPtr);
  auto d_nonMemFnPtrToPtrToPtr_1 = clad::hessian(***&nonMemFnPtrToPtrToPtr);
  auto d_nonMemFnPtrToPtrToPtr_1Par = clad::hessian(***(&nonMemFnPtrToPtrToPtr));
  auto d_nonMemFnPtrToPtrToPtr_1ParPar = clad::hessian(*(**(&nonMemFnPtrToPtrToPtr)));
  auto d_nonMemFnPtrToPtrToPtr_1ParParPar = clad::hessian((*(**((&nonMemFnPtrToPtrToPtr)))));
  auto d_nonMemFnIndirectPtr = clad::hessian(nonMemFnIndirectPtr);
  auto d_nonMemFnIndirectIndirectPtr = clad::hessian(nonMemFnIndirectIndirectPtr);
  auto d_nonMemFnStaticCast = clad::hessian(static_cast<decltype(&nonMemFn)>(nonMemFn));
  auto d_nonMemFnReinterpretCast = clad::hessian(reinterpret_cast<decltype(&nonMemFn)>(nonMemFn));
  auto d_nonMemFnCStyleCast = clad::hessian((decltype(&nonMemFn))(nonMemFn));

  NON_MEM_FN_TEST(d_nonMemFn); // CHECK-EXEC: {0.00 1.00 1.00 0.00}

  NON_MEM_FN_TEST(d_nonMemFnPar); // CHECK-EXEC: {0.00 1.00 1.00 0.00}

  NON_MEM_FN_TEST(d_nonMemFnPtr); // CHECK-EXEC: {0.00 1.00 1.00 0.00}

  NON_MEM_FN_TEST(d_nonMemFnPtrToPtr); // CHECK-EXEC: {0.00 1.00 1.00 0.00}

  NON_MEM_FN_TEST(d_nonMemFnPtrToPtrPar); // CHECK-EXEC: {0.00 1.00 1.00 0.00}

  NON_MEM_FN_TEST(d_nonMemFnPtrToPtr_1); // CHECK-EXEC: {0.00 1.00 1.00 0.00}

  NON_MEM_FN_TEST(d_nonMemFnPtrToPtr_1Par); // CHECK-EXEC: {0.00 1.00 1.00 0.00}

  NON_MEM_FN_TEST(d_nonMemFnPtrToPtr_1ParPar); // CHECK-EXEC: {0.00 1.00 1.00 0.00}

  NON_MEM_FN_TEST(d_nonMemFnPtrToPtrToPtr); // CHECK-EXEC: {0.00 1.00 1.00 0.00}

  NON_MEM_FN_TEST(d_nonMemFnPtrToPtrToPtr_1); // CHECK-EXEC: {0.00 1.00 1.00 0.00}

  NON_MEM_FN_TEST(d_nonMemFnPtrToPtrToPtr_1Par); // CHECK-EXEC: {0.00 1.00 1.00 0.00}

  NON_MEM_FN_TEST(d_nonMemFnPtrToPtrToPtr_1ParPar); // CHECK-EXEC: {0.00 1.00 1.00 0.00}

  NON_MEM_FN_TEST(d_nonMemFnPtrToPtrToPtr_1ParParPar); // CHECK-EXEC: {0.00 1.00 1.00 0.00}

  NON_MEM_FN_TEST(d_nonMemFnIndirectPtr); // CHECK-EXEC: {0.00 1.00 1.00 0.00}

  NON_MEM_FN_TEST(d_nonMemFnIndirectIndirectPtr); // CHECK-EXEC: {0.00 1.00 1.00 0.00}

  NON_MEM_FN_TEST(d_nonMemFnStaticCast); // CHECK-EXEC: {0.00 1.00 1.00 0.00}

  NON_MEM_FN_TEST(d_nonMemFnReinterpretCast); // CHECK-EXEC: {0.00 1.00 1.00 0.00}

  NON_MEM_FN_TEST(d_nonMemFnCStyleCast); // CHECK-EXEC: {0.00 1.00 1.00 0.00}

}
