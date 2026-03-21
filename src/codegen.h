#ifndef HYDROGEN_CODEGEN_H
#define HYDROGEN_CODEGEN_H

#include <iostream>
#include "parser.h"
#include "node.h"
class Generator{
	public:
		Generator(node::prog);

		[[nodiscard]] std::string gen_prog () const;

		[[nodiscard]] std::string gen_statement (const node::statement) const;
	private:
		const node::prog m_prog;

};

#endif
