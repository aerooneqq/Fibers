#include <string>
#include "util.h"

Lock::Lock(std::string name) : myName(std::move(name)) {
}

std::string Lock::GetName() {
    return myName;
}

BoolAtomicCookie::BoolAtomicCookie(std::atomic<bool>* atomic, bool startValue) {
    myStartValue = startValue;
    myAtomic = atomic;
    atomic->store(startValue, std::memory_order_seq_cst);
}

BoolAtomicCookie::~BoolAtomicCookie() {
    myAtomic->store(!myStartValue, std::memory_order_seq_cst);
    myAtomic = nullptr;
}
