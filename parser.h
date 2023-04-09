
#ifndef PARSER_H
#define PARSER_H
#include <vector>
#include <stack>
#include <QString.h>
#include "parsertable_p.h"
#include "token.h"
#include "ast.h"
#include "check.h"

class Parser: public ParserTable
{
public:
	union Value{
		void *ptr;
		const QString *string;
		ast *astnode;
		documentast *document;
        userfunctionast *userfunction;
		functionast *function;
		sentencesast *sentences;
		sentenceast	*sentence;
        callast *call;
        parmast *parm;
		expressionast *expression;
        opast *op;
        loopast *lo;
	};
	
	documentast* tree;
    int kind;
    int pos;
	Parser();
	~Parser();
	
	void parse(int startToken);
	
private:
    int _tos;
    int _index;
    int yyloc;
    int yytoken;
    int yyrecovering;
    bool _recovered;
    token _startToken;
    std::vector<int> _state_stack;
    std::vector<int> _location_stack;
    std::vector<Value> _symbol_stack;
    std::vector<token> _tokens;
	
	inline int consumeToken() {
        if (_index < int(_tokens.size()))
            return _index++;
        return static_cast<int>(_tokens.size()) - 1;
    }
    inline const token &tokenAt(int index) const {
        if (index == 0)
            return _startToken;
        return _tokens.at(index);
    }
    inline int tokenKind(int index) const {
        if (index == 0)
            return _startToken.kind;
        return _tokens.at(index).kind;
    }
	
	void reduce(int ruleno);
	
	int &location(int n) { return _location_stack[_tos + n - 1]; }
	Value &sym(int n) { return _symbol_stack[_tos + n - 1]; }
	ast *&astnode(int n) { return _symbol_stack[_tos + n - 1].astnode; }
	const QString *&string(int n) { return _symbol_stack[_tos + n - 1].string; }
	documentast *document(int n) { return _symbol_stack[_tos + n - 1].document; }
    userfunctionast *userfunction(int n) { return _symbol_stack[_tos + n - 1].userfunction; }
	functionast *function(int n) { return _symbol_stack[_tos + n - 1].function; }
	sentencesast *sentences(int n) { return _symbol_stack[_tos + n - 1].sentences; }
	sentenceast *sentence(int n) { return _symbol_stack[_tos + n - 1].sentence; }
    parmast *parm(int n) { return _symbol_stack[_tos + n - 1].parm; }
    callast *call(int n) { return _symbol_stack[_tos + n - 1].call; }
	expressionast *expression(int n) { return _symbol_stack[_tos + n - 1].expression; }
    opast *op(int n) { return _symbol_stack[_tos + n - 1].op; }
    loopast *lo(int n) { return _symbol_stack[_tos + n - 1].lo; }
	
	template <typename T>
    T *makeAstNode()
    {
        T *node = new T ();
        //node->lineno = yyloc >= 1 ? (_tokens[yyloc-1].line + 1) : 0;
        return node;
    }
 
    template <typename T, typename A1>
    T *makeAstNode(A1 a1)
    {
        T *node = new T (a1);
        //node->lineno = yyloc >= 1 ? (_tokens[yyloc-1].line + 1) : 0;
        return node;
    }

    template <typename T, typename A1, typename A2>
    T *makeAstNode(A1 a1, A2 a2)
    {
        T *node = new T (a1, a2);
        //node->lineno = yyloc >= 1 ? (_tokens[yyloc-1].line + 1) : 0;
        return node;
    }

    template <typename T, typename A1, typename A2, typename A3>
    T *makeAstNode(A1 a1, A2 a2, A3 a3)
    {
        T *node = new T (a1, a2, a3);
        //node->lineno = yyloc >= 1 ? (_tokens[yyloc-1].line + 1) : 0;
        return node;
    }
	
	
};
#endif
