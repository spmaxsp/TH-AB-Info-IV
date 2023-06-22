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
    this->gstate.TargetPosX = rand() % 800 + 100;
    this->gstate.TargetPosY = rand() % 800 + 100;
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

    this->movement_fence = std::chrono::steady_clock::now();
}

void GameLogic::makeMovement(int movement) {
    if (this->settingsManager.global.virtual_head) {
        if (movement == MOVE_UP) {
            this->gstate.CurrentPosY = this->gstate.CurrentPosY - 50;
        }
        else if (movement == MOVE_DOWN) {
            this->gstate.CurrentPosY = this->gstate.CurrentPosY + 50;
        }
        else if (movement == MOVE_LEFT) {
            this->gstate.CurrentPosX = this->gstate.CurrentPosX - 50;
        }
        else if (movement == MOVE_RIGHT) {
            this->gstate.CurrentPosX = this->gstate.CurrentPosX + 50;
        }
    }
    else {
        if (movement == MOVE_UP) {
            this->movinghead->move_up();
        }
        else if (movement == MOVE_DOWN) {
            this->movinghead->move_down();
        }
        else if (movement == MOVE_LEFT) {
            this->movinghead->move_left();
        }
        else if (movement == MOVE_RIGHT) {
            this->movinghead->move_right();
        }
    }
}

int GameLogic::determineMovement() {
    if (this->settingsManager.global.sensor == SENSOR_EEG) {

    }
    else if (this->settingsManager.global.sensor == SENSOR_VIRTUAL) {
        int movement = this->manual_move;
        this->manual_move = MOVE_NONE;
        return movement;
    }
    else if (this->settingsManager.global.sensor == SENSOR_SHIMMER) {
        if (this->shimmersensor->get_Accel_x() > 2200) {
            return MOVE_UP;
        }
        if (this->shimmersensor->get_Accel_x() < 1800) {
            return MOVE_DOWN;
        }

        if (this->shimmersensor->get_Accel_y() > 2200) {
            return MOVE_LEFT;
        }
        if (this->shimmersensor->get_Accel_y() < 1800) {
            return MOVE_RIGHT;
        }
        else {
            return MOVE_NONE;
        }
    }
}

void GameLogic::updateGame(){

    // Update sensors
    this->updateSensors();

    // Update webcam
    this->webcam->getImage();

    if (!this->gstate.GameRunning) {
        return;
    }

    if (this->dstate.show_pause) {
        return;
    }

    
    // Move Head
    const std::chrono::duration<double> movementInterval(0.2);
    if (std::chrono::steady_clock::now() > movement_fence) {
        int movement = this->determineMovement();
        this->makeMovement(movement);
        auto movementDuration = std::chrono::duration_cast<std::chrono::steady_clock::duration>(movementInterval);
        movement_fence += movementDuration;
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
#ifdef _WIN32
    system(".\\scripts\\kill_python.bat");
#else
    system("chmod +x ./scripts/kill_python.sh");
    system("./scripts/kill_python.sh");
#endif
}

void GameLogic::QuitApp(){

    // Exit python
    if (this->shimmersensor->getRunningState()) {
        this->shimmersensor->disconnect();
        this->shimmersensor->stop();
    }
    if (this->eeg->getRunningState()) {
        this->eeg->disconnect();
        this->eeg->stop();
    }
    if (this->movinghead->getRunningState()) {
        this->movinghead->disconnect();
        this->movinghead->stop();
    }
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

void GameLogic::updateSensors(){
    const std::chrono::duration<double> executionInterval(1.0/this->settingsManager.shimmer.pollrate);

    if (this->shimmersensor->getConnectedState()) {
        if (!this->shimmersensor->getStreamEnabled()) {
            this->shimmersensor->startStream();
            shimmer_polling_fence = std::chrono::steady_clock::now();
        }
        if (std::chrono::steady_clock::now() >= shimmer_polling_fence){
            this->shimmersensor->readDataStream();
            
            auto executionDuration = std::chrono::duration_cast<std::chrono::steady_clock::duration>(executionInterval);
            shimmer_polling_fence += executionDuration;
        }
    }

    if (std::chrono::steady_clock::now() >= eeg_polling_fence){
        if (this->eeg->getConnectedState()) {
            if (!this->eeg->getInitState()){
                this->eeg->pollInitState();
            }
            shimmer_polling_fence = std::chrono::steady_clock::now();
        }

        if (this->eeg->getStreamEnabled()) {
                this->eeg->readDataStream();
        }
        auto executionDuration = std::chrono::duration_cast<std::chrono::steady_clock::duration>(executionInterval);
        eeg_polling_fence += executionDuration;
    }


}