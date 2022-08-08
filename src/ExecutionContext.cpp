#include "ExecutionContext.h"

Stack::Stack(size_t size) : mySize(size)
{
}

Stack::~Stack()
{
    delete myStack;
}

int64_t Stack::MaterializeStackPointer()
{
    return 0;
}


ExecutionContext::ExecutionContext(RegisterContext* context, Stack* stack)
{

}

ExecutionContext::~ExecutionContext()
{

}
