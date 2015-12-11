#ifndef ALPHA_BETA_HPP
#define ALPHA_BETA_HPP

#include "../gametree.hpp"
#include "../game_traits.hpp"
#include <algorithm>
#include <utility>

namespace gaze{

  /**
   * alpha beta algorithm that runs on the current vertex of a game tree 
   * @return a pair containing the value of the prefered next state and the pointer to the next vertex on the game tree
   */
  template<typename game_tree>
  std::pair<typename game_traits<game_tree>::state_value_type, typename game_traits<game_tree>::vertex_property*> alphabeta(
                        game_tree& gt_,
                        int depth,
                        typename game_traits<game_tree>::state_value_type alpha,
                        typename game_traits<game_tree>::state_value_type beta,
                        bool player1) {
    return alphabeta<game_tree>(depth, alpha, beta, player1, gt_.get_current_vertex());
  }

  /**
   * alpha beta algorithm that runs on a particular vertex of the game tree 
   * @return a pair containing the value of the prefered next state and the pointer to the next vertex on the game tree
   */
  template<typename game_tree>
  std::pair<typename game_traits<game_tree>::state_value_type, typename game_traits<game_tree>::vertex_property*> alphabeta(
                        int depth_,
                        typename game_traits<game_tree>::state_value_type alpha,
                        typename game_traits<game_tree>::state_value_type beta,
                        bool player1,
                        typename game_traits<game_tree>::vertex_property& cur_) {
    typedef typename game_traits<game_tree>::vertex_property Vertex;
    typedef typename game_traits<game_tree>::state_value_type game_state_value;
    
    if(depth_ == 0 || cur_.get_children_count() == 0) {
      return std::make_pair(cur_.get_value(), &cur_);
    }
    
    Vertex* vert_;
    Vertex* ret_;
    if(player1) {
      game_state_value v_ = game_traits<game_tree>::game_state::min_state_value();
      auto it_pair = cur_.get_children();
      ret_ = &(*it_pair.first);
      for(; it_pair.first != it_pair.second; ++(it_pair.first)) {
        vert_ = &(*it_pair.first);
        auto branch_pair = alphabeta<game_tree>(depth_-1, alpha, beta, !player1, *vert_);
        v_ = std::max(v_, branch_pair.first);
        if(branch_pair.first == v_)
          ret_ = vert_;
        alpha = std::max(alpha, v_);
        if(beta <= alpha)
          break;
      }
      return std::make_pair(v_, ret_);
    } else {
      game_state_value v_ = game_traits<game_tree>::game_state::max_state_value();
      auto it_pair = cur_.get_children();
      ret_ = &(*it_pair.first);
      for(; it_pair.first != it_pair.second; ++(it_pair.first)) {
        vert_ = &(*it_pair.first);
        auto branch_pair = alphabeta<game_tree>(depth_-1, alpha, beta, !player1, *vert_);
        v_ = std::min(v_, branch_pair.first);
        if(branch_pair.first == v_)
          ret_ = vert_;
        beta = std::min(beta, v_);
        if(beta <= alpha)
          break;
      }
      return std::make_pair(v_, ret_);
    }
  } 
}
#endif
