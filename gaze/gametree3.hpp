#ifndef GAMETREE_HPP
#define GAMETREE_HPP

#include<boost/graph/adjacency_list.hpp>
#include<boost/graph/graph_traits.hpp>
#include<list>

#define DEBUG 1
#include "../logging.hpp"

namespace gaze {

template<typename state>
class vertex {
public:
  typedef std::list<vertex> child_vertices_container;
  typedef vertex<state> vertex_property;
  typedef typename child_vertices_container::iterator vertex_iterator;

  vertex() {}
  vertex(state& st, int level): st(st), level(level) {}

  state& get_state() { return st; }

  std::pair<vertex_iterator, vertex_iterator> get_children() {
    if(!children_added){
      add_children();
    }
    return std::make_pair(child_vertices.begin(), child_vertices.end());
  }

  bool operator==(const vertex& vt) {
    return st==vt.st && level==vt.level;
  }
  bool operator!=(const vertex& vt) {
    return !(*this==vt);
  }
  size_t get_children_count() {
    if(!children_added) {
      add_children();
    }
    return child_vertices.size();
  }

  std::ostream& print(std::ostream& os) {
    os<<"("<<get_state();
    if(children_added) {
      os<<" degree "<<get_children_count()<<": ";
      auto it_pair = get_children();
      for_each(it_pair.first, it_pair.second, [&](auto vert){ os<<vert; });
    }
    os<<")";
    return os;
  }
private:
  void add_children() {
    assert(!children_added);
    auto container = st.get_children();
    for_each(container.begin(), container.end(), [&](auto st){
      child_vertices.push_back(*new vertex_property(st, level+1));
    });
    children_added = true;
  }

  int level=0;
  state& st;
  bool children_added = false;
  child_vertices_container child_vertices;
};

template<typename game_state>
class game_tree {
public:
  typedef game_state state;
  typedef vertex<state> vertex_property;
  typedef typename vertex_property::vertex_iterator vertex_iterator;

  game_tree(state* st) {
    root_vertex = cur_vertex = new vertex_property(*st, 0);
  }
  game_tree(const game_tree& otherTree);

  //to be called for all moves
  void set_current_state(state committedstate) {
  }
  //returns the previously committed vetex, used by algo
  vertex_property& get_current_vertex() {return *cur_vertex;}
  //returns root vertex of the tree
  vertex_property& get_root_vertex() {return *root_vertex;}
  //returns the previously committed state, used by game designer
  state& get_current_state() {
    return cur_vertex->get_state();
  }

  std::ostream& print(std::ostream& os) {
    return os<<(*root_vertex);
  }
private:
  vertex_property *cur_vertex;
  vertex_property *root_vertex;
};

template<typename game_state>
std::ostream& operator<<(std::ostream& os, game_tree<game_state>& t)
{
#define PRINT 1
#if PRINT
  return t.print(os);
#else
  typedef typename game_tree<game_state>::graph graph;

  typedef typename boost::graph_traits<graph> GraphTraits;
  typename GraphTraits::out_edge_iterator out_i, out_end;
  typedef typename boost::graph_traits<graph>::vertex_descriptor Vertex;
  typedef typename boost::graph_traits<graph>::vertex_iterator vertex_iter;
  typename GraphTraits::edge_descriptor e;

  std::pair<vertex_iter, vertex_iter> vp;
  graph& g = t.g;
  vp = boost::vertices(g);
  std::for_each(vp.first,vp.second,[&](auto p){
    os<< g[p].get_state() << "->";
    if(boost::out_degree(p, g)) {
      for (boost::tie(out_i, out_end) = boost::out_edges(p, g);
         out_i != out_end; ++out_i) {
        e = *out_i;
        Vertex src = boost::source(e, g), targ = boost::target(e, g);
        os<< g[targ].get_state() << ",";
      }
    }
    os<<std::endl;
  });
  os<< std::endl;
  return os;
#endif
}

template<typename game_tree>
std::ostream& operator<<(std::ostream& os, vertex<game_tree>& t)
{
 return t.print(os);
}

}

#endif
