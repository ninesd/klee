#===------------------------------------------------------------------------===#
#
#                     The KLEE Symbolic Virtual Machine
#
# This file is distributed under the University of Illinois Open Source
# License. See LICENSE.TXT for details.
#
#===------------------------------------------------------------------------===#
add_subdirectory(Basic)
add_subdirectory(Support)
add_subdirectory(Expr)
add_subdirectory(Solver)
add_subdirectory(Module)
add_subdirectory(Core)

klee_add_component(kleaverLibs
AssignmentValidatingSolver.cpp
CachingSolver.cpp
CexCachingSolver.cpp
ConstantDivision.cpp
ConstructSolverChain.cpp
CoreSolver.cpp
DummySolver.cpp
FastCexSolver.cpp
IncompleteSolver.cpp
IndependentSolver.cpp
MetaSMTSolver.cpp
KQueryLoggingSolver.cpp
QueryLoggingSolver.cpp
SMTLIBLoggingSolver.cpp
Solver.cpp
SolverCmdLine.cpp
SolverImpl.cpp
SolverStats.cpp
STPBuilder.cpp
STPSolver.cpp
ValidatingSolver.cpp
Z3Builder.cpp
Z3BitvectorBuilder.cpp
Z3CoreBuilder.cpp
Z3HashConfig.cpp
Z3Solver.cpp
CmdLineOptions.cpp
)