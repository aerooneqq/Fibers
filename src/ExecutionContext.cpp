#include "ExecutionContext.h"

Stack::Stack(size_t size) : mySize(size) {
}

int64_t Stack::MaterializeStackPointer() {
    if (myStack != nullptr) {
        return (int64_t) myStack;
    }

    char stack[mySize];
    for (int i = 0; i < mySize; ++i) {
        stack[i] = 0;
    }

    char* sp = (char*) (stack + sizeof(stack));
    sp = (char*) ((int64_t) sp & -16L);
    sp -= 128;

    myStack = sp;
    return (int64_t) sp;
}

Stack::Stack(const Stack& stack) {
    myStack = stack.myStack;
    mySize = stack.mySize;
}

Stack::Stack(Stack&& stack) noexcept {
    myStack = stack.myStack;
    mySize = stack.mySize;
    stack.myStack = nullptr;
    stack.mySize = -1;
}

void Stack::Destroy() {
    delete[] myStack;
    mySize = -1;
}

Stack& Stack::operator=(Stack other) {
    std::swap(myStack, other.myStack);
    std::swap(mySize, other.mySize);
    return *this;
}

ExecutionContext::ExecutionContext(const RegisterContext& context, const Stack& stack) {
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

Stack StackManager::AllocateStack() {
    return Stack(2 << 12);
}

void StackManager::ReturnStack(Stack stack) {
    stack.Destroy();
}

StackManager* StackManager::ourInstance = nullptr;
StackManager* StackManager::GetInstance() {
    if (StackManager::ourInstance == nullptr) {
        StackManager::ourInstance = new StackManager();
    }

    return StackManager::ourInstance;
}
