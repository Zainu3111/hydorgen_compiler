#include <iostream>
#include "codegen.h"
#include <string>
#include <sstream>

Generator::Generator(NodeReturn root)
	: m_root(std::move(root))
{

}

std::string Generator::generate() const {
	std::stringstream output;
	output << "		.global main\nmain:\n";
	output << "		li a0, " << m_root.expr.int_lit.value.value() << "\n";
	output << "		li a7, 1\n";
	output << "		ecall\n";
	output << "		li a0, " << m_root.expr.int_lit.value.value() << "\n";
	output << "		li a7, 10\n";
	output << "		ecall";
	return output.str();
}
