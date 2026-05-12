#include "codegen.h"
#include <string>
#include <sstream>

Generator::Generator(node::prog prog)
	: m_prog(std::move(prog))
{

}

void Generator::push(const std::string& reg){ 
	m_output << "		sw " << reg << ", 0(sp)\n"; 
	m_output << "		addi sp, sp, -8\n";
	++m_stack_size;
}

void Generator::pop(const std::string& reg){
	m_output << "		addi sp, sp, 8\n";
	m_output << "		lw " << reg << ", 0(sp)\n";
	--m_stack_size;
}

void Generator::gen_expr(const node::expr& expr){
	// declaring inside the function keeps the visitor from polluting the outside scope.
	struct ExprVisitor {
		Generator* gen;

		void operator()(const node::exprIntLit& expr_int_lit){
			gen->m_output << "		li t0, " << expr_int_lit.int_lit.value.value() << "\n";
			gen->push("t0");
//			std::cout << "it should work" << std::endl;
		}
		void operator()(const node::exprIdent& expr_iden){
		//TODO	
			if (!gen->m_vars.contains(expr_iden.ident.value.value())){
				std::cerr << "Variable " << expr_iden.ident.value.value() << " does not exit." << std::endl;
				exit(EXIT_FAILURE);
			}
			int loc = gen->m_stack_size - gen->m_vars.at(expr_iden.ident.value.value()).stack_location;
			loc = loc * 8;
			gen->m_output << "		lw t0, " << std::to_string(loc) << "(sp)\n";
			gen->push("t0");
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
			gen->pop("a0");
			gen->m_output << "		li a7, 1\n";
			gen->m_output << "		ecall\n";
		}

		void operator()(const node::statementDeclaration& stmt_dec){
			//check first if variable already declared and only declare it if not
			if (gen->m_vars.contains(stmt_dec.ident.value.value())){
				std::cerr << "Identifier already declared: " << stmt_dec.ident.value.value() << std::endl;
				exit(EXIT_FAILURE);
			}
			gen->m_vars.insert({stmt_dec.ident.value.value(), Var {.stack_location = gen->m_stack_size}});
			gen->gen_expr(stmt_dec.expression);
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
