#include "codegen.h"
#include <string>
#include <sstream>
Generator::Generator(node::prog prog)
	: m_prog(std::move(prog))
{

}

void Generator::gen_expr(const node::expr& expr){
	// declaring inside the function keeps the visitor from polluting the outside scope.
	struct ExprVisitor {
		Generator* gen;

		void operator()(const node::exprIntLit& expr_int_lit){
			gen->m_output << "		li t0, " << expr_int_lit.int_lit.value.value() << "\n";
			gen->m_output << "		sw t0, 0(sp)\n";
			gen->m_output << "		addi sp, sp, -8\n";
//			std::cout << "it should work" << std::endl;
		}
		void operator()(const node::exprIdent& expr_iden){
		//TODO	
		}
	};
	ExprVisitor visitor{.gen = this};
	std::visit(visitor, expr.var);
}

void Generator::gen_statement(const node::statement& stmt){
	// declaring inside the function keeps the visitor from polluting the outside scope.
	struct StmtVisitor {
		Generator* gen;

		void operator()(const node::statementReturn& stmt_ret){
			gen->gen_expr(stmt_ret.expression);
			gen->m_output << "		addi sp, sp, 8\n";
			gen->m_output << "		lw a0, 0(sp)\n";
			gen->m_output << "		li a7, 1\n";
			gen->m_output << "		ecall\n";
		}

		void operator()(const node::statementDeclaration& stmt_dec){
			//TODO add functionality
		}
	};
	StmtVisitor visitor{.gen = this};
	std::visit(visitor, stmt.stmt);

}

std::string Generator::gen_prog(){
	m_output << "		.global main\nmain:\n";
	
//	std::cout << "start codegen" << std::endl;

	for(const node::statement stmt : m_prog.stmts){
		gen_statement(stmt);
//	std::cout << "calling gen_statement" << std::endl;
	}
	return m_output.str();
}
