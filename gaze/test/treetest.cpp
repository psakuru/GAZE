#include<iostream>
#include<vector>
#include "../gametree.hpp"

using namespace std;

int nodecount=1;
class state {
  int nodeno;
public:
  state(int no): nodeno(no) {}
  vector<state>& get_children() {
    //cout<<"get_children "<<nodeno<<endl;
    vector<state>& lst = *new vector<state>();
    lst.push_back(*new state(nodecount++));
    lst.push_back(*new state(nodecount++));
    return lst;
  }
  bool operator==(const state& other) { return nodeno == other.nodeno; }
  int get_value();

  friend ostream& operator<<(ostream&, state&);
};

ostream& operator<<(ostream& os, state& st)
{
  return os<<st.nodeno;
}

template<typename vertex>
void make_tree(vertex& vt, int level)
{
  //cout<<string(2*(3-level), ' ');
  //cout<<"make_tree "<<vt.get_state()<<" "<<level<<endl;
  if(!level)
    return;

  auto it_pair = vt.get_children();
  for_each(it_pair.first, it_pair.second, [&](auto vert) { make_tree(vert, level-1); });
  //cout<<string(2*(3-level), ' ');
  //cout<<"----"<<endl;
}

template<typename vertex>
void print(vertex& vt, int level)
{
  if(!level)
    return;
  cout<<"("<<vt.get_state()<<": ";
  auto it_pair = vt.get_children();
  for_each(it_pair.first, it_pair.second, [&](auto vert) { print(vert, level-1); });
  cout<<")";
}

template<typename graph>
void print_vd(graph& g)
{
  auto vert_pair = boost::vertices(g);
  cout<<"print_vd"<<endl;
  for_each(vert_pair.first, vert_pair.second, [&](auto vd) {
    cout<<g[vd].get_state()<<"::"<<vd<<endl;
  });
  cout<<"==========="<<endl;
}

gaze::game_tree<state> *gt;
void init() {
  gt = new gaze::game_tree<state>(new state(0));
  auto vertex = gt->get_root_vertex();
  make_tree(vertex, 2);
  cout<<(*gt)<<endl;
  print_vd(gt->g);
  cout<<"=-------------------="<<endl;
  make_tree(vertex, 3);
  //print(vertex, 3);
  cout<<endl;

  cout<<(*gt)<<endl;
  print_vd(gt->g);
}
int main()
{
  init();
}
