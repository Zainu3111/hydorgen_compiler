#include <iostream>
#include "parser.h"
#include <optional>
#include "node.h"
#include "tokenizer.h"

Parser::Parser(std::vector<Token> tokens)
	: m_tokens(std::move(tokens))
	, m_allocator(1024 * 1024 * 4) // 4 mb
{
	
}

Token Parser::try_consume(TokenType type, std::string err){
	if(check(type)){
		return consume();
	}else{
		std::cerr << err << std::endl;
		exit(EXIT_FAILURE);
	}
}

bool Parser::try_consume(TokenType type){
	if(check(type)){
		consume();
		return true;
	}else{
		return false;
	}
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
	}else if(check(TokenType::open_paren)){
		consume();
		auto expr = parse_expr();
		if(!expr.has_value()){
			std::cerr << "Expected expression." << std::endl;
			exit(EXIT_FAILURE);
		}
		if(!check(TokenType::close_paren)){
			std::cerr << "Expected Close Paranthesis" << std::endl;
		}
		consume();
		auto term = m_allocator.alloc<node::term>();
		auto term_paren = m_allocator.alloc<node::termParen>();
		term_paren->expression = expr.value();
		term->var = term_paren;
		return term;
	}else{
		return {};
	}
}


bool Parser::check(TokenType type, size_t offset){
	return peek(offset).has_value() && peek(offset).value().type == type;
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

std::optional<node::expr*> Parser::parse_expr(int min_prec){
	std::optional<node::term*> lhs = parse_term();
	if (!lhs.has_value()){
		return {};
	}
	auto expr_lhs = m_allocator.alloc<node::expr>(); 
	expr_lhs->var = lhs.value();

	while(1){
		std::optional<Token> cur_token = peek();
		std::optional<int> prec;
		if(cur_token.has_value()){
			prec = bin_prec(cur_token->type);
			if (!prec.has_value() || prec.value() < min_prec){
				break;
			}
		}else{
			break;
		}
		Token op = consume();
		int next_min_prec = prec.value() + 1;
		auto expr_rhs = parse_expr(next_min_prec);
		if(!expr_rhs.has_value()){
			std::cerr << "Bin op needs one more arg" << std::endl;
			exit(EXIT_FAILURE);
		}
		auto expr = m_allocator.alloc<node::binExpr>();
		if(op.type == TokenType::plus){
			auto expr_add = m_allocator.alloc<node::binExprAdd>();
			auto expr_lhs2 = m_allocator.alloc<node::expr>();
			expr_lhs2->var = expr_lhs->var;
			expr_add->left = expr_lhs2;
			expr_add->right = expr_rhs.value();
			expr->var = expr_add;
		}else if (op.type == TokenType::mult){
			auto expr_mult = m_allocator.alloc<node::binExprMult>();
			auto expr_lhs2 = m_allocator.alloc<node::expr>();
			expr_lhs2->var = expr_lhs->var;
			expr_mult->left = expr_lhs2;
			expr_mult->right = expr_rhs.value();
			expr->var = expr_mult;
		}else if (op.type == TokenType::minus){
			auto expr_sub = m_allocator.alloc<node::binExprSub>();
			auto expr_lhs2 = m_allocator.alloc<node::expr>();
			expr_lhs2->var = expr_lhs->var;
			expr_sub->left = expr_lhs2;
			expr_sub->right = expr_rhs.value();
			expr->var = expr_sub;
		}else if (op.type == TokenType::div){
			auto expr_div = m_allocator.alloc<node::binExprDiv>();
			auto expr_lhs2 = m_allocator.alloc<node::expr>();
			expr_lhs2->var = expr_lhs->var;
			expr_div->left = expr_lhs2;
			expr_div->right = expr_rhs.value();
			expr->var = expr_div;
		}else{
			std::cerr << "Not a valid op" << std::endl;
			exit(EXIT_FAILURE);
		}
		expr_lhs->var = expr;
	}
	return expr_lhs;

}

std::optional<node::scope*> Parser::parse_scope(){
	if(!check(TokenType::open_curly)){
		return {};
	}
	consume();
	auto scope = m_allocator.alloc<node::scope>();
	while((!check(TokenType::close_curly))){
		auto stmt = parse_statement();
		scope->stmts.push_back(stmt.value());
	}
	if(!check(TokenType::close_curly)){
		std::cerr << "Need a Closing Curly." << std::endl;
		exit(EXIT_FAILURE);
	}
	consume();
	return scope;
}

std::optional<node::ifPred*> Parser::parse_ifPred(){
	//check and consume else if
	if(try_consume(TokenType::_else) && try_consume(TokenType::_if)){
		//TODO else if parsing
		auto node_elif = m_allocator.alloc<node::statementElseIf>();
		try_consume(TokenType::open_paren, "Expected Open Paren (.");
		if (auto node_expression = parse_expr()){
			node_elif->expression = node_expression.value();
		}else{
			std::cerr << "Expected Expression" << std::endl;
			exit(EXIT_FAILURE);
		}

		//check for )
		try_consume(TokenType::close_paren, "Expected closing Paren");
	
	}else if(try_consume(TokenType::_else)){
		//TODO else parsing
	}else{
		return {};
	}
}

std::optional<node::statement*> Parser::parse_statement(){
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
		auto stmt = m_allocator.alloc<node::statement>();
		stmt->stmt = stmt_return;
		return stmt;

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
		auto stmt = m_allocator.alloc<node::statement>();
		stmt->stmt = stmt_dec;
		return stmt;
	}else if(check(TokenType::open_curly)){
		auto scope = parse_scope();
		auto stmt = m_allocator.alloc<node::statement>();
		stmt->stmt = scope.value();
		return stmt;
	}else if(check(TokenType::_if)){
		consume();
		if (check(TokenType::open_paren)){
			consume();
			auto stmtIf = m_allocator.alloc<node::statementIf>();
			if(auto expr = parse_expr()){
				stmtIf->expression = expr.value();
			}else{
				std::cerr << "Invalid If Statement" << std::endl;
			}
			consume();
			if(auto scope = parse_scope()){
				stmtIf->stmts = scope.value();
				stmtIf->pred = parse_ifPred();
				auto stmt = m_allocator.alloc<node::statement>();
				stmt->stmt = stmtIf;
				std::cout << "parsed node_if" << std::endl;
				return stmt;
			}else{
				std::cerr << "Invalid Scopesss" << std::endl;
				exit(EXIT_FAILURE);
			}
		}else{
			std::cerr << "Expected Open Parantheisis for if statement" << std::endl;
			exit(EXIT_FAILURE);
		}
	}else{
		return {};
	}
}

std::optional<node::prog> Parser::parse_prog(){
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
	
