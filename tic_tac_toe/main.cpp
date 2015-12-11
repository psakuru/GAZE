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

    bool is_win() {
      bool ret = false;
       for(int i = 0; i<8;i++){
        if(squares[win_moves[i][0]] != gameState::option::B &&
            (squares[win_moves[i][0]] == squares[win_moves[i][1]] &&
             squares[win_moves[i][0]] == squares[win_moves[i][2]])) {
          ret = true;
          break;
        }
       }
      return ret;
    }


  int get_value() {
      int score = 0;
      // Evaluate score for each of the 8 lines (3 rows, 3 columns, 2 diagonals)
      score += evaluateLine(0, 1, 2);  // row 0
      score += evaluateLine(3, 4, 5);  // row 1
      score += evaluateLine(6, 7, 8);  // row 2
      score += evaluateLine(0, 3, 6);  // col 0
      score += evaluateLine(1, 4, 7);  // col 1
      score += evaluateLine(2, 5, 8);  // col 2
      score += evaluateLine(0, 4, 8);  // diagonal
      score += evaluateLine(2, 4, 6);  // alternate diagonal
      return score;
   }
 
   /** The heuristic evaluation function for the given line of 3 cells
       @Return +100, +10, +1 for 3-, 2-, 1-in-a-line for computer.
               -100, -10, -1 for 3-, 2-, 1-in-a-line for opponent.
               0 otherwise */
   int evaluateLine(int row1, int row2, int row3) {
      int score = 0;
      gameState::option mySeed =  gameState::option::X; 
      gameState::option oppSeed =  gameState::option::O; 
      // First cell
      if (squares[row1] == mySeed) {
         score = 1;
      } else if (squares[row1] == oppSeed) {
         score = -1;
      }
 
      // Second cell
      if (squares[row2] == mySeed) {
         if (score == 1) {   // cell1 is mySeed
            score = 10;
         } else if (score == -1) {  // cell1 is oppSeed
            return 0;
         } else {  // cell1 is empty
            score = 1;
         }
      } else if (squares[row2] == oppSeed) {
         if (score == -1) { // cell1 is oppSeed
            score = -10;
         } else if (score == 1) { // cell1 is mySeed
            return 0;
         } else {  // cell1 is empty
            score = -1;
         }
      }
 
      // Third cell
      if (squares[row3] == mySeed) {
         if (score > 0) {  // cell1 and/or cell2 is mySeed
            score *= 10;
         } else if (score < 0) {  // cell1 and/or cell2 is oppSeed
            return 0;
         } else {  // cell1 and cell2 are empty
            score = 1;
         }
      } else if (squares[row3] == oppSeed) {
         if (score < 0) {  // cell1 and/or cell2 is oppSeed
            score *= 10;
         } else if (score > 1) {  // cell1 and/or cell2 is mySeed
            return 0;
         } else {  // cell1 and cell2 are empty
            score = -1;
         }
      }

      return score;
   }

  std::unique_ptr<std::vector<gameState*>> get_children(){
      std::vector<int> blnks = get_blanks();
      //auto v = new std::vector<gameState*>;
      std::unique_ptr<std::vector<gameState*>> v(new std::vector<gameState*>);

      if(is_win())
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
      return ".";
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
  auto pair_i = gaze::alphabeta(gt, 2, INT_MIN, INT_MAX, player1);
  gameState x;  
  while(pair_i.second->get_state() != x){
    x = pair_i.second->get_state();
    gt.set_current_state(x);
    cout<<"Move "<<move++<<"\n"<<x<<endl;
    player1 = !player1;
    pair_i = gaze::alphabeta(gt, 2, INT_MIN, INT_MAX, player1);

  }
  cout<<"last move was \n"<<(pair_i.second)->get_state();
  return 0;
}
