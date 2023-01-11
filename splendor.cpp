#include "splendor.h"

namespace splendor
{
  void splendor::PlayerState::modify_tokens(int _tokens[6], int direction)
  {
    for (int i = 0; i < 6; i++)
    {
      tokens[i] += _tokens[i] * direction;
    }
  }

  void splendor::PlayerState::increase_card_color(int color)
  {
    cards[color]++;
  }

  void splendor::Model::modify_tokens(int _tokens[6], int direction)
  {
    for (int i = 0; i < 6; i++)
    {
      tokens[i] += _tokens[i] * direction;
    }
  }

  bool splendor::PlayerState::can_afford_card(int _tokens[6])
  {
    bool accumulator = true;
    for (int i = 0; i < 6; i++)
    {
      accumulator = accumulator && (tokens[i] + cards[i] >= _tokens[i]);
    }
    return accumulator;
  }

  int *splendor::PlayerState::get_cost(int _tokens[6])
  {
    for (int i = 0; i < 5 /* omit joker */ ; i++)
    {
      _tokens[i] = std::max(0, _tokens[i] - cards[i]);
    }
    return _tokens;
  }

  splendor::Card &splendor::Model::get_active_card(int card_id)
  {
    int card_row = card_id / CARDS_MAX_X;   
    int card_column = card_id % CARDS_MAX_X;
    return active_cards[card_row][card_column];
  }
}