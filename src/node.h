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

	struct termParen;

	struct term{
		std::variant<termIntLit*, termIdent*, termParen*> var;
	};

	struct expr{
		std::variant<term*, binExpr*> var;
	};

	struct termParen{
		expr* expression;
	};

	struct binExprAdd{
		expr* left;
		expr* right;
	};

	struct binExprMult{
		expr* left;
		expr* right;
	};

	struct binExprSub{
		expr* left;
		expr* right;
	};
	
	struct binExprDiv{
		expr* left;
		expr* right;
	};

	struct binExpr{
		std::variant<binExprAdd*, binExprMult*, binExprSub*, binExprDiv*> var;
	};

	struct statementReturn{
		expr* expression;
	};

	struct statementDeclaration{
		Token ident;
		expr* expression;
	};

	struct statementScope;

	struct statement{
		std::variant<
			statementReturn*, 
			statementDeclaration*, 
			statementScope*>
				stmt;
	};

	struct prog{
		std::vector<statement> stmts;
	};
	
	struct statementScope{
		std::vector<statement*> stmts;
	};
	
}

#endif
