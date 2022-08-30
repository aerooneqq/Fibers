#include <gtest/gtest.h>
#include "../src/util.h"

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
    char stack[2 << 15];
    RegisterContext context{};
    FillContext(&context);

    if (x == 0) {
        ++x;
        RegisterContext functionCallContext{};
        functionCallContext.FirstIntArgument = (int64_t) &y;
        functionCallContext.SecondIntArgument = (int64_t) &context;
        functionCallContext.InstructionPointer = (int64_t) (void*) SomeFunction;
        functionCallContext.StackPointer = (int64_t) (char*)stack + (2 << 15);

        SetContext(&functionCallContext);
    }

    ASSERT_EQ(y, 123);
}