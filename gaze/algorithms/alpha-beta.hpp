#ifndef ALPHA_BETA_HPP
#define ALPHA_BETA_HPP

#include "../gametree.hpp"
#include "../game_traits.hpp"
#include <algorithm>
#include <utility>

namespace gaze{
  template<typename game_tree>
  std::pair<int, typename game_traits<game_tree>::vertex_property> alphabeta(game_tree& gt_, 
                                                                          int depth, 
                                                                          typename game_traits<game_tree>::state_value_type alpha, 
                                                                          typename game_traits<game_tree>::state_value_type beta, 
                                                                          bool player1) {
    return alphabeta(gt_, depth, alpha, beta, player1, gt_.get_current_vertex()); 
  }

  template<typename game_tree>
  std::pair<int, typename game_traits<game_tree>::vertex_property> alphabeta(game_tree& gt_, 
                                                                          int depth_, 
                                                                          typename game_traits<game_tree>::state_value_type alpha, 
                                                                          typename game_traits<game_tree>::state_value_type beta, 
                                                                          bool player1, 
                                                                          typename game_traits<game_tree>::vertex_property& cur_) {
    typedef typename game_traits<game_tree>::vertex_property Vertex;
    //typename game_traits<game_tree>::vertex_iterator out_i, out_end;
    typedef typename game_traits<game_tree>::state_value_type game_state_value;
    
    if(depth_ == 0 || cur_.get_children_count() == 0)
      return std::make_pair(cur_.get_value(), cur_);
    
    Vertex* vert_;
    if(player1) {
      game_state_value v_ = game_traits<game_tree>::game_state::min_state_value();
      auto it_pair = cur_.get_children(); 
      for(; it_pair.first != it_pair.second; ++(it_pair.first)) {
        vert_ = &(*it_pair.first);
        v_ = std::max(v_, alphabeta(gt_, depth_-1, alpha, beta, !player1, *vert_).first);
        alpha = std::max(alpha, v_);
        //std::cout<<"value p1 "<<v_<<std::endl;
        if(beta <= alpha)
          break;
      }
      return std::make_pair(v_, *vert_);
    } else {
      game_state_value v_ = game_traits<game_tree>::game_state::max_state_value();
      auto it_pair = cur_.get_children(); 
      for(; it_pair.first != it_pair.second; ++(it_pair.first)) {
        vert_ = &(*it_pair.first);
        v_ = std::min(v_, alphabeta(gt_, depth_-1, alpha, beta, !player1, *vert_).first);
        //std::cout<<"value !p1 "<<v_<<std::endl;
        beta = std::min(beta, v_);
        if(beta <= alpha)
          break;
      }
      return std::make_pair(v_, *vert_);
    }
  } 
}
#endif
