#include "message.h"

std::string Message::runPythonFunction(int param1, int param2, const std::string& param3, const std::string& param4) {
    Py_Initialize();

    PyObject* pModule = PyImport_ImportModule("client");
    if (!pModule) {
        std::cout << "Failed to import Python module." << std::endl;
        PyErr_Print();
        Py_Finalize();
        return "";
    }

    PyObject* pFunc = PyObject_GetAttrString(pModule, "alg");
    if (!pFunc || !PyCallable_Check(pFunc)) {
        std::cout << "Python function not found or not callable." << std::endl;
        PyErr_Print();
        Py_DECREF(pModule);
        Py_Finalize();
        return "";
    }

    PyObject* pArgs = PyTuple_New(4);
    PyTuple_SetItem(pArgs, 0, PyLong_FromLong(param1));
    PyTuple_SetItem(pArgs, 1, PyLong_FromLong(param2));
    PyTuple_SetItem(pArgs, 2, PyUnicode_FromString(param3.c_str()));
    PyTuple_SetItem(pArgs, 3, PyUnicode_FromString(param4.c_str()));

    PyObject* pResult = PyObject_CallObject(pFunc, pArgs);
    if (!pResult) {
        std::cout << "Failed to call Python function." << std::endl;
        PyErr_Print();
        Py_DECREF(pArgs);
        Py_DECREF(pFunc);
        Py_DECREF(pModule);
        Py_Finalize();
        return "";
    }

    std::string result;
    if (PyUnicode_Check(pResult)) {
        const char* resultChar = PyUnicode_AsUTF8(pResult);
        result = resultChar;
        std::cout << "Python function returned: " << result << std::endl;
    }
    else {
        std::cout << "Python function returned an unexpected result." << std::endl;
    }

    Py_DECREF(pResult);
    Py_DECREF(pArgs);
    Py_DECREF(pFunc);
    Py_DECREF(pModule);
    Py_Finalize();

    return result;
}


