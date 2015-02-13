#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <string>

class Exception {
    private:
        std::string error;
        std::string message;
        std::string caller;
    public:
        Exception(const char *caller_, const std::string message_) { message = message_; caller = caller_; }
        std::string GetMessage() { return message; }
        std::string GetCaller() { return caller; }
};

class NotCons : public Exception {
    private:
        std::string error = "Attempted list access on non-list data.";
    public:
        NotCons(const char * caller_, const std::string message_) : Exception(caller_, message_) { }
}; 

class BadForm : public Exception {
    private:
        std::string error = "Badly formed input detected.";
    public:
        BadForm(const char * caller_) : Exception(caller_, "Badly formed expression detected") {}
};

class MissingBinding : public Exception {
    private:
        std::string error = "Unbound symbol";
    public:
        MissingBinding(const char * caller_, const std::string message_) : Exception(caller_, message_) { }
};

#endif // EXCEPTION_H
