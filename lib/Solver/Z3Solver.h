//===-- Z3Solver.h
//---------------------------------------------------===//
//
//                     The KLEE Symbolic Virtual Machine
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef KLEE_Z3SOLVER_H
#define KLEE_Z3SOLVER_H

#include "klee/Solver/Solver.h"

namespace klee {
enum Z3BuilderType {
    KLEE_CORE,
    KLEE_BITVECTOR
};

/// Z3Solver - A complete solver based on Z3
class Z3Solver : public Solver {
public:
  /// Subsumption check indicator
  static bool subsumptionCheck;

  /// Z3Solver - Construct a new Z3Solver.
  Z3Solver(Z3BuilderType type);

  bool evaluate(const Query &, Validity &result,
                std::vector<ref<Expr> > &unsatCore);

  bool mustBeTrue(const Query&, bool &result,
                  std::vector<ref<Expr> > &unsatCore);

  bool mustBeTrue(const Query &query, bool &result) {
    std::vector<ref<Expr> > dummyUnsatCore;
    return mustBeTrue(query, result, dummyUnsatCore);
  }

  bool getInitialValues(const Query &,
                        const std::vector<const Array *> &objects,
                        std::vector<std::vector<unsigned char> > &result,
                        std::vector<ref<Expr> > &unsatCore);

  /// Get the query in SMT-LIBv2 format.
  /// \return A C-style string. The caller is responsible for freeing this.
  virtual char *getConstraintLog(const Query &);

  /// setCoreSolverTimeout - Set constraint solver timeout delay to the given
  /// value; 0
  /// is off.
  virtual void setCoreSolverTimeout(time::Span timeout);

  /// directComputeValidity - Compute validity directly without other
  /// layers of solving
  bool directComputeValidity(const Query &query, Solver::Validity &result,
                             std::vector<ref<Expr> > &unsatCore);
};
}

#endif /* KLEE_Z3SOLVER_H */
