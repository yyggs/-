#ifndef CHECK_H
#define CHECK_H
#include <QString>
#include <vector>
#include "ast.h"



class check
{
public:
    enum Kind{
        FLEXERR,
        NAMEERR,
        SENTENCEERR,
        CIRERR,
        VARERR,
        FUNERR,
    };

    int position;
    Kind kind;
    check(Kind _kind, int pos): kind(_kind), position(pos){}

};

////小驼峰命名法
bool NameCheck(QString str);

#endif // CHECK_H
