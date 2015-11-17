#ifndef GAMETREE_HPP
#define GAMETREE_HPP

namespace gaze {

template<typename state>
class vertex {
private:
  vertex(state&);
  vertex(state&&);//move constructor
public:
  state& get_state();
  pair<vertex_iterator, vertex_iterator> get_children();
  size_t get_children_count();
}

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

}

#endif



state {
}

namespace {
state& get_parent(state& curr, game_tree& gt){
  boost::in_edges(curr, gt);
  source(curr, gt);
}
}
