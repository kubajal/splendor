#include <vector>
#include <string>

#define MAX_PLACES 100
#define CARDS_MAX_X 4
#define CARDS_MAX_Y 3

#define TOKENS_PANE_COLOR 251
#define PLAYER_CARD_TOKENS 252
#define PLAYER_CARD_CARDS 253
#define PLAYER_CARD_RESERVED 254
#define PLAYER_CARD_MAIN 255
#define TABLE_BACKGROUND 256

#define WHITE 0
#define BLUE 1
#define GREEN 2
#define RED 3
#define BLACK 4
#define JOKER 5

namespace splendor
{
  struct Card
  {
    int tier;
    int value;
    int type;
    int green;
    int white;
    int blue;
    int black;
    int red;
  };

  struct Config {
    int player_tokens_max;
    int tokens_on_stack_N;
    int players_N;
    int reserved_cards_max;
    int joker_tokens_N;
    std::vector<splendor::Card> cards;
  };

  struct PlayerState {
    // number of cards the player bought
    int tokens[6]; // 0=white, 1=blue, 2=green, 3=red, 4=black, 5=joker
    int green_cards;
    int white_cards;
    int blue_cards;
    int black_cards;
    int red_cards;
    std::vector<splendor::Card> reserved_cards;
    int points;
  };

  struct Model {
    Config config;
    int tokens[6]; // 0=white, 1=blue, 2=green, 3=red, 4=black, 5=joker
    splendor::Card active_cards[CARDS_MAX_Y][CARDS_MAX_X];
    std::vector<splendor::Card> card_stack[CARDS_MAX_Y];
    std::vector<splendor::PlayerState> players;
    
    int active_player;
  };

}
