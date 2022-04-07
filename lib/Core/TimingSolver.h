//===-- TimingSolver.h ------------------------------------------*- C++ -*-===//
//
//                     The KLEE Symbolic Virtual Machine
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef KLEE_TIMINGSOLVER_H
#define KLEE_TIMINGSOLVER_H

#include "klee/Expr/Constraints.h"
#include "klee/Expr/Expr.h"
#include "klee/Solver/Solver.h"
#include "klee/System/Time.h"

#include <memory>
#include <vector>

namespace klee {
class ConstraintSet;
class Solver;

/// TimingSolver - A simple class which wraps a solver and handles
/// tracking the statistics that we care about.
class TimingSolver {
public:
  std::unique_ptr<Solver> solver;
  bool simplifyExprs;

public:
  /// TimingSolver - Construct a new timing solver.
  ///
  /// \param _simplifyExprs - Whether expressions should be
  /// simplified (via the constraint manager interface) prior to
  /// querying.
  TimingSolver(Solver *_solver, bool _simplifyExprs = true)
      : solver(_solver), simplifyExprs(_simplifyExprs) {}

  void setTimeout(time::Span t) { solver->setCoreSolverTimeout(t); }

  char *getConstraintLog(const Query &query) {
    return solver->getConstraintLog(query);
  }

  bool evaluate(const ConstraintSet &, ref<Expr>, Solver::Validity &result,
                SolverQueryMetaData &metaData,
                std::vector<ref<Expr> > &unsatCore);

  bool mustBeTrue(const ConstraintSet &, ref<Expr>, bool &result,
                  SolverQueryMetaData &metaData,
                  std::vector<ref<Expr> > &unsatCore);

  bool mustBeFalse(const ConstraintSet &, ref<Expr>, bool &result,
                   SolverQueryMetaData &metaData,
                   std::vector<ref<Expr> > &unsatCore);

  bool mayBeTrue(const ConstraintSet &, ref<Expr>, bool &result,
                 SolverQueryMetaData &metaData,
                 std::vector<ref<Expr> > &unsatCore);

  bool mayBeFalse(const ConstraintSet &, ref<Expr>, bool &result,
                  SolverQueryMetaData &metaData,
                  std::vector<ref<Expr> > &unsatCore);

  bool mustBeTrue(const ExecutionState &state, ref<Expr> expr, bool &result) {
    std::vector<ref<Expr> > dummyUnsatCore;
    return mustBeTrue(state, expr, result, dummyUnsatCore);
  }

  bool mustBeFalse(const ExecutionState &state, ref<Expr> expr,
                   bool &result) {
    std::vector<ref<Expr> > dummyUnsatCore;
    return mustBeFalse(state, expr, result, dummyUnsatCore);
  }

  bool mayBeTrue(const ExecutionState &state, ref<Expr> expr, bool &result) {
    std::vector<ref<Expr> > dummyUnsatCore;
    return mayBeTrue(state, expr, result, dummyUnsatCore);
  }

  bool mayBeFalse(const ExecutionState &state, ref<Expr> expr, bool &result) {
    std::vector<ref<Expr> > dummyUnsatCore;
    return mayBeFalse(state, expr, result, dummyUnsatCore);
  }

  bool getValue(const ConstraintSet &, ref<Expr> expr,
                ref<ConstantExpr> &result, SolverQueryMetaData &metaData);

  bool getInitialValues(const ConstraintSet &,
                        const std::vector<const Array *> &objects,
                        std::vector<std::vector<unsigned char>> &result,
                        SolverQueryMetaData &metaData,
                        std::vector<ref<Expr> > &unsatCore);

  std::pair<ref<Expr>, ref<Expr>> getRange(const ConstraintSet &,
                                           ref<Expr> query,
                                           SolverQueryMetaData &metaData);
};
}

#endif /* KLEE_TIMINGSOLVER_H */
