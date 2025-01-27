#pragma once
#ifndef MESSAGE_H
#define MESSAGE_H

#pragma push_macro("slots")
#undef slots
#include <Python.h>
#pragma pop_macro("slots")

#include <iostream>
#include <string>
#include<json/json.h>

class Message_str {
public:
    static void runPythonFunction(int param1, const std::string& param2Json, const std::string& param3, const std::string& param4);
};

#endif  // MESSAGE_H

