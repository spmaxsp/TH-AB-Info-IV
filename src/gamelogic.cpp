#include "gamelogic.hpp"

GameLogic::GameLogic(Shimmersensor* shimmersensor, EEG* eeg, Movinghead* movinghead, Webcam* webcam) : settingsManager("settings.json") {
    LOG_INIT_CERR();

    this->gstate = {};
    this->dstate = {};

    this->gstate.GameRunning = false;
    this->gstate.AppRunning = true;

    this->dstate.show_main_menue = true;

    this->shimmersensor = shimmersensor;
    this->webcam = webcam;
    this->eeg = eeg;
    this->movinghead = movinghead;

    // Load settings
    if (!settingsManager.loadSettings()) {
        log(LOG_ERROR) << "Failed to load settings\n";
    }

    // init webcam
    this->webcam->initDevice(settingsManager.global.camera_index);

    // Get first frame from webcam
    this->webcam->getImage();

    // Test if python was exited correctly
    if (!this->settingsManager.python.clean_exit_flag) {
        this->dstate.show_py_err = true;
        this->dstate.show_main_menue = false;
    }

    // Set flag to detect if python was exited correctly
    this->settingsManager.python.clean_exit_flag = false;
    this->settingsManager.saveSettings();
}

void GameLogic::setNewTargetPosition(){
    this->gstate.TargetPosX = rand() % 1000;
    this->gstate.TargetPosY = rand() % 1000;
}

void GameLogic::startGame(){
    this->gstate.GameRunning = true; 
    this->dstate.show_main_menue = false;
    this->dstate.show_gameover = false;
    this->dstate.show_pause = false;

    this->setNewTargetPosition();

    this->gstate.CurrentPosX = 500;
    this->gstate.CurrentPosY = 500;

    this->gstate.CurrentScore = 0;
    
    if (this->settingsManager.global.time == TIME_30)      { time_to_reach_target = 30; } 
    else if (this->settingsManager.global.time == TIME_60) { time_to_reach_target = 60; }
    else if (this->settingsManager.global.time == TIME_90) { time_to_reach_target = 90; }
    else { time_to_reach_target = 90; }

    this->start_time = std::chrono::steady_clock::now();
}

void GameLogic::updateGame(){

    // Update webcam
    this->webcam->getImage();

    if (!this->gstate.GameRunning) {
        return;
    }

    // Compute time left
    auto current_time = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = current_time - this->start_time;

    this->gstate.CurrentTime = time_to_reach_target - elapsed_seconds.count();

    // gameover if time is up
    if (this->gstate.CurrentTime <= 0) {
        this->gstate.GameRunning = false;
        this->dstate.show_pause = false;
        this->dstate.show_gameover = true;
    }

    // Update highscore
    if (this->settingsManager.score.high_score < this->gstate.CurrentScore) {
        this->settingsManager.score.high_score = this->gstate.CurrentScore;
    }

    // Test if target is hit (close enough)
    if (abs(this->gstate.CurrentPosX - this->gstate.TargetPosX) < 50 && abs(this->gstate.CurrentPosY - this->gstate.TargetPosY) < 50) {
        this->gstate.CurrentScore = this->gstate.CurrentScore + 1;
        this->setNewTargetPosition();
    }
}

void GameLogic::testPythonPath(){

}

void GameLogic::killPythonProcesses(){

}

void GameLogic::QuitApp(){

    // Exit python
    this->settingsManager.python.clean_exit_flag = true;

    // Save settings
    this->settingsManager.saveSettings();

}

void GameLogic::pauseGame(){
    this->gstate.GameRunning = false;
    this->dstate.show_pause = true;

    time_to_reach_target = this->gstate.CurrentTime;
}

void GameLogic::resumeGame(){
    this->gstate.GameRunning = true;
    this->dstate.show_pause = false;

    this->start_time = std::chrono::steady_clock::now();
}

void GameLogic::endGame(){
    this->gstate.GameRunning = false;
    this->dstate.show_pause = false;
    this->dstate.show_main_menue = true;
}