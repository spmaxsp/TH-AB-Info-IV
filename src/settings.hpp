#include <fstream>
#include <string>
#include <json.hpp>

using json = nlohmann::json;

class SettingsManager {
public:
    std::string fileName;
    int highscore = 0;
    std::string pythonPath = "";
    int difficulty = 0;
    std::string mode = "";
    bool cleanExitFlag = false;

    SettingsManager(const std::string& fileName) : fileName(fileName) {}

    bool loadSettings() {
        std::ifstream file(fileName);
        if (!file.is_open()) {
            createDefaultSettingsFile();
            return false;
        }

        json jsonData;
        file >> jsonData;

        highscore = jsonData["Highscore"];
        pythonPath = jsonData["PythonPath"];
        difficulty = jsonData["Difficulty"];
        mode = jsonData["Mode"];
        cleanExitFlag = jsonData["CleanExitFlag"];

        file.close();
        return true;
    }

    bool saveSettings() {
        json jsonData;
        jsonData["Highscore"] = highscore;
        jsonData["PythonPath"] = pythonPath;
        jsonData["Difficulty"] = difficulty;
        jsonData["Mode"] = mode;
        jsonData["CleanExitFlag"] = cleanExitFlag;

        std::ofstream file(fileName);
        if (!file.is_open()) {
            return false;
        }

        file << jsonData;

        file.close();
        return true;
    }

private:
    void createDefaultSettingsFile() {
        json jsonData;
        jsonData["Highscore"] = highscore;
        jsonData["PythonPath"] = pythonPath;
        jsonData["Difficulty"] = difficulty;
        jsonData["Mode"] = mode;
        jsonData["CleanExitFlag"] = cleanExitFlag;

        std::ofstream file(fileName);
        if (!file.is_open()) {
            return;
        }

        file << jsonData;

        file.close();
    }
};