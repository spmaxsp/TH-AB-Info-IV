#pragma once

#include <BSlogger.hpp>

#include "gamestate.hpp"
#include "displaystate.hpp"

#include "cv2_cam/webcam.hpp"

#include "settings.hpp"

#include "py_extention/modules/Shimmersensor/Shimmersensor.hpp"
//#include "py_extention/modules/EEG/EEG.hpp"
//#include "py_extention/modules/Movinghead/Movinghead.hpp"

#include <chrono>


class GameLogic {
    public:
        gamestate gstate;
        displaystate dstate;

        Shimmersensor* shimmersensor;
        //EEG* eeg;
        //Movinghead* movinghead;

        SettingsManager settingsManager;

        Webcam* webcam;

        GameLogic(Shimmersensor* shimmersensor, Webcam* webcam);

        void startGame();
        void testPythonPath();
        void killPythonProcesses();
        void QuitApp();
        void pauseGame();
        void resumeGame();
        void updateGame();
        void endGame();
    
    private:
        std::chrono::steady_clock::time_point start_time;

        int time_to_reach_target = 0;

        void setNewTargetPosition();

};
    