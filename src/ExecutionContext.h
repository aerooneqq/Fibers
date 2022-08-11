#include "util.h"

class Stack {
private:
    bool myIsStackAllocated{false};
    char* myStartStackPointer{nullptr};
    char* myAlignedStackPointer{nullptr};
    size_t mySize{0};

public:
    Stack() = default;
    explicit Stack(size_t size);
    Stack(const Stack& stack);
    Stack(Stack&& stack) noexcept;

    Stack& operator=(Stack other);

    int64_t MaterializeStackPointer();
    void Destroy();
};

class StackManager {
private:
    static StackManager* ourInstance;

public:
    static StackManager* GetInstance();
    Stack AllocateStack();

    void ReturnStack(Stack stack);
};

class ExecutionContext {
private:
    RegisterContext myRegisterContext;
    Stack myStack;

public:
    ExecutionContext() = default;
    ExecutionContext(const RegisterContext& context, const Stack& stack);
    ExecutionContext(const ExecutionContext& context);
    ExecutionContext(ExecutionContext&& context) = delete;

    ExecutionContext& operator=(ExecutionContext other);
};