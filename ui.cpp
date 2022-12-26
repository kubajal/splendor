#include "ui.h"

namespace po = boost::program_options;
using json = nlohmann::json;
#define CARD_SIZE_X 13
#define CARD_SIZE_Y 7
#define PLAYER_WINDOW_SIZE_Y 11
#define PLAYER_WINDOW_SIZE_X 69

std::optional<splendor::Config> load_default_config(std::string file_path)
{
  std::ifstream input_stream(file_path);
  if(input_stream.fail()) {
    return {};
  }
  json data = json::parse(input_stream);
  splendor::Config config = {
      data["player_tokens_max"],
      data["tokens_on_stack_max"],
      data["players_N"],
      data["reserved_cards_max"],
      data["joker_tokens_N"],
      data["row_cards_active_max"],
      {}};
  for (const auto &e : data["cards"])
  {
    config.cards.push_back({
        e["tier"],
        e["value"],
        e["type"],
        e["green"],
        e["white"],
        e["blue"],
        e["black"],
        e["red"],
    });
  }
  return config;
}

splendor::Model create_model(const splendor::Config &config)
{
  auto rng = std::default_random_engine{};

  std::vector<splendor::Card> shuffled_cards(config.cards);
  std::ranges::shuffle(shuffled_cards, rng);

  auto tier1 = shuffled_cards | std::views::filter([](auto &e)
                                                   { return e.tier == 1; });
  auto tier2 = shuffled_cards | std::views::filter([](auto &e)
                                                   { return e.tier == 2; });
  auto tier3 = shuffled_cards | std::views::filter([](auto &e)
                                                   { return e.tier == 3; });

  // this is stupid, there is no other way to cast range to vector in C++20
  std::vector<splendor::Card> tier1_v(tier1.begin(), tier1.end());
  std::vector<splendor::Card> tier2_v(tier2.begin(), tier2.end());
  std::vector<splendor::Card> tier3_v(tier3.begin(), tier3.end());

  std::vector<splendor::PlayerState> players(config.players_N);

  return splendor::Model{
      config,
      tier1_v,
      tier2_v,
      tier3_v,
      players,
      0,
      0,
      0,
      0};
}

std::optional<splendor::Model> splendor::CLI::get_model(int ac, char **av)
{
  po::options_description desc("Allowed options");
  desc.add_options()("help", "produce help message")("config-path", po::value<std::string>()->default_value("assets/config.json"), "path to Splendor config");
  po::variables_map vm;
  po::store(po::parse_command_line(ac, av, desc), vm);
  po::notify(vm);
  if (vm.count("help"))
  {
    std::cout << desc;
  }
  else
  {
    auto config = load_default_config(vm["config-path"].as<std::string>());
    if (config)
    {
      auto config_value = config.value();
      return create_model(config_value);
    }
  }
  return {};
}

void splendor::Display::interact(splendor::Model &model)
{
  int c = getch();
  switch (state.selected_pane)
  {
  case CARDS_PANE:
    switch (c)
    {
    case KEY_UP:
      state.selected_active_card = std::max(0, state.selected_active_card - model.config.row_cards_active_max);
      break;
    case KEY_DOWN:
      state.selected_active_card = std::min(3 * model.config.row_cards_active_max - 1, state.selected_active_card + model.config.row_cards_active_max);
      break;
    case KEY_LEFT:
      state.selected_active_card = std::max(0, state.selected_active_card - 1);
      break;
    case KEY_RIGHT:
      state.selected_active_card = std::min(3 * model.config.row_cards_active_max - 1, state.selected_active_card + 1);
      break;
    }
  default:
    switch (c)
    {
    case '\t':
      state.selected_pane = (state.selected_pane + 1) % 3;
      break;
    }
  }
}

void draw_card(WINDOW *win, const splendor::Card &card, bool selected)
{
  char buffer[256];
  sprintf(buffer, "  Points: %d\n  White:  %d\n  Blue:   %d\n  Green:  %d\n  Red:    %d\n  Black:  %d\n", card.value, card.white, card.blue, card.green, card.red, card.black);
  wbkgdset(win, COLOR_PAIR(card.type));
  wclear(win);
  wprintw(win, buffer);
  if (selected)
  {
    box(win, 'o', 'o');
  }
  else
  {
    box(win, '|', '-');
  }
  wnoutrefresh(win);
}

