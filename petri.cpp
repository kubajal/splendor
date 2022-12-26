#include "splendor.h"
#include <ranges>
#include <algorithm>

namespace petri
{

  bool PetriNet::can_fire(int transition_id)
  {
    if(transition_id >= transitions.size()) {
      return false;
    }
    const auto &transition = transitions[transition_id];
    for (const auto &e : transition.incoming) {
      if(tokens[e.v] < e.cost) {
        return false;
      }
    }
    return true;
  }

  int PetriNet::fire(int transition_id)
  {
    if(!can_fire(transition_id)) {
      return 1;
    }
    const auto &transition = transitions[transition_id];
    for (const auto &e : transition.incoming) {
      tokens[e.v] = tokens[e.v] - e.cost;
    }
    for (const auto &e : transition.outgoing) {
      tokens[e.v] = tokens[e.v] + e.cost;
    }
    return 0;
  }
}