#ifndef ALPHA_BETA_HPP
#define ALPHA_BETA_HPP
#include<boost/graph/graph_traits.hpp>
#include<algorithm>
#include<climits>
#include "logging.hpp"

//algorithm taken from https://en.wikipedia.org/wiki/Alpha%E2%80%93beta_pruning
template<typename Graph, typename Heuristic>
std::pair<int, const typename boost::graph_traits<Graph>::vertex_descriptor>
alphabeta(const Graph &g, const typename boost::graph_traits<Graph>::vertex_descriptor vd, int depth, int alpha, int beta, bool player1, Heuristic heuristic)
{
  typedef typename boost::graph_traits<Graph>::vertex_descriptor Vertex;
  typename boost::graph_traits<Graph>::out_edge_iterator out_i, out_end;
  dout<<vd;

  if(depth == 0 || out_degree(vd, g) == 0) {
    dout<<std::endl;
    return std::make_pair(heuristic(vd), vd);
  }
  Vertex vert;

  if(player1) {
    int v = INT_MIN;
    for(boost::tie(out_i, out_end) = boost::out_edges(vd, g); out_i != out_end; ++out_i) {
        dout<<"   children "<<out_end - out_i;
        dout<<"   true";
        dout<<" calling "<<target(*out_i, g);
        dout<<std::endl;
      vert = target(*out_i, g);
      v = std::max(v, alphabeta(g, vert, depth-1, alpha, beta, !player1, heuristic).first);
      alpha = std::max(alpha, v);
      if(beta <= alpha)
        break;//beta cut-off
    }
    return std::make_pair(v, vert);
  } else {
    int v = INT_MAX;
    for(boost::tie(out_i, out_end) = boost::out_edges(vd, g); out_i != out_end; ++out_i) {
        dout<<"   children "<<out_end - out_i;
        dout<<"   false";
        dout<<" calling "<<target(*out_i, g);
        dout<<std::endl;
      vert = target(*out_i, g);
      v = std::min(v, alphabeta(g, vert, depth-1, alpha, beta, !player1, heuristic).first);
      beta = std::min(beta, v);
      if(beta<=alpha)
        break;//alpha cut-off
    }
    return std::make_pair(v, vert);
  }
}

#endif
