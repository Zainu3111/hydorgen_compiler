#include <iostream>
#include "parser.h"
#include <optional>
#include "node.h"

Parser::Parser(std::vector<Token> tokens)
	: m_tokens(std::move(tokens))
	, m_allocator(1024 * 1024 * 4) // 4 mb
{
	
}

std::optional<node::term*> Parser::parse_term(){
	if(check(TokenType::int_lit)){
		auto node_term = m_allocator.alloc<node::term>();
		auto node_term_int_lit = m_allocator.alloc<node::termIntLit>();
		node_term_int_lit->int_lit = consume();	
		node_term->var = node_term_int_lit;
		return node_term;
	}else if(check(TokenType::ident)){
		auto node_term = m_allocator.alloc<node::term>();
		auto node_term_ident = m_allocator.alloc<node::termIdent>();
		node_term_ident->ident = consume();
		node_term->var = node_term_ident;
		return node_term;
	}else{
		return {};
	}
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
	if(auto node_term = parse_term()){
		if(check(TokenType::semi)){
			auto node_expr = m_allocator.alloc<node::expr>();
			node_expr->var = node_term.value();
			return node_expr;
		}else{
			if(check(TokenType::plus)){
				consume();
				if(auto rhs = parse_expr()){
					auto node_bin_expr = m_allocator.alloc<node::binExpr>();
					auto node_bin_expr_plus = m_allocator.alloc<node::binExprAdd>();
					auto lhs = m_allocator.alloc<node::expr>();
					auto node_expr = m_allocator.alloc<node::expr>();
					
					lhs->var = node_term.value();
					node_bin_expr_plus->left = lhs;
					node_bin_expr_plus->right = rhs.value();
					node_bin_expr->var = node_bin_expr_plus;
					node_expr->var = node_bin_expr;
					return node_expr;
				} else {
					std::cerr << "Not a valid Binary Expression" << std::endl;
					exit(EXIT_FAILURE);
				}
			}else if(check(TokenType::mult)){
				//TODO add multiplication parsing;
				return {};
			}else{
				std::cerr << "Unsupported Operator" << std::endl;
				exit(EXIT_FAILURE);
			}
		}
	}
	else{
		return {};
}
}

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
	}else{
		return {};
}
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
	
