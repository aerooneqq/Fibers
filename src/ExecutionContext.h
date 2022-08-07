class Stack
{
private:
    char* myStack;

public:
    Stack(int size);
    ~Stack();
};

class ExecutionContext {
private:
    RegisterContext* myRegisterContext;
    Stack* myStack;

public:
    ExecutionContext(RegisterContext* context, Stack* stack);

    ~ExecutionContext();
};