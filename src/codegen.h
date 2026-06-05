#ifndef HYDROGEN_CODEGEN_H
#define HYDROGEN_CODEGEN_H

#include <iostream>
#include "parser.h"
#include "node.h"
#include <sstream>
#include "arena.h"

class Generator{
	public:
		Generator(node::prog);

		std::string gen_prog ();

		void gen_statement (const node::statement&);

		void gen_expr(const node::expr*);
		
		void gen_term(const node::term*);
		
		void gen_bin_expr(const node::binExpr*);

		void gen_scope(const node::scope*);

		void gen_if_pred(const node::ifPred*, const std::string&);
	private:
		std::string create_label();

		void begin_scope();

		void end_scope();

		void push(const std::string&);

		void pop(const std::string&);

		struct Var{
			std::string name;
			size_t stack_location;
		};

		//fields in my Generator
		const node::prog m_prog;
		std::stringstream m_output;
		size_t m_stack_size = 0;
		std::vector<Var> m_vars {};
		std::vector<size_t> m_scopes {};
		int m_label_count = 0;
};

#endif
