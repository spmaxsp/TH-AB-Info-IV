#include "gamelogic.hpp"

GameLogic::GameLogic(Shimmersensor* shimmersensor, Webcam* webcam) : settingsManager("settings.json") {
    LOG_INIT_CERR();

    this->shimmersensor = shimmersensor;
    this->webcam = webcam;
    //this->eeg = eeg;
    //this->movinghead = movinghead;

    // Load settings
    if (!settingsManager.loadSettings()) {
        log(LOG_ERROR) << "Failed to load settings\n";
    }
}