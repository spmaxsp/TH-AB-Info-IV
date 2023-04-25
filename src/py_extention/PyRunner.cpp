#include "PyRunner.hpp"

PythonRunner::PythonRunner() {
    LOG_INIT_CERR();

    log(LOG_INFO) << "Initializing Python\n";
    Py_InitializeEx(1);
    PyEval_InitThreads();

    log(LOG_INFO) << "Changing path\n";
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("import os");
    PyRun_SimpleString("sys.path.append(os.getcwd())");

    this->mainThreadState = PyEval_SaveThread();
}

PythonRunner::~PythonRunner() {
    LOG_INIT_CERR();

    PyEval_RestoreThread(mainThreadState);
    Py_Finalize();
}

int PythonRunner::LoadModule(std::string module_name) {
    LOG_INIT_CERR();

    PyEval_RestoreThread(this->mainThreadState);

    //create module info
    ModuleInfo moduleInfo;
    moduleInfo.moduleId = this->Modules.size();
    moduleInfo.loadedModuleName = module_name;
    moduleInfo.moduleLoaded = true;
    moduleInfo.loadedFunctionName = "_none_";
    moduleInfo.functionRunning = false;
    moduleInfo.stopSwitch = std::make_shared<bool>(false);
    moduleInfo.PyClass = nullptr;
    moduleInfo.PyThreadState = Py_NewInterpreter();

    if (moduleInfo.PyThreadState == nullptr) {
        throw std::runtime_error("Failed to create thread state");
    }

    std::shared_ptr<LoadModuleParams> moduleParams = std::make_shared<LoadModuleParams>();
    moduleParams->module_name = module_name;
    moduleParams->interp = moduleInfo.PyThreadState->interp;
    moduleParams->PyClass = nullptr;

    std::thread thread(ActionLoadModule, moduleParams);

    this->mainThreadState = PyEval_SaveThread();

    thread.join();

    moduleInfo.PyClass = moduleParams->PyClass;
    this->Modules.push_back(std::move(moduleInfo));

    return moduleInfo.moduleId;
}

void PythonRunner::UnloadModule(int moduleIndex) {
    LOG_INIT_CERR();

    PyEval_RestoreThread(this->mainThreadState);

    if (moduleIndex >= this->Modules.size()) {
        throw std::runtime_error("Module not loaded");
    }
    if (moduleIndex < 0) {
        throw std::runtime_error("Invalid module index");
    }

    //stop function if running
    if (this->Modules[moduleIndex].functionRunning) {
        this->Modules[moduleIndex].stopSwitch = std::make_shared<bool>(true);
        this->Modules[moduleIndex].active_thread.join();
        this->Modules[moduleIndex].functionRunning = false;
        this->Modules[moduleIndex].loadedFunctionName = "_none_";
    }

    //unload module
    std::shared_ptr<LoadModuleParams> moduleParams = std::make_shared<LoadModuleParams>();
    moduleParams->module_name = this->Modules[moduleIndex].loadedModuleName;
    moduleParams->interp = this->Modules[moduleIndex].PyThreadState->interp;
    moduleParams->PyClass = this->Modules[moduleIndex].PyClass;
    
    std::thread thread(ActionUnloadModule, moduleParams);
    thread.join();

    //delete interpreter
    Py_EndInterpreter(this->Modules[moduleIndex].PyThreadState);

    //remove from list
    this->Modules.erase(this->Modules.begin() + moduleIndex);

    this->mainThreadState = PyEval_SaveThread();
}

void PythonRunner::RunFunction(int moduleIndex, std::string function_name, std::vector<std::string> args, bool oneshot) {
    LOG_INIT_CERR();

    PyEval_RestoreThread(this->mainThreadState);

    if (moduleIndex >= this->Modules.size()) {
        throw std::runtime_error("Module not loaded");
    }
    if (moduleIndex < 0) {
        throw std::runtime_error("Invalid module index");
    }

    //check if a function is already running
    if (this->Modules[moduleIndex].functionRunning) {
        throw std::runtime_error("A Function is already running");
    }

    //create new thread
    std::shared_ptr<RunFunctionParams> functionParams = std::make_shared<RunFunctionParams>();
    functionParams->interp = this->Modules[moduleIndex].PyThreadState->interp;
    functionParams->PyClass = this->Modules[moduleIndex].PyClass;
    functionParams->function_name = function_name;
    functionParams->oneshot = oneshot;
    functionParams->stopSwitch = this->Modules[moduleIndex].stopSwitch;
    functionParams->args = args;

    std::thread thread(ActionRunFunction, functionParams);
    this->Modules[moduleIndex].active_thread = std::move(thread);

    this->Modules[moduleIndex].functionRunning = true;
    this->Modules[moduleIndex].loadedFunctionName = function_name;
    
    this->mainThreadState = PyEval_SaveThread();
}

