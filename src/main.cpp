#include <iostream>
#include <string>
#include <fstream>
#include <string_view>
#include <optional>
#include <vector>

enum class TokenType {
	_return,
	int_lit,
	semi
};

struct Token {
	TokenType type;
	std::optional<std::string> value {};
};

std::vector<Token> tokenize(std::string_view str){
	std::vector<Token> tokens;
	std::string buf;
	size_t str_len = str.length();
	size_t i{};
	while(i < str_len){
		if (std::isalpha(static_cast<unsigned char>(str[i]))){ // loop to build a name or a keyword
			while(i < str_len && std::isalpha(static_cast<unsigned char>(str[i]))){
				buf.push_back(str[i]);
				++i;
			}
			 if (buf == "return"){
				tokens.push_back({.type = TokenType::_return});
				std::cout << buf << std::endl;
				buf = "";
			}else{
				continue;
			}
		} else if (std::isdigit(static_cast<unsigned char>(str[i]))){ // loop to build a str for val
			while(i < str_len && std::isdigit(static_cast<unsigned char>(str[i]))){
				buf.push_back(str[i]);
				++i;
			}
			tokens.push_back({.type = TokenType::int_lit, .value=buf});
			std::cout << buf << std::endl;
			buf = "";
		}else if(str[i] == ';'){
			tokens.push_back({.type = TokenType::semi});
			++i;
		}else{ // increment in case of whitespace
			++i;
		}
		}
	std::cout << "Done with the Line" << std::endl;	
	return tokens;
}

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

/*	std::ofstream output("./tests/translate.s");
	if (!output){
		std::cerr << "Could not build an executable." << std::endl;
		return EXIT_FAILURE;
	}
*/

	std:: string line;
	while(std::getline(input, line)){
		std::cout << line << std::endl;
		//std::string_view = view(line);
		std::vector<Token> tokens = tokenize(line);
//		output << line;
	}
	return EXIT_SUCCESS;
}
