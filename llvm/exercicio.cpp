#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/JIT.h"

#include "llvm/Analysis/Verifier.h"

#include <iostream>
#include <vector>

int main() {
	llvm::LLVMContext &context = llvm::getGlobalContext();
	llvm::Module* module;
	llvm::IRBuilder<> builder(context);

	module = new llvm::Module("intermediary code", context);

	/* function Main */
	llvm::Type* int64Type = llvm::Type::getInt64Ty(context);
	llvm::FunctionType* typeOfMain = llvm::FunctionType::get(int64Type, false);

	llvm::Function* mainFunction = llvm::Function::Create(typeOfMain, llvm::Function::ExternalLinkage, "main", module);

	llvm::BasicBlock* mainBB = llvm::BasicBlock::Create(context, "mainBB", mainFunction);

	builder.setInsertPoint(mainBB);

	/* function Teste */
	llvm::Type* boolType = llvm::Type::getInt1Ty(context);
	std::vector<llvm::Type*> testeParams(3, boolType);
	llvm::FunctionType* typeOfTeste = llvm::FunctionType::get(boolType, testeParams, false);
	llvm::Function* testeFunction = llvm::Function::Create(typeOfTeste, llvm::Function::ExternalLinkage, "teste", module);

	llvm::Function::arg_iterator params = testeFunction->arg_begin();
	params->setName("x");
	params++;
	params->setName("y");
	params++;
	params->setName("y");

	llvm::BasicBlock* testeBB = llvm::BasicBlock::Create(context, "testeBB", testeFunction);
	builder.setInsertPoint(testeBB);

	llvm::Function::arg_iterator declParams = testeFunction->arg_begin();
	
	auto x = declParams;
	auto y = ++declParams;
	auto z = ++declParams;

	auto xy = builder.createAnd(x, y, "andxy");
	auto xz = builder.createAnd(x, z, "andxz");
	auto nz = builder.createNot(z, "nz");
	auto ny = builder.createNot(y, "ny");
	auto nzny = builder.createAnd(nz, ny, "nzny");
	auto res = builder.createOr(xy, xz, "res");
	res = builder.createOr(res, nzny, "res");

	builder.createRet(res);

	llvm::verifyFunction(*testeFunction);


	/* function Exemplo */
	llvm::Type* realType = llvm::Type::getDoubleTy(context);
	std::vector<llvm::Type*> exemploParams(2, realType);
	llvm::FunctionType* typeOfExemplo = llvm::FunctionType::get(realType, exemploParams, false);
	llvm::Function* exemploFunction = llvm::Function::Create(typeOfExemplo, llvm::Function::ExternalLinkage, "exemplo", module);

	params = exemploFunction->arg_begin();
	params->setName("a");
	params++;
	params->setName("b");

	llvm::BasicBlock* exemploBB = llvm::BasicBlock::Create(context, "exemploBB", exemploFunction);
	builder.setInsertPoint(exemploBB);

	auto a = exemploFunction->arg_begin();
	auto b = ++(exemploFunction->arg_begin());

	auto multab = builder.createMul(a, b, "multab");
	auto const20 = llvm::ConstantDouble::get(context, llvm::APDouble(64, 2.0));
	auto modab = builder.createFRem(a, b, "modab");//???
	auto ret = builder.createMul(const20, modab, "ret");
	ret = builder.createSub(multab, ret, "ret");

	builder.createRet(ret);

	llvm::verifyFunction(*exemploFunction);

	/* Chamando as funções */
	builder.setInsertPoint(mainBB);

	std::vector<llvm::Value*> args;
	args.push_back(llvm::ConstantInt::get(context, llvm::APInt(1, 0)));
	args.push_back(llvm::ConstantInt::get(context, llvm::APInt(1, 0)));
	args.push_back(llvm::ConstantInt::get(context, llvm::APInt(1, 1)));
	auto testeRet = builer.CreateCall(testeFunction, args, "callteste");

	std::vector<llvm::Value*> args2;
	args2.push_back(llvm::ConstantDouble::get(context, llvm::APDouble(64, 10.0)));
	args2.push_back(llvm::ConstantDouble::get(context, llvm::APDouble(64, 5.0)));
	auto exemploRet = builder.CreateCall(exemploFunction, args2, "callexemplo");


	builder.createRet(testeRet);

	module->dump();

	llvm::ExecutionEngine* execEngine;
	std::string err;
	LLVMInitializeNativeTarget();

	execEngine = llvm::EngineBuilder(module).setErrorStr(&err).create();
	if(!execEngine) {
		fprintf(stderr, "Could not create execEngine: %s\n", err.c_str());
		exit(1);
	}

	void* mainPtr = execEngine->getPointerToFunction(mainFunction);

	int(*result)() = (int (*)())(intptr_t)mainPtr;
	std::cout << "Result of main: " << result() << std::endl;
}