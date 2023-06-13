#include "gamelogic.hpp"

GameLogic::GameLogic(Shimmersensor* shimmersensor, Webcam* webcam) : settingsManager("settings.json") {
    LOG_INIT_CERR();

    this->gstate = {};
    this->dstate = {};

    this->dstate.show_main_menue = true;

    this->shimmersensor = shimmersensor;
    this->webcam = webcam;
    //this->eeg = eeg;
    //this->movinghead = movinghead;

    // Load settings
    if (!settingsManager.loadSettings()) {
        log(LOG_ERROR) << "Failed to load settings\n";
    }
}

void GameLogic::startGame(){
    this->gstate.GameRunning = true; 

}

void GameLogic::testPythonPath(){

}

void GameLogic::killPythonProcesses(){

}

void GameLogic::QuitApp(){

}