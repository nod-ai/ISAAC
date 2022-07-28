#include "conversion/RaiseToLLVM.h"

using namespace llvm;

PTXToLLVMConverter::PTXToLLVMConverter(std::string_view name, LLVMContext &context_) : context(context_) {
    module = std::make_unique<Module>(name, context);
    // TODO: Do we need to hardcode these values or can they be deduced from PTX?
    module->setDataLayout("e-i64:64-i128:128-v16:16-v32:32-n16:32:64");
    module->setTargetTriple("nvptx-nvidia-cuda");
}

void PTXToLLVMConverter::AddFunction(PTXKernel &kernel) {
}

std::string PTXToLLVMConverter::printModule() const {
  std::string str;
  llvm::raw_string_ostream os(str);
  os << *module;
  os.flush();
  return str;
}

Module *PTXToLLVMConverter::RaiseToLLVM(PTXProgram &program, LLVMContext &context) {
  PTXToLLVMConverter converter(program.getName(), context);
  for (auto kernel : program.getKernels()) {
    converter.AddFunction(kernel);
  }
  return converter.getModule();
}
