#include "PyRunner.hpp"

void ActionLoadModule(std::shared_ptr<LoadModuleParams> module_params) {
    LOG_INIT_CERR();

    PyThreadState* _state = PyThreadState_New(module_params->interp);

    if (_state == nullptr) {
        throw std::runtime_error("Failed to create thread state");
    }

    PyEval_AcquireThread(_state);

    PyThreadState* _swap = PyThreadState_Swap(_state);

    log(LOG_INFO) << "Importing module " << module_params->module_name << "\n";
    std::string module_path = "scripts." + module_params->module_name;
    PyObject* PyModule = PyImport_ImportModule(module_path.c_str());
    if (PyModule == nullptr) {
        PyErr_Print();
        throw std::runtime_error("Failed to import module");
    }

    log(LOG_INFO) << "Loading class\n";
    module_params->PyClass = PyObject_GetAttrString(PyModule, module_params->module_name.c_str());
    if (module_params->PyClass == nullptr) {
        PyErr_Print();
        throw std::runtime_error("Failed to load class");
    }

    log(LOG_DEBUG) << "Cleaning up\n";
    PyThreadState_Swap(_swap);
    PyEval_ReleaseThread(_state);
}

void ActionUnloadModule(std::shared_ptr<LoadModuleParams> params) {
    LOG_INIT_CERR();

    PyThreadState* _state = PyThreadState_New(params->interp);

    PyEval_AcquireThread(_state);
    PyThreadState* _swap = PyThreadState_Swap(_state);

    log(LOG_INFO) << "Unloading module\n";
    Py_DECREF(params->PyClass);

    log(LOG_DEBUG) << "Cleaning up\n";
    PyThreadState_Swap(_swap);
    PyEval_ReleaseThread(_state);

    PyThreadState_Clear(_state);
    PyThreadState_Delete(_state);
}

void ActionRunFunction(std::shared_ptr<RunFunctionParams> params) {
    LOG_INIT_CERR();

    PyThreadState* _state = PyThreadState_New(params->interp);

    PyEval_AcquireThread(_state);
    PyThreadState* _swap = PyThreadState_Swap(_state);


    log(LOG_INFO) << "Running Script\n";

    log(LOG_DEBUG) << "Creating arguments\n";
    PyObject* PyArgs = PyTuple_New(params->args.size() + 1);
    PyTuple_SetItem(PyArgs, 0, params->PyClass);
    for (int i = 0; i < params->args.size(); i++) {
        PyTuple_SetItem(PyArgs, i+1, PyUnicode_FromString(params->args[i].c_str()));
    }

    log(LOG_DEBUG) << "Finding function\n";
    PyObject* PyFunction = PyObject_GetAttrString(params->PyClass, params->function_name.c_str());
    if (PyFunction == nullptr) {
        PyErr_Print();
        throw std::runtime_error("Failed to find function");
    }

    log(LOG_DEBUG) << "Calling function (once)\n";
    if (params->oneshot) {
        PyObject* PyResult = PyObject_CallObject(PyFunction, PyArgs);
        if (PyResult == nullptr) {
            PyErr_Print();
            throw std::runtime_error("Failed to call function");
        }
    }
    else {
        while (*params->stopSwitch) {
            log(LOG_DEBUG) << "Calling function (loop)\n";
            PyObject* PyResult = PyObject_CallObject(PyFunction, PyArgs);
            if (PyResult == nullptr) {
                PyErr_Print();
                throw std::runtime_error("Failed to call function");
            }
        }
    }

    log(LOG_DEBUG) << "Cleaning up\n";
    Py_DECREF(PyArgs);  
    Py_DECREF(PyFunction);

    PyThreadState_Swap(_swap);
    PyEval_ReleaseThread(_state);
} 