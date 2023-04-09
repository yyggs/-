%option noyywrap never-interactive
%{
#include "parser.h"
#include <stdio.h>
#include "token.h"
#include <QString>
#include <QDebug>

token tk;
QString string;
%}

loop loop|LOOP
loopend loopend|LOOPEND
left [{]
right [}]
sleft [(]
sright [)]
num [0-9]+
letter [a-zA-Z]
var ({letter}|_)({letter}|_|{num})*
equal [=]
add [+]
sub [-]
err ({num})({letter}|_|{num})*

%%
{loop} {tk.kind = Parser::T_LOOP; tk.string = QString(QLatin1String(yytext));  return 0;}
{loopend} {tk.kind = Parser::T_LOOPEND; tk.string = QString(QLatin1String(yytext));  return 0;}
{left} {tk.kind = Parser::T_LEFT; tk.string = QString(QLatin1String(yytext));  return 0;}
{right} {tk.kind = Parser::T_RIGHT; tk.string = QString(QLatin1String(yytext));  return 0;}
{sleft} {tk.kind = Parser::T_SLEFT; tk.string = QString(QLatin1String(yytext));  return 0;}
{sright} {tk.kind = Parser::T_SRIGHT; tk.string = QString(QLatin1String(yytext));  return 0;}
{num} {tk.kind = Parser::T_NUM; tk.string = QString(QLatin1String(yytext));  return 0;}
{var} {tk.kind = Parser::T_VAR; tk.string = QString(QLatin1String(yytext));  return 0;}
{equal} {tk.kind = Parser::T_EQUAL; tk.string = QString(QLatin1String(yytext));  return 0;}
{add} {tk.kind = Parser::T_ADD; tk.string = QString(QLatin1String(yytext));  return 0;}
{sub} {tk.kind = Parser::T_SUB; tk.string = QString(QLatin1String(yytext));  return 0;}
[" "] {}
.  {qDebug() << "未识别的标识符：" << yytext; return 1;}
%%
