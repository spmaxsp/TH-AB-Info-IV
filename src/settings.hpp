#include <fstream>
#include <string>
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
            shimmer = settingsData.at("shimmer").get<ShimmerSettings>();
            eeg = settingsData.at("eeg").get<EegSettings>();
            head = settingsData.at("head").get<HeadSettings>();
            main = settingsData.at("main").get<MainSettings>();
            python = settingsData.at("python").get<PythonSettings>();
            score = settingsData.at("score").get<ScoreSettings>();
        } catch (const json::exception&) {
            // Error occurred while parsing the settings, handle it accordingly
            return false;
        }

        return true;
    }

    bool saveSettings() {
        settingsData["shimmer"] = shimmer;
        settingsData["eeg"] = eeg;
        settingsData["head"] = head;
        settingsData["main"] = main;
        settingsData["python"] = python;
        settingsData["score"] = score;

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

        main.difficulty = "easy";
        main.sensor = "accelerometer";
        main.time = 60;
        main.virtual_head = false;

        python.path = "/usr/bin/python3";
        python.clean_exit_flag = true;

        score.high_score = 0;

        saveSettings();
    }
};
