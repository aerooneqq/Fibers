#include <gtest/gtest.h>
#include "../src/ExecutionContext.h"

TEST(ContextSwitchTests, SimpleTest) {
    volatile int x = 0;
    volatile int y = 0;
    RegisterContext context{};
    FillContext(&context);
    ++y;

    if (x == 0) {
        ++x;
        SetContext(&context);
    }

    ASSERT_EQ(y, 2);
}

void SomeFunction(int* x, RegisterContext* savedContext) {
    (*x) = 123;
    SetContext(savedContext);
}

TEST(ContextSwitchTests, TestSwitchToMethod) {
    volatile int x = 0;
    int y = 0;
    RegisterContext context{};
    FillContext(&context);

    if (x == 0) {
        ++x;
        RegisterContext functionCallContext{};
        functionCallContext.FirstIntArgument = (int64_t) &y;
        functionCallContext.SecondIntArgument = (int64_t) &context;
        functionCallContext.InstructionPointer = (int64_t) (void*) SomeFunction;
        functionCallContext.StackPointer = context.StackPointer;
        SetContext(&functionCallContext);
    }

    ASSERT_EQ(y, 123);
}

TEST(ContextSwitchTests, TestSwithToMethodWithCustomStack) {
    volatile int x = 0;
    int y = 0;
    RegisterContext context{};
    FillContext(&context);

    auto stack = new Stack(2 << 12);
    if (x == 0) {
        ++x;
        RegisterContext functionCallContext{};
        functionCallContext.FirstIntArgument = (int64_t) &y;
        functionCallContext.SecondIntArgument = (int64_t) &context;
        functionCallContext.InstructionPointer = (int64_t) (void*) SomeFunction;


        functionCallContext.StackPointer = stack->MaterializeStackPointer();

        SetContext(&functionCallContext);
    }

    delete stack;
    ASSERT_EQ(y, 123);
}