void PythonRunner::StopFunction(int moduleIndex) {
    LOG_INIT_CERR();

    if (moduleIndex >= this->Modules.size()) {
        throw std::runtime_error("Module not loaded");
    }
    if (moduleIndex < 0) {
        throw std::runtime_error("Invalid module index");
    }

    log(LOG_INFO) << "Stopping function\n";
    if (this->Modules[moduleIndex].functionRunning) {
        this->Modules[moduleIndex].stopSwitch = std::make_shared<bool>(true);
        this->Modules[moduleIndex].active_thread.join();
        this->Modules[moduleIndex].functionRunning = false;
        this->Modules[moduleIndex].loadedFunctionName = "_none_";
        return;
    }
    else {
        throw std::runtime_error("Function not running");
    }
}

int PythonRunner::getnumModules() {
    return this->Modules.size();
}

int PythonRunner::getModuleIndex(std::string module_name) {

    for (int i = 0; i < this->Modules.size(); i++) {
        if (this->Modules[i].loadedModuleName == module_name) {
            return i;
        }
    }

    return -1;
}

std::string PythonRunner::getModuleName(int moduleIndex) {
    if (moduleIndex >= this->Modules.size()) {
        throw std::runtime_error("Module not loaded");
    }
    if (moduleIndex < 0) {
        throw std::runtime_error("Invalid module index");
    }

    return this->Modules[moduleIndex].loadedModuleName;
}

bool PythonRunner::getFunctionRunning(int moduleIndex) {
    if (moduleIndex >= this->Modules.size()) {
        throw std::runtime_error("Module not loaded");
    }
    if (moduleIndex < 0) {
        throw std::runtime_error("Invalid module index");
    }

    return this->Modules[moduleIndex].functionRunning;
}

std::string PythonRunner::getLoadedFunctionName(int moduleIndex) {
    if (moduleIndex >= this->Modules.size()) {
        throw std::runtime_error("Module not loaded");
    }
    if (moduleIndex < 0) {
        throw std::runtime_error("Invalid module index");
    }

    return this->Modules[moduleIndex].loadedFunctionName;
}

bool PythonRunner::checkModuleExists(std::string module_name) {
    LOG_INIT_CERR();

    PyEval_RestoreThread(this->mainThreadState);

    std::string module_path = "scripts." + module_name + "." + module_name;
    PyObject* MyClass = PyImport_ImportModule(module_path.c_str());
    PyObject* obj = PyObject_CallObject(MyClass, NULL);


    if (obj == nullptr) {
        Py_DECREF(MyClass);
        this->mainThreadState = PyEval_SaveThread();
        return false;
    }
    else {
        Py_DECREF(MyClass);
        Py_DECREF(obj);
        this->mainThreadState = PyEval_SaveThread();
        return true;
    }
}

bool PythonRunner::checkFunctionExists(std::string module_name, std::string function_name) {

    PyEval_RestoreThread(this->mainThreadState);

    std::string module_path = "scripts." + module_name + "." + module_name;
    PyObject* MyClass = PyImport_ImportModule(module_path.c_str());
    PyObject* obj = PyObject_CallObject(MyClass, NULL);

    if (obj == nullptr) {
        Py_DECREF(MyClass);
        Py_DECREF(obj);
        this->mainThreadState = PyEval_SaveThread();
        return false;
    }
    else {
        PyObject* func = PyObject_GetAttrString(obj, function_name.c_str());
        if (func == nullptr) {
            Py_DECREF(MyClass);
            Py_DECREF(obj);
            this->mainThreadState = PyEval_SaveThread();
            return false;
        }
        else {
            Py_DECREF(MyClass);
            Py_DECREF(obj);
            Py_DECREF(func);
            this->mainThreadState = PyEval_SaveThread();
            return true;
        }
    }
}
