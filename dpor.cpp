#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <string>

int main(){
	//system("compile.sh");
	std::ofstream ofile;
	ofile.open("record.txt", std::ofstream::trunc);
	ofile.close();
	ofile.open("nextbt.txt", std::ofstream::trunc);
	ofile.close();
	
	while(true){
		system("sh run.sh");
		std::string check;
		std::ifstream file2("nextbt.txt");
		std::getline(file2, check);
		std::getline(file2, check);
		file2.close();
		if(check.substr(0,4)=="Done"){
			printf("exit normally\n");
			break;
		}
	}
	//system("sh run.sh");
	//system("sh run.sh");
	
	
	
	return 0;
}