#ifndef GAME_TRAITS_HPP
#define GAME_TRAITS_HPP

namespace gaze{ 

 template <typename game_tree>
 struct game_traits {
   typedef typename game_tree::vertex_property vertex_property;
   typedef typename game_tree::vertex_iterator vertex_iterator;
   typedef typename game_tree::game_state game_state;
   typedef typename game_tree::state_value_type state_value_type;
   typedef typename game_tree::vertex_property::degree_size_type degree_size_type;
 };

}

#endif

/*typedef typename game_tree::vertices_size_type     vertices_size_type;
typedef typename game_tree::in_edge_iterator       in_edge_iterator;
typedef typename game_tree::vertex_iterator        vertex_iterator;
typedef typename game_tree::edge_iterator          edge_iterator;
typedef typename game_tree::directed_category      directed_category;
typedef typename game_tree::edge_parallel_category edge_parallel_category;
typedef typename game_tree::traversal_category     traversal_category;
typedef typename game_tree::edges_size_type        edges_size_type;*/
