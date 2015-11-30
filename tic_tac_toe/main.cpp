#include<iostream>
#include<algorithm>
#include<vector>
#include<climits>
#include<memory>
#include"../gaze/gametree.hpp"
#include"../gaze/algorithms/alpha-beta.hpp"

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
      std::uninitialized_fill(squares, (squares+board_size),
                                        gameState::option::B);
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
        if(squares[win_moves[i][0]] != gameState::option::B &&
            (squares[win_moves[i][0]] == squares[win_moves[i][1]] &&
             squares[win_moves[i][0]] == squares[win_moves[i][2]])) {
          if(squares[win_moves[i][0]] == gameState::option::X ) {
            return gameState::max_state_value();
          } else {
            return gameState::min_state_value();
          }
        }
      }
      return 0;
    }

    std::unique_ptr<std::vector<gameState*>> get_children(){
      std::vector<int> blnks = get_blanks();
      //auto v = new std::vector<gameState*>;
      std::unique_ptr<std::vector<gameState*>> v(new std::vector<gameState*>);

      if(this->get_value() == gameState::max_state_value() ||
          this->get_value() == gameState::min_state_value())
        return v;

      gameState::option next = (blnks.size() % 2 == 1)? gameState::option::X :
                                                        gameState::option::O;
      std::for_each(blnks.begin(),blnks.end(),[&](auto i){
        gameState *gs= new gameState(*this);
        gs->set_square(i,next);
        v->push_back(gs);
      });
      return v;
    }

    bool operator==(const gameState& vt) {
      for(int i =0;i<board_size;i++){
        if(squares[i] != vt.squares[i])
          return false;
      }
      return true;
    }

    bool operator!=(const gameState& vt) {
      return !(*this == vt);
    }

    friend ostream& operator<<(ostream&, const gameState&);
  
  private:
    int board_size;
    gameState::option* squares;
    static constexpr int win_moves[8][3] = {{0,1,2},{3,4,5},{6,7,8},
                                            {0,3,6},{1,4,7},{2,5,8},
                                            {0,4,8},{2,4,6}};

    std::vector<int> get_blanks() {
      std::vector<int> blanks;
      for(int i = 0;i<board_size; i++) {
        if(squares[i] == gameState::option::B)
          blanks.push_back(i);
      }
      std::random_shuffle(blanks.begin(),blanks.end());
      return blanks;
    }

};

constexpr int gameState::win_moves[8][3];

std::string print_square(gameState::option a) {
    if(a == gameState::option::X)
      return "X";
    else if(a == gameState::option::O)
      return "O";
    else
      return "B";
}

ostream& operator<<(ostream& os, const gameState& gt) {
  os<<print_square(gt.squares[6])<<"\t"<<print_square(gt.squares[7])<<
                      "\t"<<print_square(gt.squares[8])<<"\n";
  os<<print_square(gt.squares[3])<<"\t"<<print_square(gt.squares[4])<<
                      "\t"<<print_square(gt.squares[5])<<"\n";
  os<<print_square(gt.squares[0])<<"\t"<<print_square(gt.squares[1])<<
                      "\t"<<print_square(gt.squares[2])<<"\n";
  os<<std::endl;
  return os;
}

int main(){
  gaze::game_tree<gameState> gt;
  bool player1 = true;
  int move = 1;
  auto pair_i = gaze::alphabeta(gt, 5, INT_MIN, INT_MAX, player1);
  gameState x;  
  while(pair_i.second->get_state() != x){
    x = pair_i.second->get_state();
    gt.set_current_state(x);
    cout<<"Move "<<move++<<"\n"<<x<<endl;
    player1 = !player1;
    pair_i = gaze::alphabeta(gt, 5, INT_MIN, INT_MAX, player1);

  }
  cout<<"last move was \n"<<(pair_i.second)->get_state();
  return 0;
}
