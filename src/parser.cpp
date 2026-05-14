#include <iostream>
#include "parser.h"
#include <optional>
#include "node.h"

Parser::Parser(std::vector<Token> tokens)
	: m_tokens(std::move(tokens))
	, m_allocator(1024 * 1024 * 4) // 4 mb
{
	
}

bool Parser::check(TokenType type){
	return peek().has_value() && peek().value().type == type;
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

std::optional<node::expr*> Parser::parse_expr(){
	if(check(TokenType::int_lit)){
		auto node_expr_int_lit = m_allocator.alloc<node::exprIntLit>();
		node_expr_int_lit->int_lit = consume();	
		auto node_expr = m_allocator.alloc<node::expr>();
		node_expr->var = node_expr_int_lit;
		return node_expr;
	}else if(check(TokenType::ident)){
		auto expr_ident = m_allocator.alloc<node::exprIdent>();
		expr_ident->ident = consume();
		auto node_expr = m_allocator.alloc<node::expr>();
		node_expr->var = expr_ident;
		return node_expr;
	}else{
		return {};
	}
};


std::optional<node::statement> Parser::parse_statement(){
	node::statementReturn stmt_return;
	if(check(TokenType::_return)){
		auto stmt_return = m_allocator.alloc<node::statementReturn>();
		consume();
		// check for an expression
		if (auto node_expr = parse_expr()){
			stmt_return->expression = node_expr.value();
		}else{
			std::cerr << "Not a Valid expression" << std::endl;
			exit(EXIT_FAILURE);
		}
		// check for a semi colon
		if(check(TokenType::semi)){
			consume();
		}else {
			std::cerr << "Requires a semi-colon" << std::endl;
			exit(EXIT_FAILURE);
		}

		return node::statement{.stmt = stmt_return};

	}else if (check(TokenType::_int)){
		consume();
		auto stmt_dec = m_allocator.alloc<node::statementDeclaration>();
		if(check(TokenType::ident)){
			stmt_dec->ident = consume();
		}else{
			std::cerr << "Declaration without identifier" << std::endl;
			exit(EXIT_FAILURE);
		}

		if(check(TokenType::eq)){
			consume();
		}else{
			std::cerr << "Declaration needs an = sign" << std::endl;
			exit(EXIT_FAILURE);
		}
		if(auto node_expr = parse_expr()){
			stmt_dec->expression = node_expr.value();
		}else{
			std::cerr << "Invalid Expression" << std::endl;
			exit(EXIT_FAILURE);
		}

		if(check(TokenType::semi)){
			consume();
		}else {
			std::cerr << "Requires a semi-colon" << std::endl;
			exit(EXIT_FAILURE);
		}

		return node::statement{.stmt = stmt_dec};
	}
	return {};
}

std::optional<node::prog> Parser::parse_prog(){
	//TODO: add functionality
	node::prog prog;
	while (peek().has_value()){
		if (auto stmt = parse_statement()){
			prog.stmts.push_back(stmt.value());
		} else {
			std::cerr << "Invalid Statement expression" << std::endl;
			exit(EXIT_FAILURE);
		}
	}
	return prog;
}
	
