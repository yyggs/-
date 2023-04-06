#include "check.h"

std::vector<check> errlist;
bool NameCheck(QString str){
     if(str.size() < 2) return false;
     if(str.at(0) >= 'A' && str.at(0) <= 'Z') return false;
     //if(str.at(str.size() - 1) >= 'A' && atr.at(str.size() - 1) <= 'Z') return false;
     for(int t = 1; t < str.size(); t++){
         if(str.at(t) >= 'A' && str.at(t) <= 'Z') return true;
     }
     return false;
}

bool sentenceCheck(sentenceast* sen){
    int senlen = 0;
    int maxlen = 40;
    senlen = sen->var->size() + 1;
    senlen += sen->exp->exlenth();
    if(senlen > maxlen){
        return false;
    }
    return true;
}


QSet<QString> definition;

void excheck(expressionast* ex){
    if(ex->numvar != nullptr){
        if(!(ex->numvar->at(0) >= '0' && ex->numvar->at(0) <= '9') && !definition.contains(*ex->numvar)){
            errlist.push_back(check(check::VARERR, ex->position));
        }
    }else{
        excheck(ex->leftexp);
        excheck(ex->rightexp);
    }
}

void sensCheck(sentencesast* sens){
    if(sens != nullptr){
        if(sens->sen){
            sentenceast* sen = sens->sen;
            definition << *sen->var;
            expressionast* ex = sen->exp;
            excheck(ex);
            sensCheck(sens->sens);
        }else{
            if(sens->lo)
            sensCheck(sens->lo->sens);
            sensCheck(sens->sens);
        }
    }
}

void varCheck(documentast* tree){
    if(tree->userfun){
        sensCheck(tree->userfun->sens);
        varCheck(tree->doc);
    }else{
        sentencesast* sens = tree->fun->sens;
        sensCheck(sens);
    }
}

void printerrlist(){
    for(int i = 0; i < errlist.size(); i++){
        qDebug() << errlist[i].position << errlist[i].kind;
    }
}




