#ifndef GAMETREE_HPP
#define GAMETREE_HPP

#include<boost/graph/adjacency_list.hpp>
#include<boost/graph/graph_traits.hpp>

namespace gaze {

template<typename edge_iter, typename vertex>
class viterator : public std::iterator<std::forward_iterator_tag, vertex>{
  edge_iter it;
public:
  viterator(edge_iter it): it(it) {}
  viterator& operator=(const viterator& rhs) { return *this; }
  viterator& operator++() {++it; return *this;};
  viterator operator++(int) {viterator res(*this); ++(*this); return res;}
  bool operator==(const viterator& rhs) { return it==rhs.it; }
  bool operator!=(const viterator& rhs) { return it!=rhs.it; }
  vertex& operator*() { return *it; }
};

template<typename game_tree>
class vertex {
public:
  typedef typename game_tree::graph graph;
  typedef typename game_tree::state state;
  typedef vertex<game_tree> vertex_property;
  typedef typename boost::graph_traits<graph>::out_edge_iterator graph_edge_it;
  typedef typename gaze::viterator<graph_edge_it, vertex_property> vertex_iterator;

  typedef typename boost::graph_traits<graph>::vertex_descriptor vertex_descriptor;

  vertex() {}
  vertex(state* st, vertex_descriptor vd, game_tree* gt): st(st), vd(vd), gt(gt), g(&gt->g) {}

  state& get_state() { return *st; }

  std::pair<vertex_iterator, vertex_iterator> get_children() {
    if(!children_added){
      auto container = st->get_children();
      for(auto it=container.begin(); it!=container.end(); it++){
        //vertex_property *nvert = new vertex_property(it, gt);
        auto tvd = boost::add_vertex(*g);
        (*g)[tvd] = *new vertex_property(&*it, tvd, gt);
        boost::add_edge(vd, tvd, *g);
      }
    }
    typename boost::graph_traits<graph>::out_edge_iterator begin_edge_it, end_edge_it;
    boost::tie(begin_edge_it, end_edge_it) = boost::out_edges(vd, *g);

    return std::make_pair(vertex_iterator(begin_edge_it), vertex_iterator(end_edge_it));
  }

  bool operator==(const vertex& vt) {
    return vd==vt.vd;
  }
  size_t get_children_count();
  //friend class viterator<vertex, graph>;
private:
  vertex_descriptor vd = -1;
  state* st = nullptr;
  game_tree* gt = nullptr;
  graph* g = nullptr;
  bool children_added = false;
};

template<typename game_state>
class game_tree {
public:
  typedef game_state state;
  typedef vertex<game_tree> vertex_property;
  typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
                        vertex_property> graph;
  typedef typename boost::graph_traits<graph>::vertex_descriptor vertex_descriptor;

  typedef typename vertex_property::vertex_iterator vertex_iterator;

  game_tree(state* st) {
    auto vd = boost::add_vertex(g);
    cur_vertex = new vertex_property(st, vd, this);
    g[vd] = *cur_vertex;
  }
  game_tree(const game_tree& otherTree);

  //to be called for all moves
  void set_current_state(state committedstate) {
  }
  //returns the previously committed vetex, used by algo
  vertex_property& get_current_vertex() {
    return *cur_vertex;
  }
  //returns the previously committed state, used by game designer
  state& get_current_state() {
    return cur_vertex->get_state();
  }
  graph g;
private:
  vertex_property *cur_vertex;
};

}

#endif
