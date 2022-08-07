//
// Created by aeroo on 8/7/2022.
//
#include <string>
#include "util.h"

Lock::Lock(std::string name) : myName(std::move(name))
{
}

std::string Lock::GetName()
{
    return myName;
}

