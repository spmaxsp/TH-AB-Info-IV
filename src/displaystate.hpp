#pragma once

#include <string>

struct displaystate {
    bool show_main_menue;
    bool show_pause;
    bool show_settings;
    bool show_generic_err;
    std::string error_msg;
    bool show_py_err;
    bool show_gameover;
};
