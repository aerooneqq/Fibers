#include "ExecutionContext.h"

Stack::Stack(size_t size) : mySize(size) {
}

int64_t Stack::MaterializeStackPointer() {
    char* stack = new char[mySize];
    char* sp = (char*) (stack + sizeof(stack));
    myStartStackPointer = sp;
    sp = (char*) ((int64_t) sp & -16L);
    sp -= 128;

    myAlignedStackPointer = sp;

    if (mySnapshot != nullptr) {
        auto size = mySnapshot->GetSize();
        mySnapshot->SetBytes(myAlignedStackPointer);
        myAlignedStackPointer = sp - size;
        mySnapshot = nullptr;
    }

    myIsStackAllocated = true;

    return (int64_t) myAlignedStackPointer;
}

void Stack::SaveSnapshot(const std::vector<char>& snapshot) {
    mySnapshot = new StackSnapshot();
    mySnapshot->Save(snapshot);
}

Stack* StackManager::AllocateStack() {
    static Stack* stack = new Stack(2 << 15);
    return stack;
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

bool Stack::HasSavedSnapshot() {
    return mySnapshot != nullptr;
}

int64_t Stack::GetAlignedStackPointer() {
    return (int64_t) myAlignedStackPointer;
}

ExecutionContext::ExecutionContext(const RegisterContext& context, Stack* stack) {
    myRegisterContext = new RegisterContext(context);
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

RegisterContext ExecutionContext::Restore() {
    auto context = *myRegisterContext;
    if (myStack->HasSavedSnapshot()) {
        context.StackPointer = myStack->MaterializeStackPointer();
    }

    return context;
}

void ExecutionContext::Save(const RegisterContext& newContext, const std::vector<char>& snapshot) {
    myRegisterContext = new RegisterContext(newContext);
    myStack->SaveSnapshot(snapshot);
}

RegisterContext ExecutionContext::GetRegisterContext() {
    return *myRegisterContext;
}

Stack* ExecutionContext::GetStack() {
    return myStack;
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

void StackSnapshot::Save(const std::vector<char>& snapshot) {
    delete mySnapshot;
    mySnapshot = new std::vector<char>();

    for (char byte : snapshot) {
        mySnapshot->push_back(byte);
    }
}

void StackSnapshot::SetBytes(char* stackStart) {
    for (int i = 0; i < mySnapshot->size(); ++i) {
        *(stackStart - i) = mySnapshot->at(i);
    }
}

int StackSnapshot::GetSize() {
    return mySnapshot->size();
}
