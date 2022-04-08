//===-- TxShadowArray.cpp ---------------------------------------*- C++ -*-===//
//
//               The Tracer-X KLEE Symbolic Virtual Machine
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// This file contains the implementations for the shadow array, which
/// replaces KLEE arrays with their shadow counterparts as
/// existentially-quantified variables in the interpolant.
///
//===----------------------------------------------------------------------===//

#include "TxShadowArray.h"
#include "klee/Expr/Expr.h"

#define roundingMode llvm::APFloat::rmNearestTiesToEven

using namespace klee;

namespace klee {

std::map<const Array *, const Array *> TxShadowArray::shadowArray;

UpdateNode *
TxShadowArray::getShadowUpdate(const ref<UpdateNode> &source,
                             std::set<const Array *> &replacements) {
  if (!source)
    return 0;

  return new UpdateNode(getShadowUpdate(source->next, replacements),
                        getShadowExpression(source->index, replacements),
                        getShadowExpression(source->value, replacements));
}

ref<Expr> TxShadowArray::createBinaryOfSameKind(ref<Expr> originalExpr,
                                              ref<Expr> newLhs,
                                              ref<Expr> newRhs) {
  std::vector<Expr::CreateArg> exprs;
  Expr::CreateArg arg1(newLhs);
  Expr::CreateArg arg2(newRhs);
  exprs.push_back(arg1);
  exprs.push_back(arg2);
  return Expr::createFromKind(originalExpr->getKind(), exprs);
}

void TxShadowArray::addShadowArrayMap(const Array *source, const Array *target) {
  shadowArray[source] = target;
}

ref<Expr>
TxShadowArray::getShadowExpression(ref<Expr> expr,
                                 std::set<const Array *> &replacements) {
  ref<Expr> ret;

  switch (expr->getKind()) {
  case Expr::Read: {
    ReadExpr *readExpr = llvm::dyn_cast<ReadExpr>(expr);
    const Array *replacementArray = shadowArray[readExpr->updates.root];

    if (std::find(replacements.begin(), replacements.end(), replacementArray) ==
        replacements.end()) {
      replacements.insert(replacementArray);
    }

    UpdateList newUpdates(
        replacementArray,
        getShadowUpdate(readExpr->updates.head, replacements));
    ret = ReadExpr::create(newUpdates,
                           getShadowExpression(readExpr->index, replacements));
    break;
  }
  case Expr::Constant: {
    ret = expr;
    break;
  }
  case Expr::Select: {
    ret =
        SelectExpr::create(getShadowExpression(expr->getKid(0), replacements),
                           getShadowExpression(expr->getKid(1), replacements),
                           getShadowExpression(expr->getKid(2), replacements));
    break;
  }
  case Expr::Extract: {
    ExtractExpr *extractExpr = llvm::dyn_cast<ExtractExpr>(expr);
    ret =
        ExtractExpr::create(getShadowExpression(expr->getKid(0), replacements),
                            extractExpr->offset, extractExpr->width);
    break;
  }
  case Expr::ZExt: {
    CastExpr *castExpr = llvm::dyn_cast<CastExpr>(expr);
    ret = ZExtExpr::create(getShadowExpression(expr->getKid(0), replacements),
                           castExpr->getWidth());
    break;
  }
  case Expr::SExt: {
    CastExpr *castExpr = llvm::dyn_cast<CastExpr>(expr);
    ret = SExtExpr::create(getShadowExpression(expr->getKid(0), replacements),
                           castExpr->getWidth());
    break;
  }
  case Expr::FPExt: {
    CastExpr *castExpr = llvm::dyn_cast<CastExpr>(expr);
    ret = FPExtExpr::create(getShadowExpression(expr->getKid(0), replacements),
                           castExpr->getWidth());
    break;
  }
  case Expr::FPTrunc: {
    CastExpr *castExpr = llvm::dyn_cast<CastExpr>(expr);
    ret = FPTrunc::create(getShadowExpression(expr->getKid(0), replacements),
                            castExpr->getWidth(), roundingMode);
    break;
  }
  case Expr::FPToUI: {
    CastExpr *castExpr = llvm::dyn_cast<CastExpr>(expr);
    ret = FPToUI::create(getShadowExpression(expr->getKid(0), replacements),
                          castExpr->getWidth(), roundingMode);
    break;
  }
  case Expr::FPToSI: {
    CastExpr *castExpr = llvm::dyn_cast<CastExpr>(expr);
    ret = FPToSI::create(getShadowExpression(expr->getKid(0), replacements),
                          castExpr->getWidth(), roundingMode);
    break;
  }
  case Expr::UIToFP: {
    CastExpr *castExpr = llvm::dyn_cast<CastExpr>(expr);
    ret = UIToFP::create(getShadowExpression(expr->getKid(0), replacements),
                          castExpr->getWidth(), roundingMode);
    break;
  }
  case Expr::SIToFP: {
    CastExpr *castExpr = llvm::dyn_cast<CastExpr>(expr);
    ret = SIToFP::create(getShadowExpression(expr->getKid(0), replacements),
                         castExpr->getWidth(), roundingMode);
    break;
  }
  case Expr::FSqrt: {
    CastExpr *castExpr = llvm::dyn_cast<CastExpr>(expr);
    ret = FSqrt::create(getShadowExpression(expr->getKid(0), replacements),
                         roundingMode);
    break;
  }
  case Expr::FAbs: {
    CastExpr *castExpr = llvm::dyn_cast<CastExpr>(expr);
    ret = FAbs::create(getShadowExpression(expr->getKid(0), replacements));
    break;
  }
  case Expr::FNeg: {
    CastExpr *castExpr = llvm::dyn_cast<CastExpr>(expr);
    ret = FNeg::create(getShadowExpression(expr->getKid(0), replacements));
    break;
  }
  case Expr::FRint: {
    CastExpr *castExpr = llvm::dyn_cast<CastExpr>(expr);
    ret = FRint::create(getShadowExpression(expr->getKid(0), replacements),
                         roundingMode);
    break;
  }
  case Expr::IsNaNExpr: {
    CastExpr *castExpr = llvm::dyn_cast<CastExpr>(expr);
    ret = IsNaNExpr::create(getShadowExpression(expr->getKid(0), replacements));
    break;
  }
  case Expr::IsInfiniteExpr: {
    CastExpr *castExpr = llvm::dyn_cast<CastExpr>(expr);
    ret = IsInfiniteExpr::create(getShadowExpression(expr->getKid(0), replacements));
    break;
  }
  case Expr::IsNormalExpr: {
    CastExpr *castExpr = llvm::dyn_cast<CastExpr>(expr);
    ret = IsNormalExpr::create(getShadowExpression(expr->getKid(0), replacements));
    break;
  }
  case Expr::IsSubnormalExpr: {
    CastExpr *castExpr = llvm::dyn_cast<CastExpr>(expr);
    ret = IsSubnormalExpr::create(getShadowExpression(expr->getKid(0), replacements));
    break;
  }
  case Expr::FAdd:
  case Expr::FSub:
  case Expr::FMul:
  case Expr::FDiv:
  case Expr::FRem:
  case Expr::FMax:
  case Expr::FMin:
  case Expr::FOEq:
  case Expr::FOLt:
  case Expr::FOLe:
  case Expr::FOGt:
  case Expr::FOGe:
  case Expr::Concat:
  case Expr::Add:
  case Expr::Sub:
  case Expr::Mul:
  case Expr::UDiv:
  case Expr::SDiv:
  case Expr::URem:
  case Expr::SRem:
  case Expr::Not:
  case Expr::And:
  case Expr::Or:
  case Expr::Xor:
  case Expr::Shl:
  case Expr::LShr:
  case Expr::AShr:
  case Expr::Eq:
  case Expr::Ne:
  case Expr::Ult:
  case Expr::Ule:
  case Expr::Ugt:
  case Expr::Uge:
  case Expr::Slt:
  case Expr::Sle:
  case Expr::Sgt:
  case Expr::Sge: {
    ret = createBinaryOfSameKind(
        expr, getShadowExpression(expr->getKid(0), replacements),
        getShadowExpression(expr->getKid(1), replacements));
    break;
  }
  case Expr::NotOptimized: {
    ret = NotOptimizedExpr::create(getShadowExpression(expr->getKid(0), replacements));
    break;
  }
  default:
    assert(!"unhandled Expr type");
  }

  return ret;
}

}
