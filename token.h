#ifndef TOKEN
#define TOKEN

#include <QString>
class token
{
public:
    int kind;
    int position;     //第几个token
    int length;
    int line;         //

    QString string;

//    union {
//        int matchingBrace;
//        int i; // integer value
//        QString *string; // string value
//        void *ptr;
//    };

    token()
        : kind(0), position(0), length(0), line(0), string() {}

    bool is(int k) const { return k == kind; }
    bool isNot(int k) const { return k != kind; }

    int begin() const { return position; }
    int end() const { return position + length; }
};




#endif // TOKEN

