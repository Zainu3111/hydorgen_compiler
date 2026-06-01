#include "codegen.h"
#include <string>
#include <sstream>
#include <assert.h>
#include <algorithm>

Generator::Generator(node::prog prog)
	: m_prog(std::move(prog))
{

}

// Start Private Functions

std::string Generator::create_label(){
	std::stringstream ss;
	ss << "Label" << m_label_count++;
	return ss.str();
}

void Generator::push(const std::string& reg){
//	m_output << "pushing\n";
	m_output << "		sw " << reg << ", 0(sp)\n"; 
	m_output << "		addi sp, sp, -8\n";
	++m_stack_size;
}

void Generator::pop(const std::string& reg){
//	m_output << "popping\n";
	m_output << "		addi sp, sp, 8\n";
	m_output << "		lw " << reg << ", 0(sp)\n\n";
	--m_stack_size;
}

void Generator::begin_scope(){
	//std::cout << m_vars.size() << std::endl;
	m_scopes.push_back(m_vars.size());
}

void Generator::end_scope(){
	size_t pop_count = m_vars.size() - m_scopes.back();
	m_output << "		addi sp, sp, " << pop_count * 8 << "\n";
	m_stack_size -= pop_count;
	for(size_t i = 0; i < pop_count; ++i){
		m_vars.pop_back();
	}
	m_scopes.pop_back();
}

// End Private Functions

void Generator::gen_term(const node::term* node_term){
	struct TermVisitor{
		Generator* gen;
		void operator()(const node::termIntLit* term_int_lit){
			gen->m_output << "		li t0, " << term_int_lit->int_lit.value.value() << "\n";
			gen->push("t0");
		}
		void operator()(const node::termIdent* term_ident){
			auto it = std::find_if(gen->m_vars.cbegin(), gen->m_vars.cend(), [&](const Var& var){
						return var.name == term_ident->ident.value.value();
					});

			if (it == gen->m_vars.cend()){
				std::cerr << "Variable " << term_ident->ident.value.value() << " does not exit." << std::endl;
				exit(EXIT_FAILURE);
			}
			int loc = (gen->m_stack_size - it->stack_location) * 8;
			gen->m_output << "		lw t0, " << std::to_string(loc) << "(sp)\n";
			gen->push("t0");
		}
		void operator()(const node::termParen* term_paren){
			gen->gen_expr(term_paren->expression);
		}
	};
	TermVisitor visitor({.gen = this});
	std::visit(visitor, node_term->var);
}


void Generator::gen_bin_expr(const node::binExpr* bin_expr){
	struct BinExprVisitor{
		Generator* gen;
		
		void operator()(const node::binExprSub* bin_expr_sub){
			gen->gen_expr(bin_expr_sub->left);
			gen->gen_expr(bin_expr_sub->right);
			gen->pop("t2");
			gen->pop("t1");
			gen->m_output << "		sub t3, t1, t2\n";
			gen->push("t3");
		}
		void operator()(const node::binExprDiv* bin_expr_div){
			gen->gen_expr(bin_expr_div->left);
			gen->gen_expr(bin_expr_div->right);
			gen->pop("t2");
			gen->pop("t1");
			gen->m_output << "		div t3, t1, t2\n";
			gen->push("t3");
		}
		void operator()(const node::binExprAdd* bin_expr_add){
			gen->gen_expr(bin_expr_add->left);
			gen->gen_expr(bin_expr_add->right);
			gen->pop("t1");
			gen->pop("t2");
			gen->m_output << "		add t3, t1, t2\n";
			gen->push("t3");
		}
		void operator()(const node::binExprMult* bin_expr_mult){
			gen->gen_expr(bin_expr_mult->left);
			gen->gen_expr(bin_expr_mult->right);
			gen->pop("t1");
			gen->pop("t2");
			gen->m_output << "		mul t3, t1, t2\n";
			gen->push("t3");

		}
	};
	BinExprVisitor visitor{.gen = this};
	std::visit(visitor, bin_expr->var);
}



void Generator::gen_expr(const node::expr* expr){
	// declaring inside the function keeps the visiitor from polluting the outside scope.
	struct ExprVisitor {
		Generator* gen;

		void operator()(const node::term* node_term){
			gen->gen_term(node_term);
		}
		void operator()(const node::binExpr* node_bin_expr){
			gen->gen_bin_expr(node_bin_expr);
		}
	};
	ExprVisitor visitor{.gen = this};
	std::visit(visitor, expr->var);
}

void Generator::gen_scope(const node::scope* stmt_scope){
			//We will have 2 funcs, begin and end to start a scope and end a scope. 
			//Initially it will produce static assembly but later we will be adding
			//features that save the registers according to calling conventions.
			begin_scope();
			for(node::statement* stmt : stmt_scope->stmts){
				gen_statement(*stmt);
			}
			end_scope();
}

void Generator::gen_statement(const node::statement& stmt){
	// declaring inside the function keeps the visitor from polluting the outside scope.
	struct StmtVisitor {
		Generator* gen;

		void operator()(const node::statementReturn* stmt_ret){
			gen->gen_expr(stmt_ret->expression);
			gen->pop("a0");
			gen->m_output << "		li a7, 1\n";
			gen->m_output << "		ecall\n";
			gen->m_output << "		li a7, 93\n";
			gen->m_output << "		ecall\n";
		}

		void operator()(const node::statementDeclaration* stmt_dec){
			//check first if variable already declared and only declare it if not
			auto it = std::find_if(gen->m_vars.cbegin(), gen->m_vars.cend(), [&](const Var& var){
						return var.name == stmt_dec->ident.value.value();
					});
			if (it != gen->m_vars.cend()){
				std::cerr << "Identifier already declared: " << stmt_dec->ident.value.value() << std::endl;
				exit(EXIT_FAILURE);
			}
			gen->m_vars.push_back({.name = stmt_dec->ident.value.value(), .stack_location = gen->m_stack_size});
			gen->gen_expr(stmt_dec->expression);
		}
		void operator()(const node::scope* scope){
			gen->gen_scope(scope);
		}
		void operator()(const node::statementIf* stmt_if){
			gen->gen_expr(stmt_if->expression);
			gen->pop("t0");
			std::string label = gen->create_label();
			gen->m_output << "		beqz t0, " << label << "\n";
			gen->gen_scope(stmt_if->stmts);
			gen->m_output << label << ":\n";
		}
	};
	StmtVisitor visitor{.gen = this};
	std::visit(visitor, stmt.stmt);

}

std::string Generator::gen_prog(){
	m_output << "		.global main\nmain:\n";
	
//	std::cout << "start codegen" << std::endl;

	for(const node::statement* stmt : m_prog.stmts){
		gen_statement(*stmt);
//	std::cout << "calling gen_statement" << std::endl;
	}
	return m_output.str();
}
