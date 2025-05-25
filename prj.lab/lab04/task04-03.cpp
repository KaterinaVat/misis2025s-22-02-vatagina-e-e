#include <iostream>


int main(int argc, char** argv) {
	if argc != 4{
		std::cerr << "Invalid format input data" << std::endl;
		std::cerr << " - path to lts-input perfect data" << std::endl;
		std::cerr << " - path to lst-input detect data" << std::endl;
		std::cerr << " - path to output" << std::endl;
		return -1;
	} 
	std::string pData = argc[0];
	std::string dData = argc[1];
	std::string oPath = argc[2];


}