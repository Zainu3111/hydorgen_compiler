#ifndef HYDROGEN_CODEGEN_H
#define HYDROGEN_CODEGEN_H

#include <iostream>
#include "parser.h"
#include "node.h"
class Generator{
	public:
		Generator(node::statementReturn);

		[[nodiscard]] std::string generate () const;
	private:
		const node::statementReturn m_root;

};

#endif
