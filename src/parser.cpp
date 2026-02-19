#include <iostream>
#include "parser.h"
#include <optional>

Parser::Parser(std::vector<Token> tokens)
	: m_tokens(std::move(tokens))
{
	
}


std::optional<Token> Parser::peek(size_t offset) const{
	if ((m_ind + offset) >= m_tokens.size()){
		return {};
	}else{
		return m_tokens[m_ind + offset];
	}
}

Token Parser::consume(){
	return m_tokens[m_ind++];
}

std::optional<NodeExpr> Parser::parse_expr(){
	if(peek().has_value() && peek().value().type == TokenType::int_lit){
		return NodeExpr{
			.var = NodeExprIntLit {.int_lit = consume()}
		};
	}else if(peek().has_value() && peek().value().type == TokenType::ident){
		return NodeExpr{
			.var = NodeExprIdent{.ident = consume()}
		};
	}else{
		return {};
	}
};


std::optional<NodeStatement> parse_statement(){
	if(peek().has_value() && peek().value().type == TokenType::_return){
		consume();
		auto node_return = NodeStatement{
			.stmt = NodeStatementReturn
		};
		if (auto node_expr = parse_expr()){
			node_return = {.expr = node_expr.value()}
		}else{
			std::cerr << "Not a Valid expression" << std::endl;
		}
	}else if(peek().has_value() && peek().value() == TokenType::semi){
		consume();
		exit(EXIT_FAILURE);
	}else{
		std::cerr << "Invalid Expression 2" << std::endl;
		exit(EXIT_FAILURE);
	}
}


	
std::optional<NodeReturn> Parser::parse(){
	std::optional<NodeReturn> node_return;
	while(peek().has_value()){
		std::cout << "checking" << std::endl; 
		if (peek().value().type == TokenType::_return){
			consume();
			if(auto node_expr = parse_expr()){
				std::cout << "Found Return" << std::endl;
				node_return = NodeReturn{
					.expr = node_expr.value()
				};
			
			}else{
				std::cerr << "Invalid Expression" << std::endl;
				exit(EXIT_FAILURE);
			}
			if(peek().has_value() && peek().value().type == TokenType::semi){
				consume();
			}else{
				std::cerr << "Invalid Expression" << std::endl;
				exit(EXIT_FAILURE);
			}
		}
	}
	m_ind = 0;
	return node_return;
	}
