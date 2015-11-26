#include<iostream>
#include<vector>
#include "../gametree.hpp"

using namespace std;

class state {
  int nodeno;
public:
  state(int no): nodeno(no) {}
  vector<state>& get_children() {
    vector<state>& lst = *new vector<state>();
    lst.push_back(nodeno*3);
    lst.push_back(nodeno*3+1);
    return lst;
  }
  int get_value();
  void print() {
    cout<<"node: "<<nodeno<<endl;
  }
};

gaze::game_tree<state> *gt;
void init() {
  //gt = new gaze::game_tree<state>(*new state(0));
  gt = new gaze::game_tree<state>(new state(0));
  auto vertex = gt->get_current_vertex();
  auto iter_pair = vertex.get_children();
  gaze::game_tree<state>::vertex_iterator it=iter_pair.first;
  gaze::game_tree<state>::vertex_iterator eit=iter_pair.second;

  it++;//working
  (*it);//not working

  //(*it).get_state().print();
  //for(gaze::game_tree<state>::vertex_iterator it=iter_pair.first;it!=iter_pair.second;it++)
   // (*it).get_state().print();
  //for_each(iter_pair.first, iter_pair.second, [](auto st) { st.print(); });
}
int main()
{
  init();
}
