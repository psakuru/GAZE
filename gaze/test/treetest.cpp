#include<iostream>
#include<vector>
#include "../gametree.hpp"

using namespace std;

template<typename T>
class mycont {
public:
  vector<T> vec;
  ~mycont() { cout<<"called destructor!!"<<endl; }
  auto begin() { return vec.begin(); }
  auto end() { return vec.end(); }
  void push_back(T t) { vec.push_back(t); }
};

int nodecount=0;
class state {
  int nodeno;
public:
  state(int no): nodeno(no) {}
  state() {
    nodeno = nodecount = 0;
  }
  unique_ptr<vector<state*>> get_children() {
    auto lst = make_unique<vector<state*>>();
    lst->push_back(new state(++nodecount));
    lst->push_back(new state(++nodecount));
    return lst;
  }
  bool operator==(const state& other) { return nodeno == other.nodeno; }
  bool operator!=(const state& other) { return !((*this)==other); }
  int get_value() {
    return nodeno;
  }

  friend ostream& operator<<(ostream&, state&);
};

ostream& operator<<(ostream& os, state& st)
{
  return os<<st.nodeno;
}

/**
 * Forms a tree upto \level levels by calling get_children
 */
template<typename vertex>
void make_tree(vertex& vt, int level)
{
  if(!level)
    return;

  auto it_pair = vt.get_children();
  for_each(it_pair.first, it_pair.second, [&](auto &vert) {
    make_tree(vert, level-1);
  });
}

/**
 * Simulates a game play by randomly choosing a child of
 * current vertex and setting it.
 */
template<typename game_tree>
void play(game_tree& gt, int level)
{
  for(;level>0;level--){
    auto &cur_vert = gt.get_current_vertex();
    auto it_pair = cur_vert.get_children();
    int index = rand()%cur_vert.get_children_count();
    for(int i=0;i!=index;i++) it_pair.first++;

    cout<<"for "<<cur_vert.get_state()<<" commit "<<(*it_pair.first).get_state()<<endl;
    cout<<"before removal"<<endl;
    cout<<gt<<endl;
    gt.set_current_state((*it_pair.first).get_state());
    cout<<"after removal"<<endl;
    cout<<gt<<endl;
  }
}

/**
 * Runs a simulated game play for 4 levels
 */
void test_game_play() {
  cout<<"test_game_play"<<endl;
  nodecount=0;
  gaze::game_tree<state> gt(new state(0));
  play(gt, 4);
  cout<<(gt)<<endl;
  cout<<"------------------"<<endl;
}

/**
 * Returns number of child vertices of a vertex
 */
template<typename vertex>
int child_count(vertex& v)
{
  if(!v.are_children_added())
    return 0;

  auto it_pair = v.get_children();
  int count=0;
  for(;it_pair.first!=it_pair.second;it_pair.first++) {
    auto &v = *it_pair.first;
    count+=child_count(v)+1;
  }
  return count;
}

/**
 * Returns number of vertices in the game tree
 */
template<typename game_tree>
int vertex_count(game_tree &gt)
{
  return child_count(gt.get_root_vertex())+1;
}

/**
 * Runs unit tests for game_tree
 */
void game_tree_tests()
{
  //Test get_current*(), get_root_vertex()
  {
    state *root = new state(0);
    gaze::game_tree<state> gt(root);
    assert(gt.get_current_state() == *root);
    assert(gt.get_current_vertex().get_state() == gt.get_current_state());
    assert(gt.get_root_vertex().get_state() == gt.get_current_state());
    assert(vertex_count(gt) == 1);
  }
  //Default constructor should construct vertex with default constructed state
  {
    state *root = new state;
    gaze::game_tree<state> gt;
    assert(gt.get_current_state() == *root);
  }
  //Test set_current_state()
  {
    gaze::game_tree<state> gt;
    make_tree(gt.get_root_vertex(), 2);

    auto st1 = make_unique<state>(1);
    gt.set_current_state(*st1);
    assert(gt.get_current_state() == *st1);
  }
  //Setting wrong state in set_current_state should raise exception
  {
    gaze::game_tree<state> gt;
    make_tree(gt.get_root_vertex(), 2);

    auto st1 = make_unique<state>(1);
    auto st2 = make_unique<state>(2);
    auto st3 = make_unique<state>(3);

    gt.set_current_state(*st1);
    bool exception = false;
    try {
      gt.set_current_state(*st2);
    } catch(invalid_argument &e) {
      exception = true;
    }
    assert(exception);
  }
}

