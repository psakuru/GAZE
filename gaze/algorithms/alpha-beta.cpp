#ifndef ALPHA_BETA_HPP
#define ALPHA_BETA_HPP

#include "../gameTree.hpp"
#include <algorithm>
#include <utility>

namespace gaze{
  template<typename game_tree>
  std::pair<int, game_tree::vertex_descriptor> alphabeta(const game_tree& gt_, int depth, int alpha, int beta, bool player1) {
    return alphabeta(gt_, depth, alpha, beta, player1, gt_.get_current_vertex()); 
  }

  template<typename game_tree>
  std::pair<int, game_tree::vertex_descriptor> alphabeta(const game_tree& gt_, int depth_, int alpha, int beta, bool player1, const game_tree::vertex_descriptor cur_) {
    typedef typename gaze::game_tree::vertex_descriptor Vertex;
    typename gaze::game_tree::child_iterator out_i, out_end;
    
    if(depth_ == 0 || cur_.get_children_count() == 0)
      return std::make_pair(cur_.get_value(), cur_);
    
    Vertex vert_;
    if(player1) {
      int v_ = INT_MIN;
      for(std::tie(out_i, out_end) = cur_.get_children(); out_i != out_end; ++out_i) {
        vert_ = *out_i;
        v_ = std::max(v_, alphabeta(gt_, depth-1, alpha, beta, !player1, vert_).first);
        alpha = std::max(alpha, v_);
        if(beta <= alpha)
          break;
      }
      return std::make_pair(v_,vert_);
    } else {
      int v_ = INT_MAX;
      for(std::tie(out_i, out_end) = cur_.get_children(); out_i != out_end; ++out_i) {
        vert_ = *out_i;
        v_ = std::min(v_, alphabeta(gt_, depth-1, alpha, beta, !player1, vert_).first);
        beta = std::min(beta, v_);
        if(beta <= alpha)
          break;
      }
      return std::make_pair(v_, vert_);
    }
  } 
}
#endif
