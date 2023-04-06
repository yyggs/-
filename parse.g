%decl parser.h
%impl parser.cpp
%parser ParserTable
%token_prefix T_


%token LEFT "{"
%token RIGHT "}"
%token SLEFT "("
%token SRIGHT ")"
%token NUM
%token VAR
%token EQUAL "="
%token ADD "+"
%token SUB "-"
%token LOOP
%token LOOPEND

%start document


/:
#ifndef PARSER_H
#define PARSER_H
#include <vector>
#include <stack>
#include <QString.h>
#include "parsertable_p.h"
#include "token.h"
#include "ast.h"
#include "check.h"

class Parser: public $table
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
:/

/.  
#include <iostream>
#include <cstdio>
#include <cassert>
#include <QDebug>
#include "Parser.h"
#include <qdebug.h>
#include "lex.yy.c"

extern std::vector<check> errlist;
extern bool sentenceCheck(sentenceast* sen);
extern void varCheck(documentast* tree);
extern void printerrlist();

Parser::Parser()
	: _tos(-1), _index(0), yyloc(-1), yytoken(-1), yyrecovering(0), _recovered(false)
{
    yyin=fopen("test.txt","r");
    _tokens.reserve(1024);

    _state_stack.resize(128);
    _location_stack.resize(128);
    _symbol_stack.resize(128);

    _tokens.push_back(token()); // invalid token
    while (tk.kind != T_RIGHT)
    {
        yylex();
        tk.position += 1;
        if(tk.kind == T_VAR && NameCheck(tk.string) == false){
            //check(check::NAMEERR, tk.position);
            //qDebug() << tk.string;
            errlist.push_back(check(check::NAMEERR, tk.position));
        }
        _tokens.push_back(tk);
        //qDebug() << tk.string;
    } ;
//    qDebug() << _tokens.size();
//    int t = 1;
//    while(t < 19){
//        qDebug() << _tokens.at(t).string;
//        t++;
//    }
    _index = 1;
}

Parser::~Parser()
{
}
 
void Parser::parse(int startToken)
{
    int action = 0;
    yytoken = -1;
    yyloc = -1;
    void *yyval = 0; // value of the current token.

    _recovered = false;
    _tos = -1;
    _startToken.kind = startToken;

    do {
        if (unsigned(++_tos) == _state_stack.size()) {
            _state_stack.resize(_tos * 2);
            _location_stack.resize(_tos * 2);
            _symbol_stack.resize(_tos * 2);
        }

        _state_stack[_tos] = action;

        if (yytoken == -1 && -TERMINAL_COUNT != action_index[action]) {
            yyloc = consumeToken();
            yytoken = tokenKind(yyloc);
            if (yyrecovering)
                --yyrecovering;

            yyval = &_tokens.at(yyloc).string;
            //qDebug() << _tokens.at(yyloc).string;
            //qDebug() << _tokens.at(yyloc).string;
            //printf("%d\n", yyloc);
        }

        action = t_action(action, yytoken);
        if (action > 0) {
            if (action == ACCEPT_STATE) {
                --_tos;
                return;
            }
            _symbol_stack[_tos].ptr = yyval;
            //qDebug() << *_symbol_stack[_tos].string;
            //printf("!!%s!!\n",_symbol_stack[_tos].str);
            _location_stack[_tos] = yyloc;
            yytoken = -1;
        } else if (action < 0) {
            const int ruleno = -action - 1;
            const int N = rhs[ruleno];
            _tos -= N;
			//qDebug() << *_symbol_stack[_tos].string;
            reduce(ruleno);
            action = nt_action(_state_stack[_tos], lhs[ruleno] - TERMINAL_COUNT);
        } else if (action == 0) {
            break;
        }
    } while (action);

    //return 0;
}
./




