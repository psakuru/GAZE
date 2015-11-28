#ifndef GAMETREE_HPP
#define GAMETREE_HPP

#include<boost/graph/adjacency_list.hpp>
#include<boost/graph/graph_traits.hpp>
#include<vector>

#define DEBUG 0
#include "../logging.hpp"

namespace gaze {

template<typename game_state>
class game_tree;

template<typename game_tree>
class viterator : public std::iterator<std::forward_iterator_tag,
                              typename game_tree::vertex_property> {
  typedef typename game_tree::vertex_property vertex;
  typedef typename vertex::graph_edge_it graph_edge_it;
  typedef typename game_tree::graph graph;
  graph_edge_it it;
  graph& g;
public:
  viterator(graph_edge_it it, graph& g): it(it), g(g){}
  viterator(const viterator &other): it(other.it), g(other.g) {}
  viterator& operator=(const viterator& rhs) { it = rhs.it; g = rhs.g; return *this; }
  viterator& operator++() {++it; return *this;};
  viterator operator++(int) {viterator res(*this); ++(*this); return res;}
  bool operator==(const viterator& rhs) { return it==rhs.it; }
  bool operator!=(const viterator& rhs) { return it!=rhs.it; }
  vertex& operator*() {
    return g[boost::target(*it, g)];
  }
};

template<typename game_tree>
class vertex {
public:
  typedef typename game_tree::graph graph;
  typedef typename game_tree::game_state game_state;
  typedef vertex<game_tree> vertex_property;
  typedef typename boost::graph_traits<graph>::out_edge_iterator graph_edge_it;
  typedef typename gaze::viterator<game_tree> vertex_iterator;
  typedef size_t degree_size_type;
  typedef typename game_tree::state_value_type value_type;

  typedef typename boost::graph_traits<graph>::vertex_descriptor vertex_descriptor;

  vertex() {}
  vertex(game_state* st, vertex_descriptor vd, vertex_descriptor pvd, int level,
              graph* g): st(st), vd(vd), parent_vd(pvd), g(g), level(level) {}

  game_state& get_game_state() { return *st; }
  vertex_descriptor get_vd() { return vd; }
  vertex& get_parent() { return (*g)[parent_vd]; }
  ~vertex() {
    dout<<"destructor for "<<get_state()<<std::endl;
    //TODO: remove child vertices
    level = -1;
    vd = 0;
    parent_vd = 0;
    st = nullptr;
    g = nullptr;
    children_added = false;
  }

  std::pair<vertex_iterator, vertex_iterator> get_children() {
    assert((*g)[vd]==(*this));
    if(!children_added){
      add_children();
    }
    typename boost::graph_traits<graph>::out_edge_iterator begin_edge_it,
                                                            end_edge_it;

    boost::tie(begin_edge_it, end_edge_it) = boost::out_edges(vd, *g);

    return std::make_pair(vertex_iterator(begin_edge_it, *g),
                          vertex_iterator(end_edge_it, *g));
  }

  bool operator==(const vertex& vt) {
    return vd==vt.vd && *st==*vt.st && children_added == vt.children_added;
  }

  bool operator!=(const vertex& vt) {
    return !(*this==vt);
  }

  game_state& get_state() { return *st; }

  size_t get_children_count() {
    if(!children_added) {
      add_children();
    }
    return boost::out_degree(vd, *g);
  }

  std::ostream& print(std::ostream& os) {
    os<<"("<<get_game_state()<<" ";
    if(children_added) {
      auto it_pair = get_children();
      for_each(it_pair.first, it_pair.second, [&](auto &vert){ os<<vert; });
    }
    os<<")";
    return os;
  }

  value_type get_value() {
    return (*st).get_value(); 
  }

private:
  void add_children() {
    assert(!children_added);
    auto &container = st->get_children();
    for(auto it=container.begin(); it!=container.end(); it++){
      auto tvd = boost::add_vertex(*g);
      (*g)[tvd] = vertex_property(*it, tvd, vd, level+1, g);
      boost::add_edge(vd, tvd, *g);
    }
    children_added = true;
  }

