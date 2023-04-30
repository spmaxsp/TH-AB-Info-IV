#include "PyRunner.hpp"

void ActionLoadModule(std::shared_ptr<LoadModuleParams> module_params) {
    PyThreadState* _state = PyThreadState_New(module_params->interp);

    if (_state == nullptr) {
        throw std::runtime_error("Failed to create thread state");
    }

    PyEval_AcquireThread(_state);

    PyThreadState* _swap = PyThreadState_Swap(_state);

    std::string module_path = "scripts." + module_params->module_name;
    PyObject* PyModule = PyImport_ImportModule(module_path.c_str());
    if (PyModule == nullptr) {
        PyErr_Print();
        throw std::runtime_error("Failed to import module");
    }
    Py_INCREF(PyModule);

    module_params->PyClass = PyObject_GetAttrString(PyModule, module_params->module_name.c_str());
    if (module_params->PyClass == nullptr) {
        PyErr_Print();
        throw std::runtime_error("Failed to load class");
    }
    Py_INCREF(module_params->PyClass);

    PyThreadState_Swap(_swap);
    PyEval_ReleaseThread(_state);
}

void ActionUnloadModule(std::shared_ptr<LoadModuleParams> params) {
    PyThreadState* _state = PyThreadState_New(params->interp);

    PyEval_AcquireThread(_state);
    PyThreadState* _swap = PyThreadState_Swap(_state);

    Py_DECREF(params->PyClass);

    PyThreadState_Swap(_swap);
    PyEval_ReleaseThread(_state);

    PyThreadState_Clear(_state);
    PyThreadState_Delete(_state);
}

void ActionRunFunction(std::shared_ptr<RunFunctionParams> params) {
    PyThreadState* _state = PyThreadState_New(params->interp);

    PyEval_AcquireThread(_state);
    PyThreadState* _swap = PyThreadState_Swap(_state);

    PyObject* PyArgs = PyTuple_New(params->args.size() + 1);
    if (PyArgs == nullptr) {
        PyErr_Print();
        throw std::runtime_error("Failed to create args tuple");
    }
    PyTuple_SetItem(PyArgs, 0, params->PyClass);
    for (int i = 0; i < params->args.size(); i++) {
        PyTuple_SetItem(PyArgs, i+1, PyUnicode_FromString(params->args[i].c_str()));
    }

    if(params->PyClass == nullptr){
        throw std::runtime_error("PyClass is null");
    }

    Py_INCREF(params->PyClass);
    PyObject* PyFunction = PyObject_GetAttrString(params->PyClass, params->function_name.c_str());
    if (PyFunction == nullptr) {
        PyErr_Print();
        throw std::runtime_error("Failed to find function");
    }

    PyObject* PyResult = PyObject_CallObject(PyFunction, PyArgs);
    if (PyResult == nullptr) {
        PyErr_Print();
        throw std::runtime_error("Failed to call function");
    }

    Py_DECREF(PyArgs);  
    Py_DECREF(PyFunction);
    Py_DECREF(PyResult);

    PyThreadState_Swap(_swap);
    PyEval_ReleaseThread(_state);
} 