void draw_player(splendor::PlayerWindowGroup &window_group, const splendor::PlayerState player)
{
  char buffer[256];
  wbkgdset(window_group.main, COLOR_PAIR(TABLE_BACKGROUND));
  wbkgdset(window_group.score_window, COLOR_PAIR(PLAYER_CARD_MAIN));
  wbkgdset(window_group.tokens_window, COLOR_PAIR(PLAYER_CARD_CARDS));
  wbkgdset(window_group.cards_window, COLOR_PAIR(PLAYER_CARD_CARDS));
  wbkgdset(window_group.reserved_cards_main_window, COLOR_PAIR(TABLE_BACKGROUND));
  wclear(window_group.main);
  wclear(window_group.tokens_window);
  wclear(window_group.cards_window);
  wclear(window_group.reserved_cards_main_window);
  wclear(window_group.reserved_cards[0]);
  wclear(window_group.reserved_cards[1]);
  wclear(window_group.reserved_cards[2]);
  sprintf(buffer, "\n  TOKENS\n  White: %d\n  Blue:  %d\n  Green: %d\n  Red:   %d\n  Black: %d\n  Joker: %d\n", player.white_tokens, player.blue_tokens, player.green_tokens, player.red_tokens, player.black_tokens, player.joker_tokens);
  wprintw(window_group.tokens_window, buffer);
  sprintf(buffer, "\n  CARDS\n  White: %d\n  Blue:  %d\n  Green: %d\n  Red:   %d\n  Black: %d\n", player.white_cards, player.blue_cards, player.green_cards, player.red_cards, player.black_cards);
  wprintw(window_group.cards_window, buffer);
  box(window_group.reserved_cards_main_window, 'z', 'z');
  wnoutrefresh(window_group.main);
  wnoutrefresh(window_group.tokens_window);
  wnoutrefresh(window_group.cards_window);
  wnoutrefresh(window_group.reserved_cards_main_window);
  wnoutrefresh(window_group.reserved_cards[0]);
  wnoutrefresh(window_group.reserved_cards[1]);
  wnoutrefresh(window_group.reserved_cards[2]);
}

void update_card(WINDOW *win, const splendor::Card &card, bool selected)
{
  if (selected)
  {
    box(win, 'o', 'o');
  }
  else
  {
    box(win, '|', '-');
  }
  wnoutrefresh(win);
}

