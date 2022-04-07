#===------------------------------------------------------------------------===#
#
#                     The KLEE Symbolic Virtual Machine
#
# This file is distributed under the University of Illinois Open Source
# License. See LICENSE.TXT for details.
#
#===------------------------------------------------------------------------===#
add_executable(kleaver
  main.cpp
)

set(LLVM_COMPONENTS
  kleaverSolver
  kleeCore
)
klee_get_llvm_libs(LLVM_LIBS ${LLVM_COMPONENTS})
target_link_libraries(kleaver ${KLEE_LIBS})

install(TARGETS kleaver RUNTIME DESTINATION bin)
