//
// Created by Lenovo on 2022/4/7.
//
//
// Created by Lenovo on 2022/4/7.
//

#include "klee/CommandLine.h"

namespace klee {
// TracerX cmd options
llvm::cl::opt<bool> NoInterpolation(
    "no-interpolation",
    llvm::cl::desc("Disable interpolation for search space reduction. "
                   "Interpolation is enabled by default when Z3 was the solver "
                   "used. This option has no effect when Z3 was not used."));

llvm::cl::opt<bool> OutputTree(
    "output-tree",
    llvm::cl::desc("Outputs tree.dot: the execution tree in .dot file "
                   "format. At present, this feature is only available when "
                   "Z3 is compiled in and interpolation is enabled."));

llvm::cl::opt<bool>
    SubsumedTest("subsumed-test",
                 llvm::cl::desc("Enables generation of test cases for subsumed "
                                "paths. This is needed for the computation of "
                                "coverage using gcov or llvm-cov, as otherwise the "
                                "part of code visited by the subsumed paths are "
                                "not considered in the coverage computation."),
                 llvm::cl::init(false));

llvm::cl::opt<bool> NoExistential(
    "no-existential",
    llvm::cl::desc(
        "This option avoids existential quantification in subsumption "
        "check by equating each existential variable with its corresponding "
        "free variable. For example, when checking if x < 10 is subsumed by "
        "another state where there is x' s.t., x' <= 0 && x = x' + 20 (here "
        "the existential variable x' represents the value of x before adding "
        "20), we strengthen the query by adding the constraint x' = x. This "
        "has an effect of removing all existentially-quantified variables "
        "most solvers are not very powerful at solving, however, at likely "
        "less number of subsumptions due to the strengthening of the query."));

llvm::cl::opt<int> MaxFailSubsumption(
    "max-subsumption-failure",
    llvm::cl::desc("To set the maximum number of failed subsumption check. "
                   "When this options is specified and the number of "
                   "subsumption table entries is more than the specified "
                   "value, the oldest entry will be deleted (default=0 (off))"),
    llvm::cl::init(0));

llvm::cl::opt<int>
    DebugState("debug-state",
               llvm::cl::desc("Dump information on symbolic execution state when "
                              "visited (default=0 (off))."),
               llvm::cl::init(0));

llvm::cl::opt<int> DebugSubsumption(
    "debug-subsumption",
    llvm::cl::desc("Set level of debug information on subsumption checks: the "
                   "higher the more (default=0 (off))."),
    llvm::cl::init(0));

llvm::cl::opt<int>
    BBCoverage("write-BB-cov",
               llvm::cl::desc("Set level of distinct basic block coverage: the "
                              "higher the more (default=0 (off))\n"
                              "= 1 Only report Basic Block Coverage\n"
                              "= 2 Also Report Covered Basic Blocks\n"
                              "= 3 Report Live Coverage\n"
                              "= 4 Report ICMP/Atomic Condition Coverage. Note: "
                              "Useful to compare coverage with CBMC\n"
                              "= 5 Generate Coverage by Time Plot\n"),
               llvm::cl::init(0));

llvm::cl::opt<bool> EmitAllErrorsInSamePath(
    "emit-all-errors-in-same-path",
    llvm::cl::desc("Enables detection of multiple errors "
                   "in same paths (default=false (off)). Note: Specially used "
                   "for achieving MC/DC."),
    llvm::cl::init(false));

llvm::cl::opt<bool> ExactAddressInterpolant(
    "exact-address-interpolant",
    llvm::cl::desc("This option uses exact address for interpolating "
                   "successful out-of-bound memory access instead of the "
                   "default memory offset bound."));

llvm::cl::opt<bool> SpecialFunctionBoundInterpolation(
    "special-function-bound-interpolation",
    llvm::cl::desc("Perform memory access interpolation only within function "
                   "named tracerx_check, either memory offset bound or exact "
                   "address (enabled with -exact-address-interpolant)."),
    llvm::cl::init(false));

llvm::cl::opt<bool> TracerXPointerError(
    "tracerx-pointer-error",
    llvm::cl::desc("Enables detection of more memory errors by interpolation "
                   "shadow memory (may be false positives)."),
    llvm::cl::init(false));

llvm::cl::opt<SpecType>
    SpecTypeToUse("spec-type",
                  llvm::cl::desc("Speculation type: coverage or safety"),
                  llvm::cl::values(clEnumValN(SAFETY, "safety", "safety"),
                                   clEnumValN(COVERAGE, "coverage", "coverage")),
                  llvm::cl::init(NO_SPEC));

llvm::cl::opt<SpecStrategy> SpecStrategyToUse(
    "spec-strategy",
    llvm::cl::desc(
        "Strategy used by speculation, timid, aggressive or custom."),
    llvm::cl::values(clEnumValN(TIMID, "timid", "timid"),
                     clEnumValN(AGGRESSIVE, "aggressive", "aggressive"),
                     clEnumValN(CUSTOM, "custom", "custom")),
    llvm::cl::init(CUSTOM));

llvm::cl::opt<std::string> DependencyFolder(
    "spec-dependency",
    llvm::cl::desc(
        "Path to a folder containing basic blocks' dependency."
        "One file for each BB with name format: \"BB_Dep_{order}.txt\""
        "An initial file containing visited BBs with name "
        "\"InitialVisitedBB.txt\""
        "also must be put in this folder"),
    llvm::cl::init("."));

llvm::cl::opt<bool>
    WPInterpolant("wp-interpolant",
                  llvm::cl::desc("Perform weakest-precondition interpolation"),
                  llvm::cl::init(false));

llvm::cl::opt<bool>
    MarkGlobal("mark-global",
               llvm::cl::desc("Decide whether global variables are marked or not"),
               llvm::cl::init(true));
}