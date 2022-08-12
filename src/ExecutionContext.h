#include "util.h"

class StackSnapshot {
private:
    std::vector<char>* mySnapshot;
public:
    void Save(const std::vector<char>& snapshot);
    void SetBytes(char* stackStart);
    int GetSize();
};

class Stack {
private:
    bool myIsStackAllocated{false};
    char* myStartStackPointer{nullptr};
    char* myAlignedStackPointer{nullptr};
    StackSnapshot* mySnapshot{nullptr};
    size_t mySize{0};

public:
    Stack() = delete;
    explicit Stack(size_t size);
    Stack(const Stack& stack);
    Stack(Stack&& stack) noexcept;
    ~Stack();

    Stack& operator=(Stack other);

    int64_t MaterializeStackPointer();
    int64_t GetAlignedStackPointer();
    void SaveSnapshot(const std::vector<char>& snapshot);

    bool HasSavedSnapshot();
};

class StackManager {
private:
    static StackManager* ourInstance;

public:
    static StackManager* GetInstance();
    Stack* AllocateStack();

    void ReturnStack(Stack* stack);
};

class ExecutionContext {
private:
    RegisterContext* myRegisterContext;
    Stack* myStack{nullptr};

public:
    ExecutionContext() = default;
    ExecutionContext(const RegisterContext& context, Stack* stack);
    ExecutionContext(const ExecutionContext& context);
    ExecutionContext(ExecutionContext&& context) = delete;

    ExecutionContext& operator=(ExecutionContext other);

    RegisterContext GetRegisterContext();
    Stack* GetStack();

    RegisterContext Restore();
    void Save(const RegisterContext& newContext, const std::vector<char>& snapshot);
};