/**
 * Runs unit tests for vertex
 */
void vertex_tests()
{
  {
    gaze::game_tree<state> gt;
    make_tree(gt.get_root_vertex(), 2);
    //Test if tree is formed properly
    assert(vertex_count(gt) == 7);

    auto st1 = make_unique<state>(1);
    auto st4 = make_unique<state>(4);

    gt.set_current_state(*st1);
    //Test if vertices are removed properly
    assert(vertex_count(gt) == 4);

    gt.set_current_state(*st4);
    //Test if vertices are removed properly
    assert(vertex_count(gt) == 3);


    make_tree(gt.get_current_vertex(), 2);
    //Test if tree is formed properly
    assert(vertex_count(gt) == 9);
  }
  //Test if parents are correct
  {
    gaze::game_tree<state> gt;
    make_tree(gt.get_root_vertex(), 2);

    auto &root = gt.get_current_vertex();

    auto st1 = make_unique<state>(1);
    gt.set_current_state(*st1);
    auto &vt1 = gt.get_current_vertex();
    assert(vt1.get_parent() == root);

    auto st4 = make_unique<state>(4);
    gt.set_current_state(*st4);
    auto &vt4 = gt.get_current_vertex();
    assert(vt4.get_parent() == vt1);

    //Test are_children_added()
    assert(!vt4.are_children_added());
  }
  //Test are_children_added()
  {
    gaze::game_tree<state> gt;
    auto &vert = gt.get_current_vertex();
    auto it_pair = vert.get_children();
    for(;it_pair.first!=it_pair.second;it_pair.first++) {
      auto &child = *it_pair.first;
      assert(!child.are_children_added());
    }
  }
  //Test get_state(), get_value()
  {
    gaze::game_tree<state> gt;

    auto st1 = make_unique<state>(1);
    gt.set_current_state(*st1);

    auto &vt1 = gt.get_current_vertex();
    assert(vt1.get_state() == *st1);
    assert(vt1.get_value() == st1->get_value());
  }
  //Test get_children_count()
  {
    gaze::game_tree<state> gt;
    make_tree(gt.get_current_vertex(), 2);
    assert(gt.get_current_vertex().get_children_count() == 2);

    auto st1 = make_unique<state>(1);
    gt.set_current_state(*st1);
    assert(gt.get_current_vertex().are_children_added());
    assert(gt.get_current_vertex().get_children_count() == 2);

    auto st4 = make_unique<state>(4);
    gt.set_current_state(*st4);
    assert(!gt.get_current_vertex().are_children_added());
    assert(gt.get_current_vertex().get_children_count() == 2);
  }
  //Test ==, !=
  {
    gaze::game_tree<state> gt;
    make_tree(gt.get_current_vertex(), 2);

    auto &root = gt.get_root_vertex();
    auto &cur = gt.get_current_vertex();

    assert(root == cur);

    auto st1 = make_unique<state>(1);
    gt.set_current_state(*st1);

    auto &cur1 = gt.get_current_vertex();
    assert(cur1 != root);
  }
}

/**
 * Run unit tests on vertex_iterator
 */
void vertex_iterator_tests()
{
  gaze::game_tree<state> gt;
  auto it_pair = gt.get_current_vertex().get_children();
  auto it_pair2 = it_pair;
  auto it_pair3 = it_pair2;
  //Test operator++(int), operator++()
  for(int i=0;it_pair.first!=it_pair.second;
                          it_pair.first++, ++it_pair3.first, i++) {
    //Test operator*
    assert(it_pair.first == it_pair3.first);

    auto &child = *it_pair.first;
    if(i==0) {
      assert(child.get_state() == state(1));
      //Test operator->
      assert(it_pair.first->get_state() == state(1));
    }
    else if(i==1) {
      assert(child.get_state() == state(2));
      //Test operator->
      assert(it_pair.first->get_state() == state(2));
    }
    else
      assert(1==0);//There should only be two children
  }

  //Test copy constructor, =, ==, !=
  auto it_pair4 = it_pair;
  assert(it_pair4.first == it_pair4.second);

  auto it_pair5(it_pair);
  assert(it_pair5.first == it_pair.first);

  assert(it_pair2.first != it_pair.first);
}

/**
 * Runs unit tests for game_tree, vertex, vertex_iterator
 */
void unit_tests()
{
  game_tree_tests();
  vertex_tests();
  vertex_iterator_tests();
}

gaze::game_tree<state> *gt;
void init() {
  unit_tests();
  test_game_play();
}
int main()
{
  init();
}
