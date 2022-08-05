set(SRC
  RaiseToLLVM.cpp
  TypeInference.cpp
  Utils.cpp
)
add_library(PTXToLLVMConversion ${SRC})
target_link_libraries(PTXToLLVMConversion
  PUBLIC
  PTXIR
  LLVMCore
  LLVMSupport
  LLVMTransformUtils
)
