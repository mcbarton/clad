//--------------------------------------------------------------------*- C++ -*-
// clad - the C++ Clang-based Automatic Differentiator
// version: $Id: ClangPlugin.cpp 7 2013-06-01 22:48:03Z v.g.vassilev@gmail.com $
// author:  Vassil Vassilev <vvasilev-at-cern.ch>
//------------------------------------------------------------------------------

#ifndef CLAD_HESSIAN_MODE_VISITOR_H
#define CLAD_HESSIAN_MODE_VISITOR_H

#include "Compatibility.h"
#include "VisitorBase.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/StmtVisitor.h"
#include "clang/Sema/Sema.h"

#include <array>
#include <stack>
#include <unordered_map>

namespace clad {
  /// A visitor for processing the function code to generate hessians
  /// Used to compute Hessian matrices by clad::hessian.
  class HessianModeVisitor
      : public clang::ConstStmtVisitor<HessianModeVisitor, StmtDiff>,
        public VisitorBase {
  private:
    /// A helper method that combines all the generated second derivatives
    /// (contained within a vector) obtained from Derive
    /// into a single FunctionDecl f_hessian
    DerivativeAndOverload
    Merge(std::vector<clang::FunctionDecl*> secDerivFuncs,
          llvm::SmallVector<size_t, 16> IndependentArgsSize,
          size_t TotalIndependentArgsSize, const std::string& hessianFuncName,
          clang::DeclContext* FD, clang::QualType hessianFuncType);

  public:
    HessianModeVisitor(DerivativeBuilder& builder, const DiffRequest& request);
    ~HessianModeVisitor() = default;

    ///\brief Produces the hessian second derivative columns of a given
    /// function.
    ///
    ///\returns A function containing second derivatives (columns) of a hessian
    /// matrix and potentially created enclosing context.
    ///
    /// We name the hessian of f as 'f_hessian'. Uses ForwardModeVisitor and
    /// ReverseModeVisitor to generate second derivatives that correspond to
    /// columns of the Hessian. uses Merge to return a FunctionDecl
    /// containing CallExprs to the generated second derivatives.
    DerivativeAndOverload Derive() override;
  };
} // end namespace clad

#endif // CLAD_HESSIAN_MODE_VISITOR_H
