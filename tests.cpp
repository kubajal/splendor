#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "splendor.h"

using namespace splendor;

TEST(TransitionCanFire, ZeroTransitions_CantFire)
{
  PetriNet p = {};
  bool t0_can_fire = p.can_fire(0);
  ASSERT_FALSE(t0_can_fire);
};

TEST(TransitionCanFire, OneTransition_CanFire)
{
  splendor::Transition t0 = {
      std::vector<Edge>{{0, 1}},
      std::vector<Edge>{{1, 1}}};
  PetriNet p = {
    {1, 0},
    {t0}
  };
  bool t0_can_fire = p.can_fire(0);
  ASSERT_TRUE(t0_can_fire);
};

TEST(TransitionCanFire, OneTransition_CantFire)
{
  splendor::Transition t0 = {
      std::vector<Edge>{{0, 1}},
      std::vector<Edge>{{1, 1}}};
  PetriNet p = {
    {0, 0},
    {t0}
  };
  bool t0_can_fire = p.can_fire(0);
  ASSERT_FALSE(t0_can_fire);
};

TEST(TransitionCanFire, TwoTransitions_FirstCanFire)
{
  splendor::Transition t0 = {
      std::vector<Edge>{{0, 1}},
      std::vector<Edge>{{1, 1}}};
  splendor::Transition t1 = {
      std::vector<Edge>{{1, 1}},
      std::vector<Edge>{{0, 1}}};
  PetriNet p = {
    {0, 1},
    {t0, t1}
  };
  bool t0_can_fire = p.can_fire(0);
  bool t1_can_fire = p.can_fire(1);
  ASSERT_FALSE(t0_can_fire);
  ASSERT_TRUE(t1_can_fire);
};

TEST(TransitionCanFire, TwoTransitions_SecondCanFire)
{
  splendor::Transition t0 = {
      std::vector<Edge>{{0, 1}},
      std::vector<Edge>{{1, 1}}};
  splendor::Transition t1 = {
      std::vector<Edge>{{1, 1}},
      std::vector<Edge>{{0, 1}}};
  PetriNet p = {
    {1, 0},
    {t0, t1}
  };
  bool t0_can_fire = p.can_fire(0);
  bool t1_can_fire = p.can_fire(1);
  ASSERT_TRUE(t0_can_fire);
  ASSERT_FALSE(t1_can_fire);
};

TEST(TransitionCanFire, TwoTransitions_BothCanFire)
{
  splendor::Transition t0 = {
      std::vector<Edge>{{0, 1}},
      std::vector<Edge>{{1, 1}}};
  splendor::Transition t1 = {
      std::vector<Edge>{{1, 1}},
      std::vector<Edge>{{0, 1}}};
  PetriNet p = {
    {1, 1},
    {t0, t1}
  };
  bool t0_can_fire = p.can_fire(0);
  bool t1_can_fire = p.can_fire(1);
  ASSERT_TRUE(t0_can_fire);
  ASSERT_TRUE(t1_can_fire);
};

TEST(TransitionCanFire, TwoTransitions_BothCantFire)
{
  splendor::Transition t0 = {
      std::vector<Edge>{{0, 1}},
      std::vector<Edge>{{1, 1}}};
  splendor::Transition t1 = {
      std::vector<Edge>{{1, 1}},
      std::vector<Edge>{{0, 1}}};
  PetriNet p = {
    {0, 0},
    {t0, t1}
  };
  bool t0_can_fire = p.can_fire(0);
  bool t1_can_fire = p.can_fire(1);
  ASSERT_FALSE(t0_can_fire);
  ASSERT_FALSE(t1_can_fire);
};

TEST(TransitionFire, OneTransition_Simple)
{
  splendor::Transition t0 = {
      std::vector<Edge>{{0, 1}},
      std::vector<Edge>{{1, 1}}};
  PetriNet p = {
    {1, 0},
    {t0}
  };
  int fire_result = p.fire(0);
  ASSERT_EQ(fire_result, 0);
  ASSERT_EQ(p.tokens[0], 0);
  ASSERT_EQ(p.tokens[1], 1);
};

TEST(TransitionFire, OneTransition_TwoIncomingEdges)
{
  splendor::Transition t0 = {
      std::vector<Edge>{{0, 5}, {1, 6}},
      std::vector<Edge>{{2, 7}}};
  PetriNet p = {
    {5, 6, 0},
    {t0}
  };
  int fire_result = p.fire(0);
  ASSERT_EQ(fire_result, 0);
  ASSERT_EQ(p.tokens[0], 0);
  ASSERT_EQ(p.tokens[1], 0);
  ASSERT_EQ(p.tokens[2], 7);
};

TEST(TransitionFire, OneTransition_TwoOutgoingEdges)
{
  splendor::Transition t0 = {
      std::vector<Edge>{{0, 3}},
      std::vector<Edge>{{1, 4}, {2, 5}}};
  PetriNet p = {
    {3, 0, 0},
    {t0}
  };
  int fire_result = p.fire(0);
  ASSERT_EQ(fire_result, 0);
  ASSERT_EQ(p.tokens[0], 0);
  ASSERT_EQ(p.tokens[1], 4);
  ASSERT_EQ(p.tokens[2], 5);
};

TEST(TransitionFire, OneTransition_TwoIncomingAndTwoOutgoingEdges)
{
  splendor::Transition t0 = {
      std::vector<Edge>{{0, 1}, {1, 2}},
      std::vector<Edge>{{2, 3}, {3, 4}}};
  PetriNet p = {
    {1, 2, 0, 0},
    {t0}
  };
  int fire_result = p.fire(0);
  ASSERT_EQ(fire_result, 0);
  ASSERT_EQ(p.tokens[0], 0);
  ASSERT_EQ(p.tokens[1], 0);
  ASSERT_EQ(p.tokens[2], 3);
  ASSERT_EQ(p.tokens[3], 4);
};
