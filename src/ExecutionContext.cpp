#include "ExecutionContext.h"

Stack::Stack(size_t size) : mySize(size) {
}

int64_t Stack::MaterializeStackPointer() {
    if (myIsStackAllocated) {
        return (int64_t) myAlignedStackPointer;
    }

    char stack[mySize];
    for (int i = 0; i < mySize; ++i) {
        stack[i] = 0;
    }

    char* sp = (char*) stack;
    myStartStackPointer = sp;
    sp = (char*) (stack + sizeof(stack));
    sp = (char*) ((int64_t) sp & -16L);
    sp -= 128;
    myAlignedStackPointer = sp;
    myIsStackAllocated = true;

    return (int64_t) myAlignedStackPointer;
}

Stack::Stack(const Stack& stack) {
    myAlignedStackPointer = stack.myAlignedStackPointer;
    myStartStackPointer = stack.myStartStackPointer;
    mySize = stack.mySize;
}

Stack::Stack(Stack&& stack) noexcept {
    myAlignedStackPointer = stack.myAlignedStackPointer;
    myStartStackPointer = stack.myStartStackPointer;
    mySize = stack.mySize;
    stack.myAlignedStackPointer = nullptr;
    stack.myStartStackPointer = nullptr;
    stack.mySize = -1;
}

Stack& Stack::operator=(Stack other) {
    std::swap(myStartStackPointer, other.myStartStackPointer);
    std::swap(myAlignedStackPointer, other.myAlignedStackPointer);
    std::swap(mySize, other.mySize);
    return *this;
}

Stack::~Stack() {
    delete myStartStackPointer;
    mySize = -1;
}

ExecutionContext::ExecutionContext(const RegisterContext& context, Stack* stack) {
    myRegisterContext = context;
    myStack = stack;
}

ExecutionContext::ExecutionContext(const ExecutionContext& context) {
    myStack = context.myStack;
    myRegisterContext = context.myRegisterContext;
}

ExecutionContext& ExecutionContext::operator=(ExecutionContext other) {
    std::swap(myRegisterContext, other.myRegisterContext);
    std::swap(myStack, other.myStack);
    return *this;
}

RegisterContext ExecutionContext::GetRegisterContext() {
    return myRegisterContext;
}

void ExecutionContext::SetRegisterContext(const RegisterContext& newContext) {
    myRegisterContext = RegisterContext(newContext);
}

Stack* StackManager::AllocateStack() {
    return new Stack(2 << 15);
}

void StackManager::ReturnStack(Stack* stack) {
    delete stack;
}

StackManager* StackManager::ourInstance = nullptr;
StackManager* StackManager::GetInstance() {
    if (StackManager::ourInstance == nullptr) {
        StackManager::ourInstance = new StackManager();
    }

    return StackManager::ourInstance;
}
