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
  graph* g;
public:
  viterator(graph_edge_it it, graph* g): it(it), g(g){}
  viterator(const viterator &other): it(other.it), g(other.g) {}
  viterator& operator=(const viterator& rhs) {
    it = rhs.it;
    g = rhs.g;
    return *this;
  }
  viterator& operator++() {
    ++it;
    return *this;
  };
  viterator operator++(int) {
    viterator res(*this);
    ++(*this);
    return res;
  }
  bool operator==(const viterator& rhs) {
    return it==rhs.it;
  }
  bool operator!=(const viterator& rhs) {
    return it!=rhs.it;
  }
  vertex& operator*() {
    return (*g)[boost::target(*it, *g)];
  }
  vertex* operator->() {
    return &(*g)[boost::target(*it, *g)];
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
  typedef typename game_tree::state_value_type value_type;
  typedef typename game_tree::degree_size_type degree_size_type;

  typedef typename boost::graph_traits<graph>::vertex_descriptor vertex_descriptor;

  vertex() {}
private:
  vertex(game_state* st, vertex_descriptor vd, vertex_descriptor pvd, int level,
              graph* g): st(st), vd(vd), parent_vd(pvd), g(g), level(level) {}

  /**
   * Copy constructor copies only the state
   */
  vertex(const vertex& other) {
    dout<<"copy constructor "<<*other.st<<std::endl;
    st = new game_state(*other.st);
  }

  /**
   * Move constructor moves every pointer and sets source pointers to null
   */
  vertex(vertex&& other) {
    dout<<"move constructor "<<*other.st<<std::endl;
    level = other.level;
    vd = other.vd;
    parent_vd = other.parent_vd;
    st = other.st;
    g = other.g;
    children_added = other.children_added;

    other.level = -1;
    other.vd = 0;
    other.parent_vd = 0;
    other.st = nullptr;
    other.g = nullptr;
    other.children_added = false;
  }

  /**
   * Copy assignment copies only the state
   */
  vertex& operator=(const vertex& other) {
    dout<<"copy assignment "<<std::endl;
    st = new game_state(*other.st);
    return *this;
  }

  /**
   * Move assignment moves every pointer and sets source pointers to null
   */
  vertex& operator=(vertex&& other) {
    dout<<"move assignment "<<*other.st<<std::endl;
    level = other.level;
    vd = other.vd;
    parent_vd = other.parent_vd;
    st = other.st;
    g = other.g;
    children_added = other.children_added;

    other.level = -1;
    other.vd = 0;
    other.parent_vd = 0;
    other.st = nullptr;
    other.g = nullptr;
    other.children_added = false;

    return *this;
  }

public:

  /**
   * Returns reference to game_state object
   */
  game_state& get_state() {
    return *st;
  }

  /**
   * Returns reference to parent vertex
   */
  vertex& get_parent() {
    if(parent_vd != boost::graph_traits<graph>::null_vertex()) {
      return (*g)[parent_vd];
    }
    return *this;
  }

  /**
   * Removes all outgoing edges and child vertices recursively
   */
  ~vertex() {
    dout<<"destructor";
    if(st)
      dout<<" for "<<get_state();
    dout<<std::endl;
    //TODO: remove child vertices
    if (children_added) {
      auto itpair = get_children();
      std::vector<vertex_descriptor> toremove;
      for_each(itpair.first,itpair.second,[&](auto &vert) {
        toremove.push_back(vert.get_vd());
      });
      //remove all out edges
      boost::remove_out_edge_if(get_vd(), [](auto vd) { return true; }, *g);
      for_each(toremove.begin(), toremove.end(), [&](auto vd) {
        boost::remove_vertex(vd, *g);
      });
    }
    level = -1;
    vd = 0;
    parent_vd = 0;
    delete st;
    st = nullptr;
    g = nullptr;
    children_added = false;
  }

  /**
   * Returns an iterator pair to child vertices.
   * The iterators are not stable if one of the vertices or edges
   * are removed
   */
  std::pair<vertex_iterator, vertex_iterator> get_children() {
    assert((*g)[vd]==(*this));
    if(!children_added){
      add_children();
    }
    typename boost::graph_traits<graph>::out_edge_iterator begin_edge_it,
                                                            end_edge_it;

    boost::tie(begin_edge_it, end_edge_it) = boost::out_edges(vd, *g);

    return std::make_pair(vertex_iterator(begin_edge_it, g),
                          vertex_iterator(end_edge_it, g));
  }

  /**
   * Equality operator. Vertex formed by copy constructor/assignment operator
   * will not compare equal to original vertex.
   */
  bool operator==(const vertex& vt) {
    return level==vt.level && vd==vt.vd && parent_vd==vt.parent_vd &&
            st==vt.st && g==vt.g && children_added == vt.children_added;
  }

  bool operator!=(const vertex& vt) {
    return !(*this==vt);
  }

  /**
   * Returns the number of child vertices.
   * If child vertices are not yet added, they are added
   */
  degree_size_type get_children_count() {
    if(!children_added) {
      add_children();
    }
    return boost::out_degree(vd, *g);
  }

  /**
   * Prints current vertex and all its children
   */
  std::ostream& print(std::ostream& os) {
    os<<"("<<get_state()<<" ";
    if(children_added) {
      auto it_pair = get_children();
      for_each(it_pair.first, it_pair.second, [&](auto &vert){ os<<vert; });
    }
    os<<")";
    return os;
  }

  /**
   * Returns value of the associated game_state
   */
  value_type get_value() {
    return (*st).get_value(); 
  }

  /**
   * Returns if children for the vertex are added
   */
  bool are_children_added() {
    return children_added;
  }

private:

  /**
   * Low level method to add children, takes begin, end iterators
   */
  template<class It>
  void _add_children(It begin, It end) {
    for_each(begin, end, [&](game_state *st) {
      auto tvd = boost::add_vertex(*g);
      (*g)[tvd] = std::move(vertex_property(st, tvd, vd, level+1, g));
      boost::add_edge(vd, tvd, *g);
    });
  }

  /**
   * When state.get_children() returns container object or reference,
   * this function is called
   */
  template<class C>
  void _add_children(C &container) {
    _add_children(container.begin(), container.end());
  }

  /**
   * When state.get_children() returns a unique_ptr of container,
   * this function is called
   */
  template<typename C>
  void _add_children(std::unique_ptr<C> container) {
    _add_children(container->begin(), container->end());
  }

  /**
   * When state.get_children() returns a pointer to container,
   * this function is called
   */
  template<typename C>
  void _add_children(C *container) {
    _add_children(container->begin(), container->end());
    delete container;
  }

  /**
   * This function calls the specialized _add_children functions
   * depending on whether state.get_children() returns a unique_ptr
   * or pointer or just a container
   */
  void add_children() {
    assert(!children_added);
    _add_children(st->get_children());
    children_added = true;
  }

  /**
   * Returns vertex descriptor of current vertex
   * Not to be used by algorithm and game developers
   */
  vertex_descriptor get_vd() {
    return vd;
  }
  friend game_tree;

  /* Depth of current vertex */
  int level=0;
  /* Boost graph vertex descriptor of current vertex */
  vertex_descriptor vd=0;
  /* Boost graph vertex descriptor of parent vertex */
  vertex_descriptor parent_vd=0;
  /* User provided game_state */
  game_state *st;
  /* Boost graph */
  graph *g;
  /* Denotes if child vertices are found and added to current vertex */
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
  typedef typename boost::graph_traits<graph>::degree_size_type degree_size_type;

  typedef typename vertex_property::vertex_iterator vertex_iterator;


  /**
   * Construct game tree with the provided game_state as initial state
   */
  game_tree(game_state* st) {
    root_vertex = cur_vertex = boost::add_vertex(g);
    g[root_vertex] = std::move(vertex_property(st, root_vertex, invalid_vd, 0, &g));
  }

  /**
   * No argument constructor. To be called if a no argument
   * constructor for game_state creates the first position game_state
   */
  game_tree() {
    game_state* st = new game_state();
    root_vertex = cur_vertex = boost::add_vertex(g);
    g[root_vertex] = std::move(vertex_property(st, root_vertex, invalid_vd, 0, &g));
  }
  
  game_tree(const game_tree& otherTree);

  /**
   * Sets current vertex.
   * All the other sibling vertices are removed from graph
   */
  void set_current_vertex(vertex_property& committedVertex) {
    set_current_state(committedVertex.get_state()); 
  }

  /**
   * Sets current state.
   * All the other sibling states are removed from graph
   */
  void set_current_state(game_state& committedState) {
    //dout<<"set_cur_state "<<committedState<<std::endl;
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

      if(child_vert.get_state()==committedState){
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
    vertex_descriptor vd(boost::graph_traits<graph>::null_vertex());
    std::vector<vertex_descriptor> toremove;
    for(auto it=itpair.first;it!=itpair.second;it++){
      if(it->get_state() == committedState){
        vd = it->get_vd();
      }else
        toremove.push_back(it->get_vd());
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
    return get_current_vertex().get_state();
  }

  /**
   * Not part of Game Tree concept. But useful for testing/analysis
   */
  graph& get_graph() {
    return g;
  }

  std::ostream& print(std::ostream& os) {return os<<get_root_vertex();}
private:
  graph g;
  vertex_descriptor cur_vertex;
  vertex_descriptor root_vertex;
  vertex_descriptor invalid_vd = boost::graph_traits<graph>::null_vertex();

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
