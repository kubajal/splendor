#include <vector>
#include <string>

#define MAX_PLACES 100

#define GREEN_CARD 1
#define WHITE_CARD 2
#define BLUE_CARD 3
#define BLACK_CARD 4
#define RED_CARD 5
#define PLAYER_CARD_TOKENS 252
#define PLAYER_CARD_CARDS 253
#define PLAYER_CARD_RESERVED 254
#define PLAYER_CARD_MAIN 255
#define TABLE_BACKGROUND 256

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
    int tokens_on_stack_max;
    int players_N;
    int reserved_cards_max;
    int joker_tokens_N;
    int row_cards_active_max;
    std::vector<splendor::Card> cards;
  };

  struct PlayerState {
    // number of tokens the player has
    int green_tokens;
    int white_tokens;
    int blue_tokens;
    int black_tokens;
    int red_tokens;
    int joker_tokens;
    // number of cards the player bought
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
    std::vector<splendor::Card> tier1;
    std::vector<splendor::Card> tier2;
    std::vector<splendor::Card> tier3;
    std::vector<splendor::PlayerState> players;
    int tier1_last_card;
    int tier2_last_card;
    int tier3_last_card;
    int active_player;
  };

}
