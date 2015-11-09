#include<iostream>
#include<boost/graph/graph_traits.hpp>
#include<boost/graph/adjacency_list.hpp>
#include "alpha_beta.hpp"
#include<climits>

using namespace std;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS> Graph;
typedef typename boost::graph_traits<Graph>::vertex_descriptor Vertex;

void make_graph(Graph& g)
{
    boost::add_edge(0,1,g);
    boost::add_edge(0,2,g);
    boost::add_edge(0,3,g);
    boost::add_edge(1,4,g);
    boost::add_edge(1,5,g);
    boost::add_edge(2,6,g);
    boost::add_edge(2,7,g);
    boost::add_edge(3,8,g);
    boost::add_edge(3,9,g);
    boost::add_edge(4,10,g);
    boost::add_edge(4,11,g);
    boost::add_edge(5,12,g);
    boost::add_edge(5,13,g);
    boost::add_edge(6,14,g);
    boost::add_edge(6,15,g);
    boost::add_edge(7,16,g);
    boost::add_edge(7,17,g);
    boost::add_edge(8,18,g);
    boost::add_edge(8,19,g);
    boost::add_edge(9,20,g);
    boost::add_edge(9,21,g);
}

int main() {
  Graph g(10);
  make_graph(g);
  Vertex first = *boost::vertices(g).first;
  std::cout<<alphabeta(g, 0, 10, INT_MIN, INT_MAX, true, [](auto v) {
            switch(v) {
                case 10: return 4;
                         break;
                case 11: return 6;
                         break;
                case 12: return 7;
                         break;
                case 13: return 9;
                         break;
                case 14: return 1;
                         break;
                case 15: return 2;
                         break;
                case 16: return 0;
                         break;
                case 17: return 1;
                         break;
                case 18: return 8;
                         break;
                case 19: return 1;
                         break;
                case 20: return 9;
                         break;
                case 21: return 2;
                         break;
                default:
                         return 0;
                         break;

            }
          
        })<<std::endl;

  return 0;
}
