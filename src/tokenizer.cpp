#include <iostream>
#include "tokenizer.h"
#include <cstdlib>

Tokenizer::Tokenizer(std::string_view s)
	: m_src(s),
	  m_ind(0)
{}

std::vector<Token> Tokenizer::tokenize(){
	std::vector<Token> tokens;
	std::string buf;
 	while(peek().has_value()){
 		if (std::isalpha(static_cast<unsigned char>(peek().value()))){
			 // loop to build a name or a keyword
 			while(peek().has_value() && std::isalpha(static_cast<unsigned char>(peek().value()))){
				buf.push_back(consume());
			}
			if (buf == "return"){
				tokens.push_back({.type = TokenType::_return});
				std::cout << buf << std::endl;
				buf.clear();
			}else{
				std::cerr << "You messed up. Not a keyword" << std::endl;
				exit(EXIT_FAILURE);
			}
		}else if (std::isdigit(static_cast<unsigned char>(peek().value()))){
			 // loop to build a num for val
			while(peek().has_value() && std::isdigit(static_cast<unsigned char>(peek().value()))){
				buf.push_back(consume());
				}
			tokens.push_back({.type = TokenType::int_lit, .value=buf});
			std::cout << buf << std::endl;
			buf.clear();
		}else if(peek().value() == '('){
			consume();
			tokens.push_back({.type = TokenType::open_paren});
		}else if(peek().value() == ')'){
			consume();
			tokens.push_back({.type = TokenType::close_paren});
		}else if(peek().value() == ';'){
			consume();
			tokens.push_back({.type = TokenType::semi});
		}else if (std::isspace(static_cast<unsigned char>(peek().value()))){ 
			// increment in case of whitespace
			consume();
		}else{
			tokens.push_back({.type = TokenType::ident, .value = buf});
			buf.clear()
		}
	}
	std::cout << "Done with the Line" << std::endl;
	m_ind = 0;
	return tokens;
}



std::optional<char> Tokenizer::peek(int offset) const{
	if (m_ind + offset >= m_src.length()){
		return {};
	}else{
		return m_src[m_ind + offset];
	}
}

char Tokenizer::consume(){
	return m_src[m_ind++];
}
















