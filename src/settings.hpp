#include <fstream>
#include <string>
#include <iomanip>

#include <json.hpp>

#define DIFFICULTY_EASY 0
#define DIFFICULTY_MEDIUM 1
#define DIFFICULTY_HARD 2

#define SENSOR_SHIMMER 0
#define SENSOR_EEG 1
#define SENSOR_VIRTUAL 2

#define TIME_30 0
#define TIME_60 1
#define TIME_90 2

using json = nlohmann::json;

struct ShimmerSettings {
    char com[50];
    int pollrate;
};

struct EegSettings {
    char user_id[100];
    char token[300];
    int pollrate;
};

struct HeadSettings {
    int step_size;
};

struct MainSettings {
    int difficulty;
    int sensor;
    int time;
    bool virtual_head;
    int camera_index;
};

struct PythonSettings {
    char path[300];
    bool clean_exit_flag;
};

struct ScoreSettings {
    int high_score;
};

class SettingsManager {
public:
    std::string fileName;
    ShimmerSettings shimmer;
    EegSettings eeg;
    HeadSettings head;
    MainSettings global;
    PythonSettings python;
    ScoreSettings score;

    SettingsManager(const std::string& fileName) : fileName(fileName) {}

    bool loadSettings() {
        std::ifstream file(fileName);
        if (!file.is_open()) {
            createDefaultSettingsFile();
            return false;
        }

        file >> settingsData;
        file.close();

        try {
            strcpy(shimmer.com, settingsData["shimmer"]["com"].get<std::string>().c_str());
            shimmer.pollrate = settingsData["shimmer"]["pollrate"];

            strcpy(eeg.user_id, settingsData["eeg"]["user_id"].get<std::string>().c_str());
            strcpy(eeg.token, settingsData["eeg"]["token"].get<std::string>().c_str());
            eeg.pollrate = settingsData["eeg"]["pollrate"];

            head.step_size = settingsData["head"]["step_size"];

            global.difficulty = settingsData["global"]["difficulty"];
            global.sensor = settingsData["global"]["sensor"];
            global.time = settingsData["global"]["time"];
            global.virtual_head = settingsData["global"]["virtual_head"];
            global.camera_index = settingsData["global"]["camera_index"];

            strcpy(python.path, settingsData["python"]["path"].get<std::string>().c_str());
            python.clean_exit_flag = settingsData["python"]["clean_exit_flag"];

            score.high_score = settingsData["score"]["high_score"];

        } catch (const json::exception&) {
            // Error occurred while parsing the settings, handle it accordingly
            return false;
        }

        return true;
    }

    bool saveSettings() {
        settingsData["shimmer"]["com"] = shimmer.com;
        settingsData["shimmer"]["pollrate"] = shimmer.pollrate;

        settingsData["eeg"]["user_id"] = eeg.user_id;
        settingsData["eeg"]["token"] = eeg.token;
        settingsData["eeg"]["pollrate"] = eeg.pollrate;

        settingsData["head"]["step_size"] = head.step_size;

        settingsData["global"]["difficulty"] = global.difficulty;
        settingsData["global"]["sensor"] = global.sensor;
        settingsData["global"]["time"] = global.time;
        settingsData["global"]["virtual_head"] = global.virtual_head;
        settingsData["global"]["camera_index"] = global.camera_index;

        settingsData["python"]["path"] = python.path;
        settingsData["python"]["clean_exit_flag"] = python.clean_exit_flag;

        settingsData["score"]["high_score"] = score.high_score;

        std::ofstream file(fileName);
        if (!file.is_open()) {
            return false;
        }

        file << settingsData;
        file.close();
        return true;
    }

private:
    json settingsData;

    void createDefaultSettingsFile() {
        strcpy(shimmer.com, "COM3");
        shimmer.pollrate = 100;

        strcpy(eeg.user_id, "");
        strcpy(eeg.token, "");
        eeg.pollrate = 500;

        head.step_size = 0.1;

        global.difficulty = DIFFICULTY_EASY;
        global.sensor = SENSOR_SHIMMER;
        global.time = 60;
        global.virtual_head = false;
        global.camera_index = 0;

        strcpy(python.path, "python");
        python.clean_exit_flag = true;

        score.high_score = 0;

        saveSettings();
    }
};
