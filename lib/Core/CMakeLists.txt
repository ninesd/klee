#===------------------------------------------------------------------------===#
#
#                     The KLEE Symbolic Virtual Machine
#
# This file is distributed under the University of Illinois Open Source
# License. See LICENSE.TXT for details.
#
#===------------------------------------------------------------------------===#
klee_add_component(kleeCore
  AddressSpace.cpp
  MergeHandler.cpp
  CallPathManager.cpp
  CmdLineOptions.cpp
  Context.cpp
  CoreStats.cpp
  ExecutionState.cpp
  Executor.cpp
  ExecutorUtil.cpp
  ExternalDispatcher.cpp
  ImpliedValue.cpp
  Memory.cpp
  MemoryManager.cpp
  PTree.cpp
  Searcher.cpp
  SeedInfo.cpp
  SpecialFunctionHandler.cpp
  StatsTracker.cpp
  TimingSolver.cpp
  TxDependency.cpp
  TxExprHelper.cpp
  TxPartitionHelper.cpp
  TxPathCondition.cpp
  TxPrintUtil.cpp
  TxShadowArray.cpp
  TxSpeculation.cpp
  TxStore.cpp
  TxTree.cpp
  TxTreeGraph.cpp
  TxValues.cpp
  TxWP.cpp
  TxWPHelper.cpp
  UserSearcher.cpp
  Z3Simplification.cpp
)

# TODO: Work out what the correct LLVM components are for
# kleeCore.
set(LLVM_COMPONENTS
  core
  executionengine
  mcjit
  native
  support
  llvm_libs
)

klee_get_llvm_libs(LLVM_LIBS ${LLVM_COMPONENTS})
target_link_libraries(kleeCore PUBLIC ${LLVM_LIBS} ${SQLITE3_LIBRARIES})
target_link_libraries(kleeCore PRIVATE
  kleeBasic
  kleeModule
  kleaverSolver
  kleaverExpr
  kleeSupport
)
