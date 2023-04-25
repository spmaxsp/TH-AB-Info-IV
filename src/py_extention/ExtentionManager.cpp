#include "ExtentionManager.hpp"

int ExtentionManager::getExtentionIndex(std::string name) {
    for (int i = 0; i < this->extention_list.size(); i++) {
        if (this->extention_list[i].name == name) {
            return i;
        }
    }
    return -1;
}

void ExtentionManager::ListPythonExtention(std::vector<std::string>& list) {
    LOG_INIT_CERR();

    log(LOG_INFO) << "Reading modules.txt\n";
    std::fstream modulesFile = std::fstream("./scripts/modules.txt", std::ios::in);
    if (modulesFile.is_open()) {
        std::string line;
        while (getline(modulesFile, line)) {
            list.push_back(line);
        }
        modulesFile.close();
    }
    else {
        throw std::runtime_error("Failed to open modules.txt");
    }
}

void ExtentionManager::AddPythonExtention(std::string name) {
    LOG_INIT_CERR();

    //PythonRunner.checkModuleExists(name);

    int port = 50000 + this->extention_list.size();

    extention_info info;
    info.name = name; 
    info.extention_type = EXTENTION_TYPE_PYTHON;
    info.extention_id = this->extention_list.size();
    info.process_id = PythonRunner.LoadModule(name);
    info.socket_id = MultiSocket.addConnection("localhost", port);

    std::vector<std::string> args;
    args.push_back(std::to_string(port));
    PythonRunner.RunFunction(info.process_id, "setup", args, true);
    PythonRunner.StopFunction(info.process_id);   // Does not stop the function, but it does wait for it to finish
    std::vector<std::string> args2;
    PythonRunner.RunFunction(info.process_id, "run", args2, false);

    this->extention_list.push_back(info);
}

void ExtentionManager::RemovePythonExtention(std::string name) {
    LOG_INIT_CERR();

    int index = this->getExtentionIndex(name);

    MultiSocket.disconnect(this->extention_list[index].socket_id);

    PythonRunner.UnloadModule(this->extention_list[index].process_id);
    MultiSocket.removeConnection(this->extention_list[index].socket_id);

    this->extention_list.erase(this->extention_list.begin() + index);
}

void ExtentionManager::connectPythonExtention(std::string name) {
    LOG_INIT_CERR();

    int index = this->getExtentionIndex(name);

    Sleep(2000);    // Wait for the python script to start up

    int ret = MultiSocket.connect(this->extention_list[index].socket_id);
    if (ret != 0) {
        log(LOG_ERROR) << "Failed to connect to extention " << name << "\n";
    }
    log(LOG_INFO) << "Connected to extention " << name << "\n";
}

void ExtentionManager::writePythonExtention(std::string name, std::string& message) {
    LOG_INIT_CERR();

    int index = this->getExtentionIndex(name);

    MultiSocket.send(this->extention_list[index].socket_id, message);
}

void ExtentionManager::readPythonExtention(std::string name, std::string& message) {
    LOG_INIT_CERR();

    int index = this->getExtentionIndex(name);

    //TODO: Add recieve function
}

void ExtentionManager::disconnectPythonExtention(std::string name) {
    LOG_INIT_CERR();

    int index = this->getExtentionIndex(name);

    MultiSocket.disconnect(this->extention_list[index].socket_id);
}

void ExtentionManager::getNumExtention(int& num) {
    num = this->extention_list.size();
}

void ExtentionManager::getExtentionName(int index, std::string& name) {
    name = this->extention_list[index].name;
}

void ExtentionManager::getExtentionType(int index, int& type) {
    type = this->extention_list[index].extention_type;
}

void ExtentionManager::getExtentionStatus(int index, int& status) {
    status = EXTENTION_STATUS_UNDEFINED;
    if (this->extention_list[index].process_id != -1) {
        status = EXTENTION_STATUS_CREATED;
        if (PythonRunner.getFunctionRunning(this->extention_list[index].process_id)) {
            status = EXTENTION_STATUS_RUNNING;
            if (MultiSocket.isConnected(this->extention_list[index].socket_id)) {
                status = EXTENTION_STATUS_CONNECTED;
            }
        }
    }
}