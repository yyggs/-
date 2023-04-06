#ifndef AST_H
#define AST_H
#include <QString>、
#include <QDebug>

class ast
{
public:
    enum Kind{
        Document,
        Userfunction,
        Function,
        Sentences,
        Sentence,
        Parm,
        Expression,
        Op,
        Nv,
        Loop,
        Call,
    };
    int kind;
    int lines;      //节点包含多少行
    ast(){}
    ast(Kind _kind) : kind(_kind), lines(0) {}

    void printstring();

};

class numvarast : public ast{
public:
    numvarast(const QString* _nv):ast(Nv), numvar(_nv){}
    const QString* numvar;
    int position;
    void printstring(){
        qDebug() << *numvar;
    }
};

class opast : public ast{
public:
    opast(const QString* _op):ast(Op), op(_op){}
    const QString* op;
    int position;
    void printstring(){
        qDebug() << *op;
    }
};

class expressionast : public ast{
public:
    expressionast(const QString* var, int pos):ast(Expression), numvar(var), position(pos){leftexp = nullptr; rightexp = nullptr; op = nullptr;}
    expressionast(expressionast* left, opast* opt, expressionast* right):ast(Expression), leftexp(left), rightexp(right), op(opt){numvar = nullptr;}
    expressionast* leftexp;
    opast* op;
    expressionast* rightexp;
    const QString* numvar;
    int position;
    void printstring(){
        if(numvar == nullptr){
            leftexp->printstring();
            op->printstring();
            rightexp->printstring();

        }else{
            qDebug() << *numvar;
        }
    }

    int exlenth(){
        if(numvar == nullptr){
            return 1 + leftexp->exlenth() + rightexp->exlenth();
        }else{
            return numvar->size();
        }
        return 0;
    }
};

class parmast : public ast{
public:
    parmast(const QString* _var):ast(Parm), var(_var), parm(nullptr){}
    parmast(const QString *_var, parmast* _parm):ast(Parm), var(_var), parm(_parm){}
    parmast* parm;
    const QString* var;
    void printstring(){
        qDebug() << *var;
        if(parm){
            qDebug() << ",";
            parm->printstring();
        }
    }
};

class callast : public ast{
public:
    callast(const QString *_var, parmast* _parm):ast(Call), var(_var), parm(_parm){}
    const QString* var;
    parmast* parm;
    void printstring(){
        qDebug() << *var;
        qDebug() << "(";
        parm->printstring();
        qDebug() << ")";
    }
};

class sentenceast : public ast{
public:
    sentenceast(const QString* _var, expressionast* _exp, int pos):ast(Sentence), var(_var), exp(_exp), position(pos){lines = 1;}
    const QString* var;
    int position;
    expressionast* exp;
    void printstring(){
        qDebug() << *var + "=";
        exp->printstring();
    }
};

class loopast;

class sentencesast : public ast{
public:
    sentencesast(sentenceast* _sen):ast(Sentences), sen(_sen), lo(nullptr), call(nullptr){sens = nullptr; lines = 1;}
    sentencesast(sentenceast* _sen, sentencesast* _sens):ast(Sentences), sen(_sen), sens(_sens), lo(nullptr), call(nullptr){lines = _sen->lines + _sens->lines;}
    sentencesast(loopast* _lo, sentencesast* _sens);
    sentencesast(callast*_call, sentencesast* _sens);
    sentencesast* sens;
    sentenceast* sen;
    loopast* lo;
    callast* call;
    void printstring();

};

class loopast : public ast{
public:
    loopast(sentencesast* _sens):ast(Loop), sens(_sens){lines = 2 + sens->lines;}
    sentencesast* sens;
    void printstring(){
        if(sens)
        sens->printstring();
    }
};



class functionast : public ast{
public:
    functionast(sentencesast* _sens):ast(Function), sens(_sens){lines = _sens->lines;}
    sentencesast* sens;
    void printstring(){
        qDebug() << "{";
        sens->printstring();
        qDebug() << "}";
    }
};


class userfunctionast : public ast{
public:
    userfunctionast(callast *_call, sentencesast* _sens):call(_call), ast(Userfunction), sens(_sens){lines = _sens->lines;}
    callast* call;
    sentencesast* sens;
    void printstring(){
        call->printstring();
        qDebug() << "(";
        sens->printstring();
        qDebug() << ")";
    }
};

class documentast : public ast{
public:
    documentast(functionast* _fun):ast(Document), fun(_fun), userfun(nullptr), doc(nullptr){lines = _fun->lines;}
    documentast(userfunctionast* _userfun, documentast* _doc):ast(Document), userfun(_userfun), doc(_doc), fun(nullptr){}
    functionast* fun;
    documentast* doc;
    userfunctionast* userfun;
    void printstring();



};












#endif // AST_H
