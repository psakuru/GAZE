#include<iostream>
#include<vector>
#include <gaze/gametree.hpp>
#include <gaze/algorithms/alpha-beta.hpp>

static int nodecount=0;

class state {
  int id;
public:
  state() {
    id = nodecount++;
  }

  std::vector<state*>* get_children() {
    auto lst = new std::vector<state*>;
    lst->push_back(new state);
    lst->push_back(new state);
    return lst;
  }

  bool operator==(const state& other) {
    return id == other.id;
  }

  int get_value() {
    return id;
  }
  static const int min_state_value = INT_MIN;
  static const int max_state_value = INT_MAX;
};

std::ostream& operator<<(std::ostream& os, state& st)
{
  return os<<st.get_value();
}

int main()
{
  gaze::game_tree<state> gt;
  for(int i=0;i<10;i++) {
    auto suggestion = gaze::alphabeta(gt, 2, INT_MIN, INT_MAX, i%2);
    gt.set_current_state(suggestion.second->get_state());
    std::cout<<gt<<std::endl;
  }
}
