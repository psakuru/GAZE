#include<iostream>
#include<vector>
#include "../gametree.hpp"

using namespace std;

int nodecount=0;
class state {
  int nodeno;
public:
  state(int no): nodeno(no) {}
  vector<state*>& get_children() {
    //cout<<"get_children "<<nodeno<<endl;
    vector<state*>& lst = *new vector<state*>();
    lst.push_back(new state(++nodecount));
    lst.push_back(new state(++nodecount));
    //lst.push_back(new state(++nodecount));
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
  for_each(it_pair.first, it_pair.second, [&](auto &vert) {
      make_tree(vert, level-1);
  });
  //cout<<string(2*(3-level), ' ');
  //cout<<"----"<<endl;
}

template<typename game_tree>
void play(game_tree& gt, int level)
{
  for(;level>0;level--){
    auto &cur_vert = gt.get_current_vertex();
    auto it_pair = cur_vert.get_children();
    int index = rand()%cur_vert.get_children_count();
    for(int i=0;i!=index;i++) it_pair.first++;

    cout<<"for "<<cur_vert.get_state()<<" commit "<<(*it_pair.first).get_state()<<endl;
    cout<<"before removal"<<std::endl;
    cout<<gt<<endl;
    gt.set_current_state((*it_pair.first).get_state());
    cout<<"after removal"<<std::endl;
    cout<<gt<<endl;
  }
}
template<typename vertex>
void print(vertex& vt, int level)
{
  if(!level)
    return;
  cout<<"("<<vt.get_state()<<": ";
  auto it_pair = vt.get_children();
  for_each(it_pair.first, it_pair.second, [&](auto &vert) {
      print(vert, level-1);
  });
  cout<<")";
}

template<typename graph>
void print_vd(graph& g)
{
  auto vert_pair = boost::vertices(g);
  for_each(vert_pair.first, vert_pair.second, [&](auto vd) {
    cout<<g[vd].get_state()<<"::"<<vd<<endl;
  });
  cout<<"==========="<<endl;
}

void test_make_tree() {
  cout<<"test_make_tree"<<std::endl;
  nodecount=0;
  gaze::game_tree<state> gt(new state(0));
  auto &vertex = gt.get_root_vertex();
  make_tree(vertex, 3);
  cout<<(gt)<<endl;
  cout<<"=-------="<<endl;
  make_tree(vertex, 3);
  make_tree(vertex, 3);
  cout<<endl;
  cout<<(gt)<<endl;
  cout<<"------------------"<<std::endl;
}

void test_game_play() {
  cout<<"test_game_play"<<std::endl;
  nodecount=0;
  gaze::game_tree<state> gt(new state(0));
  play(gt, 4);
  cout<<(gt)<<endl;
  cout<<"------------------"<<std::endl;
}
gaze::game_tree<state> *gt;
void init() {
  test_make_tree();
  test_game_play();
}
int main()
{
  init();
}
