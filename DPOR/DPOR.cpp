#include "llvm/Pass.h"
//#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <math.h>
//#include <filesystem>

using namespace llvm;

//opcode quick lookup
//ret 1 -- store 32 -- cal 55 -- load 31 -- add 12 -- mul 16


/*static Function *printf_prototype(LLVMContext &ctx, Module *mod) {

  FunctionType *printf_type =
      TypeBuilder<int(char *, ...), false>::get(getGlobalContext());

  Function *func = cast<Function>(mod->getOrInsertFunction(
      "printf", printf_type,
      AttributeSet().addAttribute(mod->getContext(), 1U, Attribute::NoAlias)));

  return func;
}*/

static void arrange_order(Module* M, int bti[]){
	for(Module::iterator fb = M->begin(), fe = M->end(); fb!=fe; fb++){
		std::string tmpcheckstr,tmpstr;
		llvm::raw_string_ostream rso(tmpstr);
		std::size_t index_of_at;
		tmpcheckstr = (bti[1]==1)? "ThreadFun1" : "ThreadFun2";
		if(fb->getName()==tmpcheckstr){
			//outs() << "Function name: " << fb->getName() << '\n';
			
			//initializing function call to insert later
			LLVMContext& Ctx = fb->getContext();
			Constant* delayFunc = M->getOrInsertFunction(
			  "delay", Type::getVoidTy(Ctx), Type::getInt32Ty(Ctx));
			
			for(Function::iterator bb=fb->begin(), bbe=fb->end(); bb!=bbe; bb++){
				
				if(bb->getTerminator()->getOpcode() == 1){		//ret opcode 1
					//outs() << *bb << '\n';
					int counter=0;

					for(BasicBlock::iterator instr=bb->begin(), instre=bb->end(); instr!=instre; instr++){
						if(instr->getOpcode()==32){
							tmpstr="";		//store opcode 32
							instr->print(rso);
							index_of_at = tmpstr.find("@");
							if((int)tmpstr[index_of_at+1] != bti[2]) { 
								//outs()<<"didnt pass"<<'\n';
								//outs()<<"if cond: " << (int)tmpstr[index_of_at+1] << " and " << bti[2] <<'\n';
								//outs() << *instr << '\n';
								continue;
							}
							if(counter!=bti[0]) { counter++; continue; }
							//initilize irbuilder
							instr--;
							instr--;
							while(instr->getOpcode()!=55) instr--;
							IRBuilder<> builder(&*instr);
							std::vector<Value*> args;
							
							//push the args need to pass to runtime library
							args.push_back(builder.getInt32(1));		//delay time
							
							//inserting call
							builder.CreateCall(delayFunc, args);
							//outs() << "called indeed" <<'\n';
							
							break;
						}
					}					
				}
			}
		}
		
		tmpcheckstr = (bti[4]==1)? "ThreadFun1" : "ThreadFun2";
		if(fb->getName()==tmpcheckstr){
			//outs() << "Function name: " << fb->getName() << '\n';
			
			//initializing function call to insert later
			LLVMContext& Ctx = fb->getContext();
			Constant* delayFunc = M->getOrInsertFunction(
			  "delay", Type::getVoidTy(Ctx), Type::getInt32Ty(Ctx));
			
			for(Function::iterator bb=fb->begin(), bbe=fb->end(); bb!=bbe; bb++){
				
				if(bb->getTerminator()->getOpcode() == 1){		//ret opcode 1
					//outs() << *bb << '\n';
					int counter=0;

					for(BasicBlock::iterator instr=bb->begin(), instre=bb->end(); instr!=instre; instr++){
						if(instr->getOpcode()==32){
							tmpstr="";		//store opcode 32
							instr->print(rso);
							index_of_at = tmpstr.find("@");
							if((int)tmpstr[index_of_at+1] != bti[5]) { 
								//outs()<<"didnt pass"<<'\n';
								//outs()<<"if cond: " << (int)tmpstr[index_of_at+1] << " and " << bti[2] <<'\n';
								//outs() << *instr << '\n';
								continue;
							}
							if(counter!=bti[3]) { counter++; continue; }
							//initilize irbuilder
							instr++;
							instr++;
							IRBuilder<> builder(&*instr);
							std::vector<Value*> args;
							
							//push the args need to pass to runtime library
							args.push_back(builder.getInt32(2));		//delay time
							
							//inserting call
							builder.CreateCall(delayFunc, args);
							//outs() << "called indeed" <<'\n';
							
							break;
						}
					}
				}
			}
		}
		
	}
}

