#pragma once
#ifndef MESSAGE_H
#define MESSAGE_H

#pragma push_macro("slots")
#undef slots
#include <Python.h>
#pragma pop_macro("slots")

#include <iostream>
#include <string>
class Message {
public:
    static std::string runPythonFunction(int param1, int param2, const std::string& param3, const std::string& param4);
};

#endif  // MESSAGE_H
