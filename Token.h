#include "DSString.h"

#ifndef TOKEN_H
#define TOKEN_H

struct Token
{
    DSString value;
    bool negated;
};

#endif