/.
void Parser::reduce(int ruleno){

switch(ruleno){
./


document ::= function;
/.
case $rule_number: {
    qDebug() << "document ::= function";
	tree = makeAstNode<documentast>(function(1));
    astnode(1) = tree ;
    //printerrlist();
    //varCheck(tree);
    //document(1)->printstring();


}   break;
./

document ::= userfunction document;
/.
case $rule_number: {
    qDebug() << "ducument ::= userfunction document";
    tree = makeAstNode<documentast>(userfunction(1), document(2));
    astnode(1) = tree ;
    //printerrlist();
    varCheck(tree);
    //document(1)->printstring();


}   break;
./




userfunction ::= call SLEFT Sentences SRIGHT;
/.
case $rule_number: {
    qDebug() << "userfunction ::= call SLEFT Sentences SRIGHT;";
    astnode(1) = makeAstNode<userfunctionast>(call(1), sentences(3));
    //userfunction(1)->printstring();
}   break;
./


function ::= LEFT Sentences RIGHT;
/.
case $rule_number: {
    qDebug() << "function ::= LEFT Sentences RIGHT";
    astnode(1) = makeAstNode<functionast>(sentences(2));
	//function(1)->printstring();
}   break;
./

Sentences ::= Sentence Sentences;
/.
case $rule_number: {
    qDebug() << "Sentences ::= Sentence Sentences";
    astnode(1) = makeAstNode<sentencesast>(sentence(1), sentences(2));
	//sentences(1)->printstring();
}   break;
./

Sentences ::= call Sentences;
/.
case $rule_number: {
    qDebug() << "Sentences ::= call sentences";
    astnode(1) = makeAstNode<sentencesast>(call(1), sentences(2));
    //sentences(1)->printstring();
}   break;
./

Sentences ::= Loop Sentences;
/.
case $rule_number: {
    qDebug() << "Sentences ::= Loop Sentences";
    astnode(1) = makeAstNode<sentencesast>(lo(1), sentences(2));
    //sentences(1)->printstring();
}   break;
./


Loop ::= LOOP numvar Sentences LOOPEND;
/.
case $rule_number: {
    qDebug() << "Loop ::= LOOP Sentences LOOPEND";
    astnode(1) = makeAstNode<loopast>(sentences(3));
    if(lo(1)->lines > 5){
        errlist.push_back(check(check::CIRERR, lo(1)->sens->sen->position));
    }
    //lo(1)->printstring();
}   break;
./

Sentences ::= Sentence;
/.
case $rule_number: {
    qDebug() << "Sentences ::= Sentence";
    astnode(1) = makeAstNode<sentencesast>(sentence(1));
	//sentences(1)->printstring();
}   break;
./

Sentence ::= VAR EQUAL Expression;
/.
case $rule_number: {
    qDebug() << "Sentence ::= VAR EQUAL Expression";
    astnode(1) = makeAstNode<sentenceast>(string(1), expression(3), location(1));
    //sentence(1)->printstring();
    if(sentenceCheck(sentence(1)) == false){
        //int p = sentence(1)->position;
        //QString a = *sentence(1)->var;
        errlist.push_back(check(check::SENTENCEERR, sentence(1)->position));
    }
    //sentence(1)->printstring();
}   break;
./

call ::= VAR SLEFT Parmlist SRIGHT;
/.
case $rule_number: {
    qDebug() << "call ::= VAR SLEFT Parmlist SRIGGHT";
    astnode(1) = makeAstNode<callast>(string(1), parm(3));
    parmast* pa = parm(3);
    int panum = 0;
    while(pa){
        panum++;
        pa = pa->parm;
    }
    if(panum > 3){
        errlist.push_back(check(check::FUNERR, location(1)));
    }
    //qDebug() << location(1);
    //qDebug() << panum << "@@@@";
    //parmlist(1)->printstring();
    //qDebug() << *string(1);
}   break;
./

Parmlist ::= VAR Parmlist;
/.
case $rule_number: {
    qDebug() << "Parmlist ::= VAR Parmlist";
    astnode(1) = makeAstNode<parmast>(string(1), parm(2));
    //parmlist(1)->printstring();
    //qDebug() << *string(1);
}   break;
./

Parmlist ::= VAR;
/.
case $rule_number: {
    qDebug() << "Parmlist ::= VAR";
    astnode(1) = makeAstNode<parmast>(string(1));
    //parmlist(1)->printstring();
    //qDebug() << *string(1);
}   break;
./

Expression ::= VAR;
/.
case $rule_number: {
    qDebug() << "Expression ::= VAR";
    astnode(1) = makeAstNode<expressionast>(string(1), location(1));
	//expression(1)->printstring();
    //qDebug() << *string(1);
}   break;
./


Expression ::= NUM;
/.
case $rule_number: {
    qDebug() << "Expression ::= NUM";
    astnode(1) = makeAstNode<expressionast>(string(1), location(1));
	//expression(1)->printstring();
}   break;
./

Expression ::= Expression Operator Expression;
/.
case $rule_number: {
    qDebug() << "Expression ::= Expression Operator Expression";
    astnode(1) = makeAstNode<expressionast>(expression(1), op(2), expression(3));
	//expression(1)->printstring();
}   break;
./

Operator ::= ADD;
/.
case $rule_number: {
    qDebug() << "Operator ::= ADD";
    astnode(1) = makeAstNode<opast>(string(1));
	//op(1)->printstring();
    //qDebug() << *string(1);
}   break;
./

Operator ::= SUB;
/.
case $rule_number: {
    qDebug() << "Operator ::= SUB";
    astnode(1) = makeAstNode<opast>(string(1));
	//op(1)->printstring();
}   break;
./

numvar ::= NUM;
/.
case $rule_number: {
    qDebug() << "numvar ::= NUM";
    astnode(1) = makeAstNode<numvarast>(string(1));
    //numvar(1)->printstring();
}   break;
./


numvar ::= VAR;
/.
case $rule_number: {
    qDebug() << "numvar ::= VAR";
    astnode(1) = makeAstNode<numvarast>(string(1));
    //numvar(1)->printstring();
}   break;
./

/.
}
}
./
