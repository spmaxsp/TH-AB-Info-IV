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

using json = nlohmann::json;

struct ShimmerSettings {
    std::string com;
    int pollrate;
};

struct EegSettings {
    std::string user_id;
    std::string token;
    int pollrate;
};

struct HeadSettings {
    float step_size;
};

struct MainSettings {
    int difficulty;
    int sensor;
    int time;
    bool virtual_head;
};

struct PythonSettings {
    std::string path;
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
    MainSettings main;
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
            shimmer.com = settingsData["shimmer"]["com"];
            shimmer.pollrate = settingsData["shimmer"]["pollrate"];

            eeg.user_id = settingsData["eeg"]["user_id"];
            eeg.token = settingsData["eeg"]["token"];
            eeg.pollrate = settingsData["eeg"]["pollrate"];

            head.step_size = settingsData["head"]["step_size"];

            main.difficulty = settingsData["main"]["difficulty"];
            main.sensor = settingsData["main"]["sensor"];
            main.time = settingsData["main"]["time"];
            main.virtual_head = settingsData["main"]["virtual_head"];

            python.path = settingsData["python"]["path"];
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

        settingsData["main"]["difficulty"] = main.difficulty;
        settingsData["main"]["sensor"] = main.sensor;
        settingsData["main"]["time"] = main.time;
        settingsData["main"]["virtual_head"] = main.virtual_head;

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
        shimmer.com = "COM1";
        shimmer.pollrate = 100;

        eeg.user_id = "";
        eeg.token = "";
        eeg.pollrate = 500;

        head.step_size = 0.1;

        main.difficulty = DIFFICULTY_EASY;
        main.sensor = SENSOR_SHIMMER;
        main.time = 60;
        main.virtual_head = false;

        python.path = "/usr/bin/python3";
        python.clean_exit_flag = true;

        score.high_score = 0;

        saveSettings();
    }
};
