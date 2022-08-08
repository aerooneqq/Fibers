#include "util.h"

class Stack
{
private:
    char* myStack{nullptr};
    size_t mySize{0};

public:
    explicit Stack(size_t size);
    ~Stack();

    int64_t MaterializeStackPointer();
};

class ExecutionContext {
private:
    RegisterContext* myRegisterContext;
    Stack* myStack;

public:
    ExecutionContext(RegisterContext* context, Stack* stack);

    ~ExecutionContext();
};