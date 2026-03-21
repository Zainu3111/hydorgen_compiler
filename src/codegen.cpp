#include "codegen.h"
#include <string>
#include <sstream>
Generator::Generator(node::prog prog)
	: m_prog(std::move(prog))
{

}


std::string Generator::gen_statement(const node::statement) const {
	
}

std::string Generator::gen_prog() const {
	std::stringstream output;
	output << "		.global main\nmain:\n";
	
	for(const node::statement stmt : m_prog.stmts){
		output << gen_statement(stmt);
	}




	output << "		li a0, 0\n";
	output << "		li a7, 1\n";
	output << "		ecall\n";
	output << "		li a0, 0\n";
	output << "		li a7, 10\n";
	output << "		ecall";
	return output.str();
}
