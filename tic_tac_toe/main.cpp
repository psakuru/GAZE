#include<iostream>
#include<algorithm>
#include<vector>
#include<climits>
#include<memory>
#include"../gaze/gametree.hpp"

using namespace std;

class gameState{
  public:
    typedef typename std::vector<gameState>::iterator state_iterator;
    enum option{
      X,
      O,
      B
    };

    gameState() :board_size(9) {
      squares = new gameState::option[9]; 
      std::uninitialized_fill(squares, (squares+board_size), gameState::option::B);
    }

    gameState(const gameState& state_) {
      board_size = state_.board_size;
      squares = new gameState::option[9];      
      for(int i =0; i<board_size; i++) {
        squares[i] = state_.squares[i];
      }
    }

    ~gameState() {
      delete[] squares;
    }

    gameState& operator=(const gameState& state_) {
      board_size = state_.board_size;
      for(int i =0; i<board_size; i++) {
        squares[i] = state_.squares[i];
      }
      return *this;
    }

    gameState::option get_square(int i){
      return squares[i];
    }

    void set_square(int i,gameState::option option_) {
      squares[i] = option_;
    }

    static int min_state_value() {
      return INT_MIN;
    }
    
    static int max_state_value() {
      return INT_MAX;
    }

    int get_value() {
      for(int i = 0; i<8;i++){
        if(squares[win_moves[i][0]] != gameState::option::B && (squares[win_moves[i][0]] == squares[win_moves[i][1]] && squares[win_moves[i][0]] == squares[win_moves[i][2]]))
          if(squares[win_moves[i][0]] == gameState::option::X )
            return gameState::max_state_value();
          else
            return gameState::min_state_value();
      } 
      return 0;
    }

    std::vector<gameState> get_children(){
      std::vector<int> blnks = get_blanks();
      std::vector<gameState> v;

      gameState::option next = (blnks.size() % 2 == 1)? gameState::option::X : gameState::option::O;
      std::for_each(blnks.begin(),blnks.end(),[&](auto i){
        gameState gs(*this);
        gs.set_square(i,next);
        v.push_back(gs);
      });
      return v;
    }

    friend ostream& operator<<(ostream&, const gameState&);

  private:
    int board_size;
    gameState::option* squares; 
    static constexpr int win_moves[8][3] = {{0,1,2},{3,4,5},{6,7,8},{0,3,6},{1,4,7},{2,5,8},{0,4,8},{2,4,6}};

    std::vector<int> get_blanks() {
      std::vector<int> blanks;
      for(int i = 0;i<board_size; i++) {
        if(squares[i] == gameState::option::B)
          blanks.push_back(i);
      }
      return blanks;
    }

};

constexpr int gameState::win_moves[8][3];

ostream& operator<<(ostream& os, const gameState& gt) {
  for(int i = 0;i<gt.board_size;i++){
    if(gt.squares[i] == gameState::option::X)
      os<<"X";
    else if(gt.squares[i] == gameState::option::O)
      os<<"O";
    else
      os<<"B";
  }
  os<<std::endl;
  return os;
}

int main(){
  gameState g;
  cout<<g.get_square(1)<<endl;
  cout<<gameState::option::X<<endl;
  cout<<gameState::option::O<<endl;
  cout<<gameState::option::B<<endl;
  auto out_end = g.get_children();
  std::for_each(out_end.begin(),out_end.end(),[&](auto i){cout<<i<<endl;});
  return 0;
}
