#include<vector>
#include "../gametree.hpp"
#include "../algorithms/alpha-beta.hpp"

using namespace std;

static int nodecount=0;
static int values[] = {3, 2, 4, 7, 9, 11, 13, 8, 6, 3, 4, 1, 100, 77, 89, 32};

class state
{
  int id;

public:
  state() {
    id = nodecount = 0;
  }

  state(int no): id(no) {}

  int get_value() {
    if(id<sizeof(values)/sizeof(*values))
      return values[id];
    return id;
  }

  static int min_state_value() {
    return INT_MIN;
  }

  static int max_state_value() {
    return INT_MAX;
  }

  bool operator==(const state& other) {
    return id == other.id;
  }

  unique_ptr<vector<state*>> get_children() {
    auto lst = make_unique<vector<state*>>();
    lst->push_back(new state(++nodecount));
    lst->push_back(new state(++nodecount));

    return lst;
  }

  friend ostream& operator<<(ostream&, state&);
};

ostream& operator<<(ostream& os, state& st)
{
  return os<<st.get_value();
}

void alpha_beta_tests()
{
  typedef gaze::game_tree<state> game_tree_t;
  game_tree_t gt;
  auto pair_i = gaze::alphabeta<game_tree_t>(gt, 2, INT_MIN, INT_MAX, true);
  assert(pair_i.first);
  assert(pair_i.second);

  gt.set_current_state(pair_i.second->get_state());
  auto pair_i_2 = gaze::alphabeta<game_tree_t>(gt, 2, INT_MIN, INT_MAX, false);
  assert(pair_i_2.first);
  assert(pair_i_2.second);

  gt.set_current_state(pair_i_2.second->get_state());
  auto pair_i_3 = gaze::alphabeta<game_tree_t>(gt, 2, INT_MIN, INT_MAX, true);
  assert(pair_i_3.first);
  assert(pair_i_3.second);
}

int main()
{
  alpha_beta_tests();
}
