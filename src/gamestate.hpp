#pragma once

struct gamestate {
    bool GameRunning;
    bool AppRunning;

    int TargetPosX;
    int TargetPosY;

    int CurrentPosX;
    int CurrentPosY;

    int CurrentTime;

    int CurrentScore;
};
