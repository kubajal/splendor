#include "splendor.h"

namespace splendor
{
  void splendor::PlayerState::modify_tokens(const char _tokens[6], int direction)
  {
    for(int i = 0; i < 6; i++)
    {
      tokens[i] += _tokens[i] * direction;
    }
  }

  void splendor::Model::modify_tokens(const char _tokens[6], int direction)
  {
    for(int i = 0; i < 6; i++)
    {
      tokens[i] += _tokens[i] * direction;
    }
  }
}