#include<iostream>
#include<boost/graph/graph_traits.hpp>
#include<boost/graph/adjacency_list.hpp>
#include "alpha_beta.hpp"
#include<climits>

using namespace std;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS> Graph;
typedef typename boost::graph_traits<Graph>::vertex_descriptor Vertex;

void make_graph(Graph g)
{
}

int main() {
  cout<<"hi"<<endl;
  Graph g(10);
  make_graph(g);
  Vertex first = *boost::vertices(g).first;
  std::cout<<alphabeta(g, first, 4, INT_MIN, INT_MAX, true, [](auto v) {return 1;})<<std::endl;

  return 0;
}
