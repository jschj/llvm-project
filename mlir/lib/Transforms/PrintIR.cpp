//===- PrintIR.cpp - Pass to dump IR on debug stream ----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "mlir/Pass/Pass.h"
#include "mlir/Transforms/Passes.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/FileSystem.h"

namespace mlir {
#define GEN_PASS_DEF_PRINTIRPASS
#include "mlir/Transforms/Passes.h.inc"
} // namespace mlir

using namespace mlir;

namespace {
struct PrintIRPass : public impl::PrintIRPassBase<PrintIRPass> {
  PrintIRPass(const PrintIRPassOptions &options) : PrintIRPassBase(options) {}

  PrintIRPass(const PrintIRPassOptions &options, OpPrintingFlags printingFlags)
      : PrintIRPassBase(options), printingFlags(printingFlags) {}

  void runOnOperation() override;

private:
  void printIR(raw_ostream &stream);

  OpPrintingFlags printingFlags = std::nullopt;
};
} // namespace

void PrintIRPass::runOnOperation() {
  if (fileName.empty()) {
    printIR(llvm::errs());
  } else {
    std::error_code EC;
    llvm::raw_fd_ostream stream(fileName, EC);

    if (EC) {
      llvm::errs() << "Could not open file: " << EC.message();
      signalPassFailure();
    } else {
      printIR(stream);
    }
  }

  markAllAnalysesPreserved();
}

void PrintIRPass::printIR(raw_ostream &stream) {
  stream << "// -----// IR Dump";
  if (!label.empty())
    stream << " " << label;
  stream << " //----- //\n";

  getOperation()->print(stream, printingFlags);
}

std::unique_ptr<Pass> mlir::createPrintIRPass(const PrintIRPassOptions &options,
                                              OpPrintingFlags printingFlags) {
  return std::make_unique<PrintIRPass>(options, printingFlags);
}
