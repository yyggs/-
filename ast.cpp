#include "ast.h"

sentencesast::sentencesast(loopast* _lo, sentencesast* _sens):ast(Sentences), lo(_lo), sens(_sens), sen(nullptr), call(nullptr){lines = _lo->lines + _sens->lines;}
sentencesast::sentencesast(callast* _call, sentencesast* _sens):ast(Sentences), call(_call), sens(_sens), sen(nullptr), lo(nullptr){lines = call->lines + _sens->lines;}
void sentencesast::printstring(){
    if(lo != nullptr){
        lo->printstring();
    }else if(call != nullptr){
        call->printstring();
    }else{
        if(sen != nullptr)
        sen->printstring();
    }
    if(sens != nullptr)
    sens->printstring();
}

void documentast::printstring(){
    if(userfun){
        userfun->printstring();
        doc->printstring();
    }else{
        //qDebug() << "11";
        fun->printstring();
    }


}
