#include <iostream>
#include <string>
#include <fstream>
#include <string_view>
#include <optional>
#include <vector>
#include "tokenizer.h"
#include "parser.h"
#include "codegen.h"


int main(int argc, char* argv[]){	
	// Check if we have only 1 file to compile
	if (argc != 2){
		std::cerr << "Incorrect Usage" << std::endl;
		std::cerr << "Please provide 1 file to compile" << std::endl;
		return EXIT_FAILURE;
	}
	
	// Load the file and check if exists
	// std::cout << argv[1]  << std::endl;
	std::string file = argv[1];
	std::ifstream input(file);
	if (!input){
		std::cerr << "Could not open File" << std::endl;
		return EXIT_FAILURE;
	}

	// making an output file

	std::ofstream output("./../tests/translate.s");
	if (!output){
		std::cerr << "Could not open a file." << std::endl;
		return EXIT_FAILURE;
	}


	std::string line;
	while(std::getline(input, line)){
		std::cout << line << std::endl;
	
		// create tokens
		Tokenizer tokenizer = Tokenizer(line);
		std::vector<Token> tokens = tokenizer.tokenize();
		std::cout << "checking" << std::endl;
		Parser parser(tokens);
		std::cout << "trying" << std::endl;
		std::optional<NodeReturn> tree = parser.parse();
		if (!tree.has_value()){
			std::cerr << "No Return Statement Found" << std::endl;
			exit(EXIT_FAILURE);
		}
		Generator generator = Generator(tree.value());

		//	std::cout<<"debugging"<<std::endl;
		output << generator.generate() ;
	}
	return EXIT_SUCCESS;
}
