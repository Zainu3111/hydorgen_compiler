#ifndef NODES
#define NODES
#include "tokenizer.h"
#include <optional>
#include <variant>

namespace node {
	struct exprIntLit{
		Token int_lit;
	};

	struct exprIdent{
		Token ident;
	};

	struct expr{
		std::variant<exprIntLit, exprIdent> var;
	};

	struct statementReturn{
		expr expr;
	};

	struct statementDeclaration{
		Token ident;
		expr expr;
	};

	struct statement{
		std::variant<statementReturn, statementDeclaration> stmt;
	};

	struct prog{
		std::vector<statement> stmts;
	};
}

#endif
