#include <vector>

#define MAX_PLACES 100

namespace petri {

  struct Edge
  {
    // source / target place id (depending on membership in incoming/outgoing vector)
    int v;
    // number of tokens consumed from / emitted to v if the transition was activated
    int cost;
  };

  struct Transition
  {
    std::vector<Edge> incoming;
    std::vector<Edge> outgoing;
  };

  struct PetriNet
  {
    int tokens[MAX_PLACES]; // number of tokens on each place
    std::vector<Transition> transitions;
    bool can_fire(int transition_id);
    int fire(int transition_id);
  };

}