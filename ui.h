
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
    WINDOW *reserved_card_window[3];
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
    WINDOW *cards_pane;
    WINDOW *tokens_pane;
    WINDOW *red_tokens_window;
    WINDOW *blue_tokens_window;
    WINDOW *black_tokens_window;
    WINDOW *white_tokens_window;
    WINDOW *green_tokens_window;
    WINDOW *card_windows[3][4];
    std::vector<PlayerWindowGroup> player_panes;
    splendor::DisplayState state;
    void initialize(const splendor::Model &model);
    void refresh_display(const splendor::Model &model);
    void reserve_card(int card_row, int card_column, splendor::Model &model);
    void interact(splendor::Model &model);
    void draw_card(WINDOW *win, const splendor::Card &card, bool selected);
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