  int level=0;
  vertex_descriptor vd, parent_vd;
  game_state* st = nullptr;
  graph* g = nullptr;
  bool children_added = false;
};

template<typename state>
class game_tree {
public:
  typedef state game_state;
  typedef decltype(((game_state*)nullptr)->get_value()) state_value_type;
  typedef vertex<game_tree> vertex_property;
  typedef boost::adjacency_list<boost::listS, boost::listS, boost::directedS,
                        vertex_property> graph;
  typedef typename boost::graph_traits<graph>::vertex_descriptor vertex_descriptor;

  typedef typename vertex_property::vertex_iterator vertex_iterator;


  game_tree(game_state* st) {
    root_vertex = cur_vertex = boost::add_vertex(g);
    g[root_vertex] = vertex_property(st, root_vertex, 0, 0, &g);
  }

  game_tree() {
    game_state* st = new game_state();
    root_vertex = cur_vertex = boost::add_vertex(g);
    g[root_vertex] = vertex_property(st, root_vertex, 0, 0, &g);
  }
  
  game_tree(const game_tree& otherTree);

  //to be called for all moves
  void set_current_state(game_state& committedstate) {
    //dout<<"set_cur_state "<<committedstate<<std::endl;
    auto &vert = get_current_vertex();
    auto itpair = vert.get_children();
    //edge iterator is unstable after edge removal, so can't use below code
    /*vertex_iterator it = itpair.first;
    vertex_iterator next = it;
    for(next = it; it != itpair.second; it = next){
      ++next;
      auto &child_vert = *it;
      dout<<"cur="<<child_vert.get_state()<<" next=";
      if(next==itpair.second)
        dout<<"end"<<std::endl;
      else
        dout<<(*next).get_state()<<std::endl;

      if(child_vert.get_state()==committedstate){
        dout<<"keeping "<<child_vert.get_state()<<std::endl;
        cur_vertex = child_vert.get_vd();
      } else {
        auto vd = child_vert.get_vd();
        dout<<"removing "<<child_vert.get_state()<<std::endl;
        boost::remove_edge(vert.get_vd(), vd, g);
        boost::remove_vertex(vd, g);
        dout<<"removed"<<std::endl;
      }
    }*/
    vertex_descriptor vd(0);
    std::vector<vertex_descriptor> toremove;
    for(auto it=itpair.first;it!=itpair.second;it++){
      if((*it).get_state() == committedstate){
        vd = (*it).get_vd();
      }else
        toremove.push_back((*it).get_vd());
    }
    if(vd==0)
      throw std::invalid_argument("No such state");

    cur_vertex = vd;

    //remove all out edges
    boost::remove_out_edge_if(vert.get_vd(), [](auto vd) { return true; }, g);
    boost::add_edge(vert.get_vd(), vd, g);
    for_each(toremove.begin(), toremove.end(), [&](auto vd) {
        boost::remove_vertex(vd, g);
    });
  }
  //returns the previously committed vetex, used by algo
  vertex_property& get_current_vertex() {return g[cur_vertex];}
  //returns root vertex of the tree
  vertex_property& get_root_vertex() {return g[root_vertex];}
  //returns the previously committed game_state, used by game designer
  game_state& get_current_state() {
    return cur_vertex->get_game_state();
  }
  graph g;

  std::ostream& print(std::ostream& os) {return os<<get_root_vertex();}
private:
  vertex_descriptor cur_vertex;
  vertex_descriptor root_vertex;
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
    os<< g[p].get_game_state() << "->";
    if(boost::out_degree(p, g)) {
      for (boost::tie(out_i, out_end) = boost::out_edges(p, g);
         out_i != out_end; ++out_i) {
        e = *out_i;
        Vertex src = boost::source(e, g), targ = boost::target(e, g);
        os<< g[targ].get_game_state() << ",";
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
