
#include <boost/program_options.hpp>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <random>
#include <algorithm>
#include <curses.h>
#include <optional>
#include <ranges>
#include "splendor.h"

#define CARDS_PANE 0
#define PLAYER_PANE 1
#define TOKENS_PANE 2

namespace splendor
{
  struct PlayerWindowGroup
  {
    WINDOW *main;
    WINDOW *score_window; // groups tokens & cards
    WINDOW *tokens_window;
    WINDOW *cards_window;
    WINDOW *reserved_cards_main_window; // groups reserved_cards[]
    WINDOW *reserved_cards[3];
  };

  struct DisplayState
  {
    int selected_pane;
    int selected_active_card;
    int selected_reserved_card;
    int selected_token_0;
    int selected_token_1;
    int selected_token_2;
  };

  struct Display
  {
    WINDOW *cards_window;
    std::vector<WINDOW *> tier1_windows;
    std::vector<WINDOW *> tier2_windows;
    std::vector<WINDOW *> tier3_windows;
    std::vector<PlayerWindowGroup> player_windows;
    splendor::DisplayState state;
    void initialize(const splendor::Model &model);
    void refresh_display(const splendor::Model &model);
    void interact(splendor::Model &model);
    ~Display();
  };

  struct CLI
  {
    std::optional<splendor::Model> get_model(int ac, char **av);
  };

  struct UI
  {
    splendor::Display display;
    splendor::CLI cli;
  };
}