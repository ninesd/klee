//
// Created by Lenovo on 2022/4/6.
//

#ifndef KLEE_COMMANDLINE_H
#define KLEE_COMMANDLINE_H

#include "llvm/Support/CommandLine.h"
#include "klee/Config/config.h"

#define INTERPOLATION_ENABLED (!NoInterpolation)
#define OUTPUT_INTERPOLATION_TREE (INTERPOLATION_ENABLED &&OutputTree)

namespace klee {
enum SpecType { NO_SPEC, SAFETY, COVERAGE };

enum SpecStrategy { TIMID, AGGRESSIVE, CUSTOM };

extern llvm::cl::opt<bool> NoInterpolation;

extern llvm::cl::opt<bool> OutputTree;

extern llvm::cl::opt<bool> SubsumedTest;

extern llvm::cl::opt<bool> NoExistential;

extern llvm::cl::opt<int> MaxFailSubsumption;

extern llvm::cl::opt<int> DebugState;

extern llvm::cl::opt<int> DebugSubsumption;

extern llvm::cl::opt<int> BBCoverage;

extern llvm::cl::opt<bool> ExactAddressInterpolant;

extern llvm::cl::opt<bool> SpecialFunctionBoundInterpolation;

extern llvm::cl::opt<bool> TracerXPointerError;

extern llvm::cl::opt<bool> EmitAllErrorsInSamePath;

extern llvm::cl::opt<SpecType> SpecTypeToUse;

extern llvm::cl::opt<SpecStrategy> SpecStrategyToUse;

extern llvm::cl::opt<std::string> DependencyFolder;

extern llvm::cl::opt<bool> WPInterpolant;

extern llvm::cl::opt<bool> MarkGlobal;

extern cl::opt<bool> DebugTracerX;
}

#endif // KLEE_COMMANDLINE_H
