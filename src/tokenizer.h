#ifndef HYDEOGEN_TOKENIZATION_HPP
#define HYDROGEN_TOKENIZATION_HPP

#include <vector>
#include <string>
#include <string_view>
#include <optional>
// Tokentype is the type a token is
// will add more types
enum class TokenType {
	_return,
	int_lit,
	semi
};

// Token, what is the type and val of the token
struct Token {
	TokenType type;
 	std::optional<std::string> value {};
};   

class Tokenizer{
public:
	Tokenizer(std::string_view);
	
	// tokenize is a function that is used to tokenize a src string
	std::vector<Token> tokenize();


private:
	// m_src is the line of code being tokenized
	const std::string_view m_src;

	// cur index of char that is being tokenized
	int m_ind{};
	
	// peek is to check if there is another char and what is it, [[nodiscard]] so we 
	// can get an error in case we accidentally call it and not use it
	[[nodiscard]] std::optional<char> peek(int ahead = 1) const;

	// consume returns a char at index i and increment it
	char consume();
};














#endif
