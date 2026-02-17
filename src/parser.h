#ifndef HYDROGEN_PARSER_H
#define HYDROGEN_PARSER_H

#include <iostream>
#include "tokenizer.h"
#include <optional>

struct NodeExpr{
	Token int_lit;
};

struct NodeReturn{
	NodeExpr expr;
};




class Parser{
public:
	Parser(std::vector<Token>);

	std::optional<NodeReturn> parse();

	std::optional<NodeExpr> parse_expr();
private:
	const std::vector<Token> m_tokens;
	size_t m_ind{};

	// peek is to check if there is another char and what is it, [[nodiscard]] so we 
	// can get an error in case we accidentally call it and not use it
	[[nodiscard]] std::optional<Token> peek(size_t ahead = 1) const;

	// consume returns a char at index i and increment it
	Token consume();

};




#endif
