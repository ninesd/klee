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
        Solver/AssignmentValidatingSolver.cpp
        Solver/CachingSolver.cpp
        Solver/CexCachingSolver.cpp
        Solver/ConstantDivision.cpp
        Solver/ConstructSolverChain.cpp
        Solver/CoreSolver.cpp
        Solver/DummySolver.cpp
        Solver/FastCexSolver.cpp
        Solver/IncompleteSolver.cpp
        Solver/IndependentSolver.cpp
        Solver/MetaSMTSolver.cpp
        Solver/KQueryLoggingSolver.cpp
        Solver/QueryLoggingSolver.cpp
        Solver/SMTLIBLoggingSolver.cpp
        Solver/Solver.cpp
        Solver/SolverCmdLine.cpp
        Solver/SolverImpl.cpp
        Solver/SolverStats.cpp
        Solver/STPBuilder.cpp
        Solver/STPSolver.cpp
        Solver/ValidatingSolver.cpp
        Solver/Z3Builder.cpp
        Solver/Z3BitvectorBuilder.cpp
        Solver/Z3CoreBuilder.cpp
        Solver/Z3HashConfig.cpp
        Solver/Z3Solver.cpp
        Core/CmdLineOptions.cpp
)