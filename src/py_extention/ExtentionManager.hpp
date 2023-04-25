#pragma once

#include <stdlib.h>
#include <string>
#include <vector>

#include "PyRunner.hpp"
#include "websocket/MultiSocket.hpp"

#define EXTENTION_TYPE_UNDEFINED -1
#define EXTENTION_TYPE_PYTHON 0

#define EXTENTION_STATUS_UNDEFINED -1
#define EXTENTION_STATUS_CREATED 0
#define EXTENTION_STATUS_RUNNING 1
#define EXTENTION_STATUS_CONNECTED 2

struct extention_info {
    std::string name;
    int extention_type;
    int extention_id;
    int process_id;
    int socket_id;
};

class ExtentionManager {
private:
    PythonRunner PythonRunner;
    MultiSocket MultiSocket;
    std::vector<extention_info> extention_list;

    int getExtentionIndex(std::string name);

public:
    void ListPythonExtention(std::vector<std::string>& list);

    void AddPythonExtention(std::string name);
    void RemovePythonExtention(std::string name);

    void connectPythonExtention(std::string name);
    void writePythonExtention(std::string name, std::string& message);
    void readPythonExtention(std::string name, std::string& message);
    void disconnectPythonExtention(std::string name);

    void getNumExtention(int& num);
    void getExtentionName(int index, std::string& name);
    void getExtentionType(int index, int& type);
    void getExtentionStatus(int index, int& status);
};
