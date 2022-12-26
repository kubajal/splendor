
#include <boost/program_options.hpp>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <random>
#include <algorithm>
#include <curses.h>
#include "splendor.h"

namespace splendor
{
  struct Display
  {
    std::vector<WINDOW*> tier1_windows;
    std::vector<WINDOW*> tier2_windows;
    std::vector<WINDOW*> tier3_windows;
    std::vector<WINDOW*> player_windows;
    void initialize(splendor::Model &model);
    void refresh_display(splendor::Model &model);
    ~Display();
  };

  struct UI
  {
    splendor::Model model;
    splendor::Display display;
    UI(int ac, char **av);
    void show_table();
    void interact();
  };
}