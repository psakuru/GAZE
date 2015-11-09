#ifndef ALPHA_BETA_HPP
#define ALPHA_BETA_HPP
#include<boost/graph/graph_traits.hpp>
#include<climits>

#define MAX(a, b) (((a)>(b))?(a):(b))
#define MIN(a, b) (((a)>(b))?(b):(a))

//algorithm taken from https://en.wikipedia.org/wiki/Alpha%E2%80%93beta_pruning
template<typename Graph, typename Heuristic>
int alphabeta(const Graph &g, const typename boost::graph_traits<Graph>::vertex_descriptor vd, int depth, int alpha, int beta, bool player1, Heuristic heuristic)
{
  typename boost::graph_traits<Graph>::out_edge_iterator out_i, out_end;

  if(depth == 0 || out_degree(vd, g) == 0) {
    return heuristic(vd);
  }

  if(player1) {
    int v = INT_MIN;
    for(boost::tie(out_i, out_end) = boost::out_edges(vd, g); out_i != out_end; ++out_i) {
        std::cout<<"number of children "<<out_end - out_i <<std::endl;
        std::cout<<"true"<<std::endl;
        std::cout<<target(*out_i,g)<<std::endl;
      v = MAX(v, alphabeta(g, target(*out_i, g), depth-1, alpha, beta, !player1, heuristic));
      alpha = MAX(alpha, v);
      if(beta <= alpha)
        break;//beta cut-off
    }
    return v;
  } else {
    int v = INT_MAX;
    for(boost::tie(out_i, out_end) = boost::out_edges(vd, g); out_i != out_end; ++out_i) {
        std::cout<<"number of children "<<out_end - out_i <<std::endl;
        std::cout<<"false"<<std::endl;
        std::cout<<target(*out_i,g)<<std::endl;
      v = MIN(v, alphabeta(g, target(*out_i, g), depth-1, alpha, beta, !player1, heuristic));
      beta = MIN(beta, v);
      if(beta<=alpha)
        break;//alpha cut-off
    }
    return v;
  }
}

#endif
