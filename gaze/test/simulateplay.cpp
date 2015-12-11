#include<iostream>
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
    cout<<"  before removal"<<endl;
    cout<<"  "<<gt<<endl;
    gt.set_current_state((*it_pair.first).get_state());
    cout<<"  after removal"<<endl;
    cout<<"  "<<gt<<endl;
  }
}

/**
 * Runs a simulated game play for user specified number of levels
 */
int main()
{
  int levels;
  cout<<"Input number of moves to test for:";
  cin>>levels;

  nodecount=0;
  gaze::game_tree<state> gt(new state(0));
  play(gt, levels);
  cout<<(gt)<<endl;
}
