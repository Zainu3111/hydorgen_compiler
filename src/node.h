#ifndef NODES
#define NODES
#include "tokenizer.h"
#include <optional>
#include <variant>

namespace node {
	struct termIntLit{
		Token int_lit;
	};

	struct termIdent{
		Token ident;
	};
	
	struct binExpr;

	struct term{
		std::variant<termIntLit*, termIdent*> var;
	};

	struct expr{
		std::variant<term*, binExpr*> var;
	};

	struct binExprAdd{
		expr* left;
		expr* right;
	};

//	struct binExprMult{
//		expr* left;
//		expr* right;
//	};

	struct binExpr{
		binExprAdd* add;
	};

	struct statementReturn{
		expr* expression;
	};

	struct statementDeclaration{
		Token ident;
		expr* expression;
	};

	struct statement{
		std::variant<statementReturn*, statementDeclaration*> stmt;
	};

	struct prog{
		std::vector<statement> stmts;
	};
}

#endif
