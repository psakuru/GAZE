#include<gaze>

class gameState{

};

int main(){
  gameState g;
  GameTree<g> t;
  while(1){
    next_state = gaze::alphabeta(g,4,INT_MAX, INT_MIN, true);
  }
  return 0;
}
