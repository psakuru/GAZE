#ifndef GAMETREE_HPP
#define GAMETREE_HPP

namespace gaze {

template<typename state>
class game_tree {
public:
  typedef typename vertex<state> vertex_descriptor;

  game_tree(state initialstate);
  game_tree(const game_tree& otherTree);

  //to be called for all moves
  void set_current_state(state committedstate);
  //returns the previously committed vetex, used by algo
  vertex& get_current_vertex();
  //returns the previously committed state, used by game designer
  state& get_current_state();
};

class vertex_properties {
};

template<typename game_tree, typename state>
state& get_previous(state& curr, game_tree& gt);

template<typename game_tree, typename state>
size_t get_children_count(state& curr, game_tree& gt);

template<typename game_tree, typename state>
pair<graph_traits<game_tree>::vertex_iterator, graph_traits<game_tree>::vertex_iterator>
    get_children(state& curr, game_tree& gt);
}

#endif
