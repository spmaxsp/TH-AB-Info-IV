

class PyModule
{
    private:
        PyInstance* instance;
        int processId;
        int port;
        std::string name;
        SoccketClient* client;
    public:
        PyModule(std::string name, PyInstance* instance, int port);
        ~PyModule();
        void connect();
        void disconnect();
        void send(std::string message);
        void recieve(std::string& message);
        std::string getName();
};


class ModuleTemplate : public PyModule
{
    public:
        ModuleTemplate(std::string name, PyInstance* instance, int port) : PyModule(name, instance, port) {}
        ~ModuleTemplate() {}
        void connect() {
            PyModule::connect();
        }
        void disconnect() {
            PyModule::disconnect();
        }
};