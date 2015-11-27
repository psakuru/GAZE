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
    lst.push_back(nodecount++);
    lst.push_back(nodecount++);
    return lst;
  }
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
  cout<<"make_tree "<<vt.get_state()<<" "<<level<<endl;
  if(!level)
    return;

  auto it_pair = vt.get_children();
  for_each(it_pair.first, it_pair.second, [&](auto vert) { make_tree(vert, level-1); });
}

gaze::game_tree<state> *gt;
void init() {
  //gt = new gaze::game_tree<state>(*new state(0));
  gt = new gaze::game_tree<state>(new state(0));
  auto vertex = gt->get_root_vertex();
  make_tree(vertex, 3);
  //auto iter_pair = vertex.get_children();
  //gaze::game_tree<state>::vertex_iterator it=iter_pair.first;
  //gaze::game_tree<state>::vertex_iterator eit=iter_pair.second;

  //(*it).get_state().print();
  cout<<(*gt)<<endl;
  //for_each(iter_pair.first, iter_pair.second, [](auto st) { st.print(cout); });
}
int main()
{
  init();
}
