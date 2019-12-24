#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <unistd.h>

extern "C" __attribute__((visibility("default")))
void record_op(int threadnumber, int opcode, int operand){
	printf("thread: %d  opecode %d  operand: %d\n", threadnumber, opcode, operand);
    std::ofstream output("record.txt", std::ios_base::app);
	std::string tmp;
	tmp.append(std::to_string(threadnumber));
	tmp.append("+");
	tmp.append(std::to_string(opcode));
	tmp.append("+");
	tmp.append(std::to_string(operand));
    output<<tmp<<std::endl;
	output.close();
}

extern "C" __attribute__((visibility("default")))
void delay(int delay_time){
	int sleep_time = delay_time * 100000;
	printf("sleeping for %d microsec \n", sleep_time);
	usleep(sleep_time);
}