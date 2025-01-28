#include "message_str.h"

void Message_str::runPythonFunction(int param1, const std::string& param2Json, const std::string& param3, const std::string& param4) {
    // 解析JSON字符串为JSON对象
    Json::Value param2;
    Json::Reader reader;
    if (!reader.parse(param2Json, param2)) {
        std::cout << "Failed to parse JSON string." << std::endl;
        return;
    }

    Py_Initialize();
    PyObject* sysPath = PySys_GetObject("path");
    if (sysPath && PyList_Check(sysPath)) {
        Py_ssize_t len = PyList_Size(sysPath);
        for (Py_ssize_t i = 0; i < len; ++i) {
            PyObject* item = PyList_GetItem(sysPath, i);
            if (item && PyUnicode_Check(item)) {
                const char* path = PyUnicode_AsUTF8(item);
                //td::cout << "Python module search path: " << path << std::endl;
            }
        }
    }

    PyObject* pModule = PyImport_ImportModule("client");
    if (!pModule) {
        std::cout << "Failed to import Python module." << std::endl;
        PyErr_Print();
        Py_Finalize();
        return;
    }

    PyObject* pFunc = PyObject_GetAttrString(pModule, "alg");
    if (!pFunc || !PyCallable_Check(pFunc)) {
        std::cout << "Python function not found or not callable." << std::endl;
        PyErr_Print();
        Py_DECREF(pModule);
        Py_Finalize();
        return;
    }

    PyObject* pArgs = PyTuple_New(4);
    PyTuple_SetItem(pArgs, 0, PyLong_FromLong(param1));

    // 将JSON对象转换为Python对象
    PyObject* pParam2 = PyDict_New();
    for (const auto& key : param2.getMemberNames()) {
        const auto& value = param2[key];
        PyObject* pValue = nullptr;
        if (value.isString()) {
            pValue = PyUnicode_FromString(value.asCString());
        }
        else if (value.isInt()) {
            pValue = PyLong_FromLong(value.asInt());
        }
        else if (value.isDouble()) {
            pValue = PyFloat_FromDouble(value.asDouble());
        }
        else if (value.isBool()) {
            pValue = PyBool_FromLong(value.asBool());
        }
        if (pValue) {
            PyDict_SetItemString(pParam2, key.c_str(), pValue);
            Py_DECREF(pValue);
        }
    }
    PyTuple_SetItem(pArgs, 1, pParam2);

    PyTuple_SetItem(pArgs, 2, PyUnicode_FromString(param3.c_str()));
    PyTuple_SetItem(pArgs, 3, PyUnicode_FromString(param4.c_str()));

    PyObject* pResult = PyObject_CallObject(pFunc, pArgs);
    if (!pResult) {
        std::cout << "Failed to call Python function." << std::endl;
        PyErr_Print();
    }
    else {
        if (PyUnicode_Check(pResult)) {
            const char* result = PyUnicode_AsUTF8(pResult);
            std::cout << "Python function returned: " << result << std::endl;
        }
        else {
            std::cout << "Python function returned an unexpected result." << std::endl;
        }
        Py_DECREF(pResult);
    }

    Py_DECREF(pArgs);
    Py_DECREF(pFunc);
    Py_DECREF(pModule);
    Py_Finalize();
}
