#ifndef HYDROGEN_CODEGEN_H
#define HYDROGEN_CODEGEN_H

#include <iostream>
#include "parser.h"
#include "node.h"
#include <sstream>
class Generator{
	public:
		Generator(node::prog);

		[[nodiscard]] std::string gen_prog ();

		void gen_statement (const node::statement&);

		void gen_expr(const node::expr&);
	private:
		const node::prog m_prog;
		std::stringstream m_output;

};

#endif
