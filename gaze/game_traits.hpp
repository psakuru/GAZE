#ifndef GAME_TRAITS_HPP
#define GAME_TRAITS_HPP

namespace gaze{ 

 /**
  * Container for holding asscoiated types of a Game Tree Concept
  *
  */
 template <typename game_tree>
 struct game_traits {

   /**
    * returns the type of the vertex held in the game tree
    */
   typedef typename game_tree::vertex_property vertex_property;

   /**
    * returns the iterator type that iterates over the game tree vertices
    */
   typedef typename game_tree::vertex_iterator vertex_iterator;

   /**
    * returns the type of the game state stored in each vertex
    */
   typedef typename game_tree::game_state game_state;

   /**
    * returns the type of the value used to compare various game states
    */
   typedef typename game_tree::state_value_type state_value_type;

   /**
    * returns the type out degree size returned by children_count
    */
   typedef typename game_tree::vertex_property::degree_size_type degree_size_type;
 };

}

#endif