namespace {
struct DPOR : public ModulePass {
	static char ID;
	int valid_instr_counter=0;
	
	DPOR() : ModulePass(ID) {}

  	bool runOnModule(Module &M) { //overide deleted
		outs() << "--------------static part--------------\n";
		int funcnumb;
		std::string tmpstr;
		llvm::raw_string_ostream rso(tmpstr);
		std::size_t index_of_at;
		
		for(Module::iterator fb = M.begin(), fe = M.end(); fb!=fe; fb++){
			
			if(fb->getName()=="ThreadFun1" || fb->getName()=="ThreadFun2"){
				outs() << "Function name: " << fb->getName() << '\n';
				
				//initializing function call to insert later
				LLVMContext& Ctx = fb->getContext();
				Constant* logFunc = M.getOrInsertFunction(
				  "record_op", Type::getVoidTy(Ctx), Type::getInt32Ty(Ctx), Type::getInt32Ty(Ctx), Type::getInt32Ty(Ctx) );
				
				for(Function::iterator bb=fb->begin(), bbe=fb->end(); bb!=bbe; bb++){
					
					if(bb->getTerminator()->getOpcode() == 1){		//ret opcode 1
						//outs() << *bb << '\n';

						for(BasicBlock::iterator instr=bb->begin(), instre=bb->end(); instr!=instre; instr++){
							if(instr->getOpcode()==32){		//store opcode 32
								valid_instr_counter++;
								
								//initilize irbuilder
								IRBuilder<> builder(&*instr);
								std::vector<Value*> args;
								if(fb->getName()=="ThreadFun1") funcnumb=1;
								else funcnumb=2;
								
								//push the args need to pass to runtime library
								args.push_back(builder.getInt32(funcnumb));		//thread number
								args.push_back(builder.getInt32((--instr)->getOpcode()));		//opcode
								instr++;
								instr->print(rso);
								index_of_at = tmpstr.find("@");
								args.push_back(builder.getInt32(tmpstr[index_of_at+1]));		//operand(limitation only 1 char)
								tmpstr="";
								
								//inserting call
								builder.CreateCall(logFunc, args);
							}
						}					
					}
				}
			}
		}
				
				
				std::ifstream file("record.txt"); // std::ios::ate means open at end
				int valid_count_on_shared_var[50];
				valid_count_on_shared_var[0]=0;
				if ( file.peek() == std::ifstream::traits_type::eof() )
				{
					//outs() << "empty file. no more action needed\n"; 
				}else{
					//outs() << "not empty file. need to inspect\n";
					std::string tmpline,tmp;
					int tmpindex,i=0,j,k,line_count;
					
					//get length and reset back to start line
					for (line_count = 0; std::getline(file, tmpline); ++line_count)
					    ;
					file.clear();
					file.seekg(0, std::ios::beg);
					
					//initiliaze and fill array
					int** instr_array = new int*[line_count];
					for(int i = 0; i < line_count; i++)
					    instr_array[i] = new int[3];
					i=0;
					while (std::getline(file, tmpline)) {
						instr_array[i][0] = i%valid_instr_counter;		//oder of execution
						tmpindex = tmpline.find("+");
						tmp = tmpline.substr(0,tmpindex);
						//outs() << "check " << tmp <<'\n';
						instr_array[i][1] = std::stoi(tmp,nullptr);		//which thread
						tmpindex = tmpline.find("+",tmpindex+1);
						tmp = tmpline.substr(tmpindex+1,10);
						//outs() << "check " << tmp <<'\n';
						instr_array[i][2] = std::stoi(tmp,nullptr);		//on which operand
						i++;						
					}
					
					//printing for test
					/*for(i=0;i<line_count;i++){
						outs() << "array: " << instr_array[i][0] << " " << instr_array[i][1] << " " << instr_array[i][2] << '\n';
					}*/
					
					//check for order
					std::ifstream file2("nextbt.txt");
					int turn=0,numinturn=0,file2index;
					std::string file2string;
					if ( file2.peek() == std::ifstream::traits_type::eof() ){
						turn=1;
						numinturn=1; 
					}
					else{
						std::getline(file2, file2string);
						file2index = file2string.find("+");
						turn = std::stoi(file2string.substr(0,file2index),nullptr);
						numinturn = std::stoi(file2string.substr(file2index+1,10),nullptr);
						if (numinturn+1 > std::pow(2,(turn-1))){
							turn+=1;
							numinturn=1;
						}
						else numinturn++;
					}
					//outs()<<"turn and numinturn: "<< turn << " " << numinturn <<'\n';
					file2.close();

					int backtrack_instr[6],take_count=1;
					bool need_arrange = false,flag=false;
					for(i=(numinturn-1)*valid_instr_counter; i< numinturn * valid_instr_counter; i++){
						//int tmpoperand = instr_Array[i][2];
						//outs()<<"this is a test i"<<'\n';
						for(j=(numinturn-1)*valid_instr_counter;j<(numinturn-1)*valid_instr_counter+i;j++){		//pick a instr's operand
							//outs()<<"this is a test j"<<'\n';
							if(instr_array[i][2]==instr_array[j][2] && instr_array[i][1]!=instr_array[j][1]){	//check same operand before from other thread
								//outs()<<"this is a test k"<<'\n';
								if(valid_count_on_shared_var[0]==0){
									for(k=(numinturn-1)*valid_instr_counter;k<numinturn*valid_instr_counter;k++){
										//check the number of this instr	
										if(instr_array[k][2]==instr_array[j][2]) valid_count_on_shared_var[0]++;
									}
								}
								
								if(take_count<turn){
									//outs()<<"jumped"<<'\n';
									take_count++;
									continue;
								}
								
								int find_instr_j_counter=0,find_instr_i_counter=0;
								need_arrange=true;
								for(k=(numinturn-1)*valid_instr_counter;k<j;k++){
									//check the number of this instr	
									if(instr_array[k][1]==instr_array[j][1]&&instr_array[k][2]==instr_array[j][2]) find_instr_j_counter++;
									//outs()<<"testing :" << k << " " << j <<'\n';
								}
								for(k=(numinturn-1)*valid_instr_counter;k<i;k++){
									//check the number of this instr	
									if(instr_array[k][1]==instr_array[i][1]&&instr_array[k][2]==instr_array[i][2]) find_instr_i_counter++;
									//outs()<<"testing :" << k << " " << i <<'\n';
								}
								//outs() << "out"<<'\n';
								backtrack_instr[0] = find_instr_j_counter;
								backtrack_instr[1] = instr_array[j][1];
								backtrack_instr[2] = instr_array[j][2];
								backtrack_instr[3] = find_instr_i_counter;
								backtrack_instr[4] = instr_array[i][1];
								backtrack_instr[5] = instr_array[i][2];
								flag=true;
								break;
							}
						}
						if(flag) break;
					}
					
					/*outs() << "backtrack: " << backtrack_instr[0] << " " << backtrack_instr[1] << " " << backtrack_instr[2]<< " " 
						<< backtrack_instr[3] << " " << backtrack_instr[4] << " " << backtrack_instr[5] << '\n';*/
					if(need_arrange) {
						arrange_order(&M,backtrack_instr);
						//outs() << "delay func called" <<'\n';
						std::ofstream file2;
						std::string updatestring="";
						file2.open("nextbt.txt", std::ofstream::trunc);
						updatestring.append(std::to_string(turn));
						updatestring.append("+");
						updatestring.append(std::to_string(numinturn));
						file2<<updatestring<<std::endl;
						
						if(turn==valid_count_on_shared_var[0]-1 &&  numinturn == std::pow(2,(turn-1))){
							std::string outputstring;
							outputstring ="Done. Each run has ";
							outputstring.append(std::to_string(valid_instr_counter));
							outputstring.append(" instructions.");
							file2<<outputstring<<std::endl;
							file2.close();
							//outs()<<"exit flag written"<<'\n';
						}else{
							file2.close();
						}
					}				
				}
				file.close();
			
		return false;
  	}
}; // end of struct HWBasicBlock
}  // end of anonymous namespace

char DPOR::ID = 0;
static RegisterPass<DPOR> X("dpor", "Dynamic Partial-Order Reduction", false , false );
/*static void registerDPOR(const PassManagerBuilder &,
                         legacy::PassManagerBase &PM) {
  PM.add(new DPOR());
}
static RegisterStandardPasses
  RegisterMyPass(PassManagerBuilder::EP_EarlyAsPossible,
                 registerDPOR);*/
