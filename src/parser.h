#ifndef HYDROGEN_PARSER_H
#define HYDROGEN_PARSER_H

#include <iostream>
#include "tokenizer.h"
#include <optional>
#include <variant>
#include "node.h"


class Parser{
public:
	Parser(std::vector<Token>);

	std::optional<node::expr> parse_expr();

	std::optional<node::statementReturn> parse();


	std::optional<node::statement> parse_statement();
private:

	// consume returns a char at index i and increment it
	Token consume();

	// peek is to check if there is another char and what is it, [[nodiscard]] so we 
	// can get an error in case we accidentally call it and not use it
	[[nodiscard]] std::optional<Token> peek(size_t offset = 0) const;

	const std::vector<Token> m_tokens;
	size_t m_ind{};
	
	bool check(TokenType);
};


#endif
