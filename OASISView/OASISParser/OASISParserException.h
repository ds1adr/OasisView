#ifndef OASISPARSEREXCEPTION_H
#define OASISPARSEREXCEPTION_H

#include <exception>

namespace OASISParser {

class ParserException: public std::exception {
    virtual const char* description() const throw() = 0;
};

class InvalidIntegerLength: public ParserException {
    const char* description() const throw() {
        return "Integer Length exceed its lengths.";
    }
};

class InvalidString: public ParserException {
    const char* description() const throw() {
        return "Invalid String Value.";
    }
};

}

#endif // OASISPARSEREXCEPTION_H
