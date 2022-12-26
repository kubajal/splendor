#include "ui.h"

namespace po = boost::program_options;
using json = nlohmann::json;
#define CARD_SIZE_X 15
#define CARD_SIZE_Y 7
#define PLAYER_WINDOW_SIZE_Y 9
#define PLAYER_WINDOW_SIZE_X 80

std::optional<splendor::Config> load_default_config(std::string file_path)
{
  std::ifstream input_stream(file_path);
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
      0,
      0,
      true};
}

splendor::UI::UI(int ac, char **av)
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
      model = create_model(config_value);
      display.initialize(model);
    }
  }
}

void splendor::UI::show_table()
{
  display.refresh_display(model);
}

void splendor::UI::interact()
{
  int c = getch();
  switch(c)
  {
    case KEY_UP:
      model.selected = std::max(0, model.selected - model.config.row_cards_active_max);
      break;
    case KEY_DOWN:
      model.selected = std::min(3 * model.config.row_cards_active_max - 1, model.selected + model.config.row_cards_active_max);
      break;
    case KEY_LEFT:
      model.selected = std::max(0, model.selected - 1);
      break;
    case KEY_RIGHT:
      model.selected = std::min(3 * model.config.row_cards_active_max - 1, model.selected + 1);
      break;
    default:
      break;
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
  wrefresh(win);
}

void draw_player(WINDOW *win, const splendor::PlayerState player)
{
  char buffer[256];
  sprintf(buffer, "TOKENS | White: %d, Blue: %d, Green: %d, Red: %d, Black: %d, Joker: %d\n", player.white_tokens, player.blue_tokens, player.green_tokens, player.red_tokens, player.black_tokens, player.joker_tokens);
  sprintf(buffer + strlen(buffer), "CARDS  | White: %d, Blue: %d, Green: %d, Red: %d, Black: %d\n", player.white_cards, player.blue_cards, player.green_cards, player.red_cards, player.black_cards);
  wbkgdset(win, COLOR_PAIR(PLAYER_CARD));
  wclear(win);
  wprintw(win, buffer);
  wrefresh(win);
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
  wrefresh(win);
}

void splendor::Display::initialize(splendor::Model &model)
{
  initscr();
  refresh();
  noecho();
  keypad(stdscr, TRUE);

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
  init_pair(PLAYER_CARD, COLOR_BLACK, COLOR_YELLOW);

  bkgdset(COLOR_PAIR(TABLE_BACKGROUND));
  refresh();

  int row_cards_active_max = model.config.row_cards_active_max;

  tier1_windows.resize(row_cards_active_max);
  tier2_windows.resize(row_cards_active_max);
  tier3_windows.resize(row_cards_active_max);

  for (int i = 0; i < row_cards_active_max; i++)
  {
    tier1_windows[i] = newwin(CARD_SIZE_Y, CARD_SIZE_X, 0, i * (CARD_SIZE_X + 3) + 1);
    tier2_windows[i] = newwin(CARD_SIZE_Y, CARD_SIZE_X, CARD_SIZE_Y + 1, i * (CARD_SIZE_X + 3) + 1);
    tier3_windows[i] = newwin(CARD_SIZE_Y, CARD_SIZE_X, 2 * (CARD_SIZE_Y + 1), i * (CARD_SIZE_X + 3) + 1);
    draw_card(tier1_windows[i], model.tier1[i], model.selected == i);
    draw_card(tier2_windows[i], model.tier2[i], model.selected == (i + row_cards_active_max));
    draw_card(tier3_windows[i], model.tier3[i], model.selected == (i + 2*row_cards_active_max));
  }

  player_windows.resize(model.players.size());
  for(int i = 0; i < model.players.size(); i++)
  {
    player_windows[i] = newwin(PLAYER_WINDOW_SIZE_Y, PLAYER_WINDOW_SIZE_X, row_cards_active_max * CARD_SIZE_Y + i * (PLAYER_WINDOW_SIZE_Y+1) - 4, 0);
    draw_player(player_windows[i], model.players[i]);
  }
}

void splendor::Display::refresh_display(splendor::Model &model)
{
  int row_cards_active_max = tier1_windows.size();
  for (int i = 0; i < row_cards_active_max; i++)
  {
    update_card(tier1_windows[i], model.tier1[i], model.selected == i);
    update_card(tier2_windows[i], model.tier2[i], model.selected == (i + row_cards_active_max));
    update_card(tier3_windows[i], model.tier3[i], model.selected == (i + 2*row_cards_active_max));
  }
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
  for(int i = 0; i < player_windows.size(); i++)
  {
    delwin(player_windows[i]);
  }
  endwin();
}
