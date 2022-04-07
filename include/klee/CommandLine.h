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

llvm::cl::opt<bool> NoInterpolation;

llvm::cl::opt<bool> OutputTree;

llvm::cl::opt<bool> SubsumedTest;

llvm::cl::opt<bool> NoExistential;

llvm::cl::opt<int> MaxFailSubsumption;

llvm::cl::opt<int> DebugState;

llvm::cl::opt<int> DebugSubsumption;

llvm::cl::opt<int> BBCoverage;

llvm::cl::opt<bool> ExactAddressInterpolant;

llvm::cl::opt<bool> SpecialFunctionBoundInterpolation;

llvm::cl::opt<bool> TracerXPointerError;

llvm::cl::opt<bool> EmitAllErrorsInSamePath;

llvm::cl::opt<SpecType> SpecTypeToUse;

llvm::cl::opt<SpecStrategy> SpecStrategyToUse;

llvm::cl::opt<std::string> DependencyFolder;

llvm::cl::opt<bool> WPInterpolant;

llvm::cl::opt<bool> MarkGlobal;
}

#endif // KLEE_COMMANDLINE_H
