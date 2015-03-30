#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <string>

enum Exception {
    NOTCONS,
    BADFORM,
    MISSINGBINDING,
    BADBIND,
    BADARGS,
    INTERNAL_ERROR
};

#endif // EXCEPTION_H
