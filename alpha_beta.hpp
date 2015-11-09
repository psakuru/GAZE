#ifndef ALPHA_BETA_HPP
#define ALPHA_BETA_HPP
#include<boost/graph/graph_traits.hpp>
#include<climits>

#define MAX(a, b) (((a)>(b))?(a):(b))
#define MIN(a, b) (((a)>(b))?(b):(a))

//algorithm taken from https://en.wikipedia.org/wiki/Alpha%E2%80%93beta_pruning
template<typename Graph, typename Heuristic>
int alphabeta(Graph &g, typename boost::graph_traits<Graph>::vertex_descriptor v, int depth, int alpha, int beta, bool player1, Heuristic heuristic)
{
  typename boost::graph_traits<Graph>::out_edge_iterator out_i, out_end;

  if(depth == 0 || out_degree(v, g) == 0)
    return heuristic(v);

  if(player1) {
    int v = INT_MIN;
    for(boost::tie(out_i, out_end) = out_edges(v, g); out_i != out_end; ++out_i) {
      v = MAX(v, alphabeta(g, target(*out_i, g), depth-1, alpha, beta, false, heuristic));
      alpha = MAX(alpha, v);
      if(beta <= alpha)
        break;//beta cut-off
    }
    return v;
  } else {
    v = INT_MAX;
    for(boost::tie(out_i, out_end) = out_edges(v, g); out_i != out_end; ++out_i) {
      v = MIN(v, alphabeta(g, source(*out_i, g), depth-1, alpha, beta, true, heuristic));
      beta = MIN(beta, v);
      if(beta<=alpha)
        break;//alpha cut-off
    }
    return v;
  }
}

#endif