void splendor::Display::initialize(splendor::Model &model)
{
  initscr();
  noecho();
  keypad(stdscr, TRUE);
  refresh();

  state.selected_active_card = 0;
  state.selected_pane = CARDS_PANE;
  state.selected_reserved_card = 0;
  state.selected_token_0 = 0;
  state.selected_token_1 = 0;
  state.selected_token_2 = 0;

  if (!has_colors())
  {
    endwin();
    printf("Your terminal does not support color\n");
    exit(1);
  }
  start_color();

  init_pair(GREEN_CARD, COLOR_BLACK, COLOR_GREEN);
  init_pair(WHITE_CARD, COLOR_BLACK, COLOR_WHITE);
  init_pair(BLUE_CARD, COLOR_BLACK, COLOR_BLUE);
  init_pair(BLACK_CARD, COLOR_WHITE, COLOR_BLACK);
  init_pair(RED_CARD, COLOR_BLACK, COLOR_RED);
  init_pair(TABLE_BACKGROUND, COLOR_RED, COLOR_CYAN);
  init_pair(PLAYER_CARD_TOKENS, COLOR_BLACK, COLOR_MAGENTA);
  init_pair(PLAYER_CARD_CARDS, COLOR_BLACK, COLOR_CYAN);
  init_pair(PLAYER_CARD_RESERVED, COLOR_BLACK, COLOR_WHITE);
  init_pair(PLAYER_CARD_MAIN, COLOR_BLACK, COLOR_YELLOW);

  bkgdset(COLOR_PAIR(TABLE_BACKGROUND));

  int row_cards_active_max = model.config.row_cards_active_max;

  tier1_windows.resize(row_cards_active_max);
  tier2_windows.resize(row_cards_active_max);
  tier3_windows.resize(row_cards_active_max);

  cards_window = newwin(3 * (CARD_SIZE_Y + 1) + 1, row_cards_active_max * (CARD_SIZE_X + 1) + 3, 0, 0);
  for (int i = 0; i < row_cards_active_max; i++)
  {
    tier1_windows[i] = derwin(cards_window, CARD_SIZE_Y, CARD_SIZE_X, 1, i * (CARD_SIZE_X + 1) + 2);
    tier2_windows[i] = derwin(cards_window, CARD_SIZE_Y, CARD_SIZE_X, CARD_SIZE_Y + 2, i * (CARD_SIZE_X + 1) + 2);
    tier3_windows[i] = derwin(cards_window, CARD_SIZE_Y, CARD_SIZE_X, 2 * (CARD_SIZE_Y + 1) + 1, i * (CARD_SIZE_X + 1) + 2);
  }

  player_windows.resize(model.players.size());
  for (int i = 0; i < model.players.size(); i++)
  {
    player_windows[i].main = newwin(PLAYER_WINDOW_SIZE_Y, PLAYER_WINDOW_SIZE_X, row_cards_active_max * CARD_SIZE_Y + i * (PLAYER_WINDOW_SIZE_Y + 1) - 3, 0);
    player_windows[i].score_window = derwin(player_windows[i].main, PLAYER_WINDOW_SIZE_Y, 2 * (CARD_SIZE_X + 1) + 5, 0, 2);
    player_windows[i].tokens_window = derwin(player_windows[i].score_window, PLAYER_WINDOW_SIZE_Y - 2, CARD_SIZE_X, 1, 0);
    player_windows[i].cards_window = derwin(player_windows[i].score_window, PLAYER_WINDOW_SIZE_Y - 2, CARD_SIZE_X, 1, CARD_SIZE_X + 1);
    player_windows[i].reserved_cards_main_window = derwin(player_windows[i].main, PLAYER_WINDOW_SIZE_Y - 2, CARD_SIZE_X * 3, 1, 2 * (CARD_SIZE_X + 1));
    player_windows[i].reserved_cards[0] = derwin(player_windows[i].reserved_cards_main_window, CARD_SIZE_Y, CARD_SIZE_X, 1, 1);
    player_windows[i].reserved_cards[1] = derwin(player_windows[i].reserved_cards_main_window, CARD_SIZE_Y, CARD_SIZE_X, CARD_SIZE_X + 2, 1);
    player_windows[i].reserved_cards[2] = derwin(player_windows[i].reserved_cards_main_window, CARD_SIZE_Y, CARD_SIZE_X, 2 * (CARD_SIZE_X + 1) + 1, 1);
  }
  for (int i = 0; i < model.players.size(); i++)
  {
    draw_player(player_windows[i], model.players[i]);
  }
  for (int i = 0; i < row_cards_active_max; i++)
  {
    draw_card(tier1_windows[i], model.tier1[i], state.selected_active_card == i);
    draw_card(tier2_windows[i], model.tier2[i], state.selected_active_card == (i + row_cards_active_max));
    draw_card(tier3_windows[i], model.tier3[i], state.selected_active_card == (i + 2 * row_cards_active_max));
  }

  // draw_card & draw_player call wnoutrefresh
  // this finalizes drawing to the screen
  // https://linux.die.net/man/3/wrefresh
  doupdate();
  refresh();
 }

void splendor::Display::refresh_display(splendor::Model &model)
{
  switch (state.selected_pane)
  {
  case CARDS_PANE:
    wborder(player_windows[model.active_player].main, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    wnoutrefresh(player_windows[model.active_player].main);
    box(cards_window, '%', '%');
    wnoutrefresh(cards_window);
    break;
  case PLAYER_PANE:
    wborder(cards_window, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    wnoutrefresh(cards_window);
    box(player_windows[model.active_player].main, '%', '%');
    wnoutrefresh(player_windows[model.active_player].main);
    break;
  case TOKENS_PANE:
    break;
  }

  int row_cards_active_max = tier1_windows.size();
  for (int i = 0; i < row_cards_active_max; i++)
  {
    update_card(tier1_windows[i], model.tier1[i], state.selected_active_card == i && state.selected_pane == CARDS_PANE);
    update_card(tier2_windows[i], model.tier2[i], state.selected_active_card == (i + row_cards_active_max) && state.selected_pane == CARDS_PANE);
    update_card(tier3_windows[i], model.tier3[i], state.selected_active_card == (i + 2 * row_cards_active_max) && state.selected_pane == CARDS_PANE);
  }
  doupdate();
}

splendor::Display::~Display()
{
  int row_cards_active_max = tier1_windows.size();
  for (int i = 0; i < row_cards_active_max; i++)
  {
    delwin(tier1_windows[i]);
    delwin(tier2_windows[i]);
    delwin(tier3_windows[i]);
  }
  delwin(cards_window);
  for (int i = 0; i < player_windows.size(); i++)
  {
    delwin(player_windows[i].cards_window);
    delwin(player_windows[i].reserved_cards_main_window);
    delwin(player_windows[i].tokens_window);
    delwin(player_windows[i].main);
  }
  endwin();
}
