#ifndef HYDROGEN_CODEGEN_H
#define HYDROGEN_CODEGEN_H

#include <iostream>
#include "parser.h"
#include "node.h"
#include <sstream>
#include <unordered_map>
class Generator{
	public:
		Generator(node::prog);

		std::string gen_prog ();

		void gen_statement (const node::statement&);

		void gen_expr(const node::expr&);
	private:

		void push(const std::string&);

		void pop(const std::string&);

		struct Var{
			size_t stack_location;
		};

		const node::prog m_prog;
		std::stringstream m_output;
		size_t m_stack_size = 0;
		std::unordered_map<std::string, Var> m_vars {};

};

#endif
