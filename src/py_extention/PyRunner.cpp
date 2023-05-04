#include "PyRunner.hpp"

/**********************

//
//  PythonRunner(): Constructor for PythonRunner:
//                  Initializes Python and saves main thread state
//

static void * wrap_import_array() { import_array(); }

PythonRunner::PythonRunner() {
    //initialize python
    Py_InitializeEx(0);
    //PyEval_InitThreads();

    wrap_import_array();

    //add current directory to python path
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("import os");
    import_ufunc();
    PyRun_SimpleString("import numpy");
    PyRun_SimpleString("sys.path.append(os.getcwd())");

    //wrap_import_array();
    //

    PyRun_SimpleString("import numpy");
    PyRun_SimpleString("import pyshimmer");

    //save main thread state
    this->mainThreadState = PyEval_SaveThread();
}

//
//  ~PythonRunner(): Destructor for PythonRunner:
//                   Restores main thread state and deletes all modules
//
PythonRunner::~PythonRunner() {
    //delete all modules
    Py_Finalize();
}



//  #################################################################################################
//  #####################################  Load/Unload Functions  ###################################
//  #################################################################################################


//
//  LoadModule(): Loads a python module into a new interpreter 
//                and saves the interpreter in a ModuleInfo struct
//
int PythonRunner::LoadModule(std::string module_name) {

    //restore main thread state
    PyEval_RestoreThread(this->mainThreadState);

    //create module info
    ModuleInfo moduleInfo;
    moduleInfo.moduleId = this->Modules.size();
    moduleInfo.loadedModuleName = module_name;
    moduleInfo.moduleLoaded = true;
    moduleInfo.loadedFunctionName = "_none_";
    moduleInfo.functionRunning = false;
    moduleInfo.PyClass = nullptr;
    moduleInfo.PyThreadState = Py_NewInterpreter();

    if (moduleInfo.PyThreadState == nullptr) {
        throw std::runtime_error("Failed to create thread state");
    }

    //create Load Module Parameter
    std::shared_ptr<LoadModuleParams> moduleParams = std::make_shared<LoadModuleParams>();
    moduleParams->module_name = module_name;
    moduleParams->interp = moduleInfo.PyThreadState->interp;
    moduleParams->PyClass = nullptr;

    //Run and Join Process for module creation
    std::thread thread(ActionLoadModule, moduleParams);
    this->mainThreadState = PyEval_SaveThread();
    thread.join();

    //saving back Py Class
    moduleInfo.PyClass = moduleParams->PyClass;
    this->Modules.push_back(std::move(moduleInfo));

    return moduleInfo.moduleId;
}

//
//  UnloadModule(): Unloads a python module from an interpreter
//                  and deletes the interpreter as well as the ModuleInfo struct
//
void PythonRunner::UnloadModule(int moduleIndex) {
    //restore main thread state
    PyEval_RestoreThread(this->mainThreadState);

    //check valid module Index
    if (moduleIndex >= this->Modules.size()) {
        throw std::runtime_error("Module not loaded");
    }
    if (moduleIndex < 0) {
        throw std::runtime_error("Invalid module index");
    }

    //is a function running?
    if (this->Modules[moduleIndex].functionRunning) {
        throw std::runtime_error("A Function is running");
    }

    //create Load Module Parameter
    std::shared_ptr<LoadModuleParams> moduleParams = std::make_shared<LoadModuleParams>();
    moduleParams->module_name = this->Modules[moduleIndex].loadedModuleName;
    moduleParams->interp = this->Modules[moduleIndex].PyThreadState->interp;
    moduleParams->PyClass = this->Modules[moduleIndex].PyClass;
    
    //Run and Join Process for module deletion
    std::thread thread(ActionUnloadModule, moduleParams);
    this->mainThreadState = PyEval_SaveThread();

    //join thread
    thread.join();

    //delete interpreter
    Py_EndInterpreter(this->Modules[moduleIndex].PyThreadState);

    //remove from list
    this->Modules.erase(this->Modules.begin() + moduleIndex);
}



//  #################################################################################################
//  #####################################  Run/Join Functions  ######################################
//  #################################################################################################


//
//  StartProcess(): Starts a python function in a new thread
//                  and saves the thread in the ModuleInfo struct
//
void PythonRunner::StartProcess(int moduleIndex, std::string function_name, std::vector<std::string> args) {
    //restore main thread state
    PyEval_RestoreThread(this->mainThreadState);

    //check valid module Index
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

    //create new thread parameter
    std::shared_ptr<RunFunctionParams> functionParams = std::make_shared<RunFunctionParams>();
    functionParams->interp = this->Modules[moduleIndex].PyThreadState->interp;
    functionParams->PyClass = this->Modules[moduleIndex].PyClass;
    functionParams->function_name = function_name;
    functionParams->args = args;

    //Run Process
    std::thread thread(ActionRunFunction, functionParams);

    //save thread
    this->Modules[moduleIndex].active_thread = std::move(thread);
    this->Modules[moduleIndex].functionRunning = true;
    this->Modules[moduleIndex].loadedFunctionName = function_name;
    
    //save main thread state
    this->mainThreadState = PyEval_SaveThread();
}

//
//  JoinProcess(): Joins a python function thread
//                 and marks the thread as inactive in the ModuleInfo struct
//
void PythonRunner::JoinProcess(int moduleIndex){
    //check valid module Index
    if (moduleIndex >= this->Modules.size()) {
        throw std::runtime_error("Module not loaded");
    }
    if (moduleIndex < 0) {
        throw std::runtime_error("Invalid module index");
    }

    //check if a function is running
    if (!this->Modules[moduleIndex].functionRunning) {
        throw std::runtime_error("No Function is running");
    }

    //join thread
    this->Modules[moduleIndex].active_thread.join();
    this->Modules[moduleIndex].functionRunning = false;
    this->Modules[moduleIndex].loadedFunctionName = "_none_";
}



//  #################################################################################################
//  #####################################  Getter Functions  ########################################
//  #################################################################################################


//
//  getnumModules(): Returns the number of loaded modules
//
//
int PythonRunner::getnumModules() {
    //return number of modules
    return this->Modules.size();
}

//
//  getModuleIndex(): Returns the index of a module
//                    or -1 if not found
//
int PythonRunner::getModuleIndex(std::string module_name) {
    //search for module
    for (int i = 0; i < this->Modules.size(); i++) {
        if (this->Modules[i].loadedModuleName == module_name) {
            //found
            return i;
        }
    }

    //not found
    return -1;
}

//
//  getModuleName(): Returns the name of a module
//                 or -1 if not found
//
std::string PythonRunner::getModuleName(int moduleIndex) {
    //check valid module Index
    if (moduleIndex >= this->Modules.size()) {
        throw std::runtime_error("Module not loaded");
    }
    if (moduleIndex < 0) {
        throw std::runtime_error("Invalid module index");
    }

    //return module name
    return this->Modules[moduleIndex].loadedModuleName;
}

//
//  getFunctionRunning(): Returns if a function is running
//
//
bool PythonRunner::getFunctionRunning(int moduleIndex) {
    //check valid module Index
    if (moduleIndex >= this->Modules.size()) {
        throw std::runtime_error("Module not loaded");
    }
    if (moduleIndex < 0) {
        throw std::runtime_error("Invalid module index");
    }

    //return function running
    return this->Modules[moduleIndex].functionRunning;
}

//
//  getLoadedFunctionName(): Returns the name of the loaded function
//                           or "_none_" if no function is loaded
//
std::string PythonRunner::getLoadedFunctionName(int moduleIndex) {
    //check valid module Index
    if (moduleIndex >= this->Modules.size()) {
        throw std::runtime_error("Module not loaded");
    }
    if (moduleIndex < 0) {
        throw std::runtime_error("Invalid module index");
    }

    //return loaded function name
    return this->Modules[moduleIndex].loadedFunctionName;
}



//  #################################################################################################
//  #####################################  Check Functions  #########################################
//  #################################################################################################


//
//  checkModuleExists(): Checks if a module exists
//
//
bool PythonRunner::checkModuleExists(std::string module_name) {
    //restore main thread state
    PyEval_RestoreThread(this->mainThreadState);

    //try to import module
    std::string module_path = "scripts." + module_name + "." + module_name;
    PyObject* MyClass = PyImport_ImportModule(module_path.c_str());
    PyObject* obj = PyObject_CallObject(MyClass, NULL);

    //check if module exists
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

//
//  checkFunctionExists(): Checks if a function exists
//
//
bool PythonRunner::checkFunctionExists(std::string module_name, std::string function_name) {
    //restore main thread state
    PyEval_RestoreThread(this->mainThreadState);

    //try to import module
    std::string module_path = "scripts." + module_name + "." + module_name;
    PyObject* MyClass = PyImport_ImportModule(module_path.c_str());
    PyObject* obj = PyObject_CallObject(MyClass, NULL);

    //check if module exists
    if (obj == nullptr) {
        Py_DECREF(MyClass);
        Py_DECREF(obj);
        this->mainThreadState = PyEval_SaveThread();
        return false;
    }

    //check if function exists
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


**********************/
