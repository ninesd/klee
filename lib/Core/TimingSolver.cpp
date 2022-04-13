//===-- TimingSolver.cpp --------------------------------------------------===//
//
//                     The KLEE Symbolic Virtual Machine
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "TimingSolver.h"

#include "klee/CommandLine.h"
#include "klee/Config/Version.h"
#include "klee/ExecutionState.h"
#include "klee/Solver.h"
#include "klee/Statistics.h"
#include "klee/Internal/System/Time.h"

#include "CoreStats.h"

#include "llvm/Support/TimeValue.h"

using namespace klee;
using namespace llvm;

/***/

bool TimingSolver::evaluate(const ExecutionState &state, ref<Expr> expr,
                            Solver::Validity &result,
                            std::vector<ref<Expr> > &unsatCore) {
  // Fast path, to avoid timer and OS overhead.
  if (ConstantExpr *CE = dyn_cast<ConstantExpr>(expr)) {
    result = CE->isTrue() ? Solver::True : Solver::False;
    return true;
  }

  sys::TimeValue now = util::getWallTimeVal();

  std::vector<ref<Expr> > simplificationCore;
  if (simplifyExprs)
    expr = state.constraints.simplifyExpr(expr, simplificationCore);

  unsatCore.clear();

  bool success =
      solver->evaluate(Query(state.constraints, expr), result, unsatCore);
  if (INTERPOLATION_ENABLED) {
    if (result != Solver::Unknown) {
      if (simplifyExprs) {
        unsatCore.insert(unsatCore.begin(), simplificationCore.begin(),
                         simplificationCore.end());
      }
    }
  }

  sys::TimeValue delta = util::getWallTimeVal();
  delta -= now;
  stats::solverTime += delta.usec();
  state.queryCost += delta.usec()/1000000.;

  return success;
}

bool TimingSolver::mustBeTrue(const ExecutionState &state, ref<Expr> expr,
                              bool &result,
                              std::vector<ref<Expr> > &unsatCore) {
  // Fast path, to avoid timer and OS overhead.
  if (ConstantExpr *CE = dyn_cast<ConstantExpr>(expr)) {
    result = CE->isTrue() ? true : false;
    return true;
  }

  sys::TimeValue now = util::getWallTimeVal();

  std::vector<ref<Expr> > simplificationCore;
  if (simplifyExprs)
    expr = state.constraints.simplifyExpr(expr, simplificationCore);

  bool success =
      solver->mustBeTrue(Query(state.constraints, expr), result, unsatCore);

  if (INTERPOLATION_ENABLED && simplifyExprs) {
    unsatCore.insert(unsatCore.begin(), simplificationCore.begin(),
                     simplificationCore.end());
  }

  sys::TimeValue delta = util::getWallTimeVal();
  delta -= now;
  stats::solverTime += delta.usec();
  state.queryCost += delta.usec()/1000000.;

  return success;
}

bool TimingSolver::mustBeFalse(const ExecutionState &state, ref<Expr> expr,
                               bool &result,
                               std::vector<ref<Expr> > &unsatCore) {
  return mustBeTrue(state, Expr::createIsZero(expr), result, unsatCore);
}

bool TimingSolver::mayBeTrue(const ExecutionState &state, ref<Expr> expr,
                             bool &result, std::vector<ref<Expr> > &unsatCore) {
  bool res;
  if (!mustBeFalse(state, expr, res, unsatCore))
    return false;
  result = !res;
  return true;
}

bool TimingSolver::mayBeFalse(const ExecutionState &state, ref<Expr> expr,
                              bool &result,
                              std::vector<ref<Expr> > &unsatCore) {
  bool res;
  if (!mustBeTrue(state, expr, res, unsatCore))
    return false;
  result = !res;
  return true;
}

bool TimingSolver::getValue(const ExecutionState& state, ref<Expr> expr, 
                            ref<ConstantExpr> &result) {
  // Fast path, to avoid timer and OS overhead.
  if (ConstantExpr *CE = dyn_cast<ConstantExpr>(expr)) {
    result = CE;
    return true;
  }
  
  sys::TimeValue now = util::getWallTimeVal();

  std::vector<ref<Expr> > simplificationCore;
  if (simplifyExprs)
    expr = state.constraints.simplifyExpr(expr, simplificationCore);

  bool success = solver->getValue(Query(state.constraints, expr), result);

  sys::TimeValue delta = util::getWallTimeVal();
  delta -= now;
  stats::solverTime += delta.usec();
  state.queryCost += delta.usec()/1000000.;

  return success;
}

bool
TimingSolver::getInitialValues(const ExecutionState &state,
                               const std::vector<const Array *> &objects,
                               std::vector<std::vector<unsigned char> > &result,
                               std::vector<ref<Expr> > &unsatCore) {
  if (objects.empty())
    return true;

  sys::TimeValue now = util::getWallTimeVal();

  bool success = solver->getInitialValues(
      Query(state.constraints, ConstantExpr::alloc(0, Expr::Bool)), objects,
      result, unsatCore);

  sys::TimeValue delta = util::getWallTimeVal();
  delta -= now;
  stats::solverTime += delta.usec();
  state.queryCost += delta.usec()/1000000.;
  
  return success;
}

std::pair< ref<Expr>, ref<Expr> >
TimingSolver::getRange(const ExecutionState& state, ref<Expr> expr) {
  std::pair<ref<Expr>, ref<Expr> > ret =
      solver->getRange(Query(state.constraints, expr));
  return ret;
}
