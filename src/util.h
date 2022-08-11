#include <atomic>
#include "emmintrin.h"
#include <functional>
#include <iostream>

struct RegisterContext;

extern "C" void SetContext(RegisterContext* context);
extern "C" void FillContext(RegisterContext* context);

class Lock {
private:
    std::string myName;

public:
    explicit Lock(std::string name);

    std::string GetName();
};

class BoolAtomicCookie {
private:
    bool myStartValue;
    std::atomic<bool>* myAtomic;
public:
    BoolAtomicCookie(std::atomic<bool>* atomic, bool startValue);

    ~BoolAtomicCookie();
};


struct RegisterContext {
    int64_t InstructionPointer{};
    int64_t StackPointer{};
    int64_t Rbx{};
    int64_t Rbp{};
    int64_t R12{};
    int64_t R13{};
    int64_t R14{};
    int64_t R15{};
    int64_t Rdi{};
    int64_t Rsi{};
    int64_t FirstIntArgument{};
    int64_t SecondIntArgument{};

    __m128i Xmm6{};
    __m128i Xmm7{};
    __m128i Xmm8{};
    __m128i Xmm9{};
    __m128i Xmm10{};
    __m128i Xmm11{};
    __m128i Xmm12{};
    __m128i Xmm13{};
    __m128i Xmm14{};
    __m128i Xmm15{};
};