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
    this->dstate.show_main_menue = false;
    this->gstate.TargetPosX = rand() % 1000;
    this->gstate.TargetPosY = rand() % 1000;
    this->gstate.CurrentPosX = 500;
    this->gstate.CurrentPosY = 500;
}

void GameLogic::testPythonPath(){

}

void GameLogic::killPythonProcesses(){

}

void GameLogic::QuitApp(){

}

void GameLogic::pauseGame(){
    this->gstate.GameRunning = false;
    this->dstate.show_pause = true;
}

void GameLogic::resumeGame(){
    this->gstate.GameRunning = true;
    this->dstate.show_pause = false;
}

void GameLogic::endGame(){
    this->gstate.GameRunning = false;
    this->dstate.show_pause = false;
    this->dstate.show_main_menue = true;
}