#include <iostream>
#include "tokenizer.h"


std::vector<Token> Tokenizer::tokenize(std::string_view str){
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
		}else if (std::isdigit(static_cast<unsigned char>(str[i]))){ // loop to build a str for val
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



std::optional<char> Tokenizer::peek() const{
	if (m_ind + 1 >= m_src.length()){
		return {}
	}else{
		return m_src[m_ind + 1]
	}
}
















