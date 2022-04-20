//===-- Checks.cpp --------------------------------------------------------===//
//
//                     The KLEE Symbolic Virtual Machine
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "Passes.h"

#include "klee/Config/Version.h"

#include "KLEEIRMetaData.h"

#include "llvm/IR/Constants.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/Pass.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"

using namespace llvm;
using namespace klee;

char DivCheckPass::ID;

bool DivCheckPass::runOnModule(Module &M) {
  std::vector<llvm::BinaryOperator *> divInstruction;

  bool moduleChanged = false;

  for (auto &F : M) {
    for (auto &BB : F) {
      for (auto &I : BB) {
        auto binOp = dyn_cast<BinaryOperator>(&I);
        if (!binOp)
          continue;

        // find all [s|u][div|rem] instructions
        auto opcode = binOp->getOpcode();
        if (opcode != Instruction::SDiv && opcode != Instruction::UDiv &&
            opcode != Instruction::SRem && opcode != Instruction::URem)
          continue;

        CastInst *denominator =
            CastInst::CreateIntegerCast(I.getOperand(1),
                                        Type::getInt64Ty(getGlobalContext()),
                                        false,  /* sign doesn't matter */
                                        "int_cast_to_i64",
                                        &I);

        // Lazily bind the function to avoid always importing it.
        if (!divZeroCheckFunction) {
          Constant *fc = M.getOrInsertFunction("klee_div_zero_check",
                                               Type::getVoidTy(getGlobalContext()),
                                               Type::getInt64Ty(getGlobalContext()),
                                               NULL);
          divZeroCheckFunction = cast<Function>(fc);
        }

        CallInst * ci = CallInst::Create(divZeroCheckFunction, denominator, "", I);

        // Set debug location of checking call to that of the div/rem
        // operation so error locations are reported in the correct
        // location.
        ci->setDebugLoc(binOp->getDebugLoc());
        moduleChanged = true;
      }
    }
  }
  return moduleChanged;
}

char OvershiftCheckPass::ID;

bool OvershiftCheckPass::runOnModule(Module &M) {
  std::vector<llvm::BinaryOperator *> shiftInstructions;

  bool moduleChanged = false;

  for (auto &F : M) {
    for (auto &BB : F) {
      for (auto &I : BB) {
        auto binOp = dyn_cast<BinaryOperator>(&I);
        if (!binOp)
          continue;

        // find all shift instructions
        auto opcode = binOp->getOpcode();
        if (opcode != Instruction::Shl && opcode != Instruction::LShr &&
            opcode != Instruction::AShr)
          continue;

        std::vector<llvm::Value*> args;

        // Determine bit width of first operand
        uint64_t bitWidth=I.getOperand(0)->getType()->getScalarSizeInBits();

        ConstantInt *bitWidthC = ConstantInt::get(Type::getInt64Ty(getGlobalContext()),bitWidth,false);
        args.push_back(bitWidthC);

        CastInst *shift =
            CastInst::CreateIntegerCast(I.getOperand(1),
                                        Type::getInt64Ty(getGlobalContext()),
                                        false,  /* sign doesn't matter */
                                        "int_cast_to_i64",
                                        &I);
        args.push_back(shift);


        // Lazily bind the function to avoid always importing it.
        if (!overshiftCheckFunction) {
          Constant *fc = M.getOrInsertFunction("klee_overshift_check",
                                               Type::getVoidTy(getGlobalContext()),
                                               Type::getInt64Ty(getGlobalContext()),
                                               Type::getInt64Ty(getGlobalContext()),
                                               NULL);
          overshiftCheckFunction = cast<Function>(fc);
        }

        // Inject CallInstr to check if overshifting possible
        CallInst* ci = CallInst::Create(overshiftCheckFunction, args, "", I);
        // set debug information from binary operand to preserve it
        ci->setDebugLoc(binOp->getDebugLoc());
        moduleChanged = true;
      }
    }
  }

  return moduleChanged;
}
