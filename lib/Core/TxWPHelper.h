//===-- TxWPHelper.h - Tracer-X symbolic execution tree -------------*- C++
//-*-===//
//
//               The Tracer-X KLEE Symbolic Virtual Machine
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// This file contains declarations of the classes that support for generating
/// weakest precondition interpolation.
///
//===----------------------------------------------------------------------===//

#ifndef LIB_CORE_TXWPHELPER_H_
#define LIB_CORE_TXWPHELPER_H_

#include "TxWP.h"
#include "klee/ExecutionState.h"
#include <klee/Expr.h>
#include <klee/ExprBuilder.h>
#include <klee/Internal/Support/ErrorHandling.h>
#include <klee/util/ArrayCache.h>
#include <vector>

namespace klee {

class TxWPArrayStore;

class TxWPHelper {

public:

  static bool isTargetDependent(llvm::Value *inst, ref<Expr> expr);

  static ref<Expr> substituteExpr(ref<Expr> base, const ref<Expr> lhs,
                                  const ref<Expr> rhs);

};

} /* namespace klee */

#endif /* LIB_CORE_TXWPHELPER_H_ */
