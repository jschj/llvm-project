//===-- IA64PCInstrBuilder.h - Aids for building IA64 insts -----*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file was developed by Duraid Madina and is distributed under the
// University of Illinois Open Source License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file exposes functions that may be used with BuildMI from the
// MachineInstrBuilder.h file to simplify generating frame and constant pool
// references.
//
//===----------------------------------------------------------------------===//

#ifndef IA64_INSTRBUILDER_H
#define IA64_INSTRBUILDER_H

#include "llvm/CodeGen/MachineInstrBuilder.h"

namespace llvm {

/// addFrameReference - This function is used to add a reference to the base of
/// an abstract object on the stack frame of the current function.  This
/// reference has base register as the FrameIndex offset until it is resolved.
/// This allows a constant offset to be specified as well...
///
inline const MachineInstrBuilder&
addFrameReference(const MachineInstrBuilder &MIB, int FI, int Offset = 0,
                  bool mem = true) {
  if (mem)
    return MIB.addSImm(Offset).addFrameIndex(FI);
  else
    return MIB.addFrameIndex(FI).addSImm(Offset);
}

/// addConstantPoolReference - This function is used to add a reference to the
/// base of a constant value spilled to the per-function constant pool.  The
/// reference has base register ConstantPoolIndex offset which is retained until
/// either machine code emission or assembly output.  This allows an optional
/// offset to be added as well.
///
inline const MachineInstrBuilder&
addConstantPoolReference(const MachineInstrBuilder &MIB, unsigned CPI,
                         int Offset = 0) {
  return MIB.addSImm(Offset).addConstantPoolIndex(CPI);
}

} // End llvm namespace

#endif

