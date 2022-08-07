#include "emmintrin.h"

typedef void PureJobFunction();

class Lock
{
private:
    std::string myName;

public:
    explicit Lock(std::string name);
    std::string GetName();
};


struct RegisterContext
{
    int64_t ReturnAddress{};
    int64_t StackPointer{};
    int64_t Rbx{};
    int64_t Rbp{};
    int64_t R12{};
    int64_t R13{};
    int64_t R14{};
    int64_t R15{};
    int64_t Rdi{};
    int64_t Rsi{};

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