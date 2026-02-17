#ifndef HYDROGEN_CODEGEN_H
#define HYDROGEN_CODEGEN_H

#include <iostream>
#include "parser.h"
class Generator{
	public:
		Generator(NodeReturn);

		[[nodiscard]] std::string generate () const;
	private:
		const NodeReturn m_root;

};

#endif
