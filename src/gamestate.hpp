#pragma once

struct gamestate {
    bool running;
    bool sensor_shimmer_connected;
    bool sensor_eeg_connected;

    bool show_settings;
    bool show_menu;

    bool pause;
};
