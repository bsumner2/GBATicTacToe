// Copyright 2023 AE

#include<iostream>
using std::cout;
using std::cin;
using std::endl;
using std::ostream;
using std::string;
using std::runtime_error;

class State{
    // The dimention of our board
    static const int kdim = 3;
    friend ostream& operator << (ostream& whereto, const State& s);
   // Game layout 2D char array of - , X , O
 public:
    
    State(){
        setDefaultLayOut();
        setString();
    }

    State(char layOut[][kdim] ){
        layOut_ = new char*[kdim];
        setlayOut(layOut);
    }

    ~State(){
        for (int i = 0; i < kdim; ++i) {
            delete[] layOut_[i]; 
        }
        delete[] layOut_;
    }
    State& operator = (State& s) {
        layOut_ = new char*[kdim];

        for (int i = 0; i < kdim; ++i) {
            layOut_[i] = new char[kdim];
            for (int j = 0; j < kdim; ++j) {
                layOut_[i][j] = s.getlayOut()[i][j];
            }
        }
        dim_ = s.dim_;
        return *this;
    }

   // Check if the state is finished
    bool checkFinish(){
        for (int i = 0; i < kdim; ++i) {
            for (int j = 0; j < kdim; ++j) {
                if (this->layOut_[i][j] == '-'){
                    return false;
                }
            }
        }
        return true;
    }

    // return 1 if X won, -1 if O. 0 if none and error if
    // not finished
    int score(){
        if (!checkFinish()){
          throw runtime_error("Game is not finished!");
        }
       if(ifWon('X')) {
        return 1;
       }else if (ifWon('O')) {
        return -1;
       }else {
        return 0;
       }
    }
    // Check if the char 'x' won the game 
    bool ifWon(char x) {
        for (int i = 0; i < kdim; ++i) {
            if (layOut_[0][i] == x && layOut_[1][i] == x &&
                layOut_[2][i] == x) {
                    return true;
                }
            if (layOut_[i][0] == x && layOut_[i][1] == x &&
                layOut_[i][2] == x) {
                    return true;
                }  
        }
            if (layOut_[0][0] == x && layOut_[1][1] == x &&
                layOut_[2][2] == x) {
                    return true;
                }  
            if (layOut_[0][2] == x && layOut_[1][1] == x &&
                layOut_[2][0] == x) {
                    return true;
                }
        return false;
            
    }
    // Set based on a 2D char array
    void setlayOut(char layOut[][kdim] ){
        for (int i = 0; i < kdim; ++i) {
            for (int j = 0; j < kdim; ++j) {
                this->layOut_[i][j] = layOut[i][j];
            }
        }
        setString();
    }
    // set the "change" to the position a and b 
    void setij(int a, int b, char change) {
        if(layOut_[a][b] == '-'){
            layOut_[a][b] = change;
        }
        setString();
    }

    void setDefaultLayOut(){
        layOut_ = new char*[kdim];
        for (int i = 0; i < kdim; ++i) {
            layOut_[i] = new char[kdim];
            for (int j = 0; j < kdim; ++j) {
                this->layOut_[i][j] = '-';
                // cout << "HERE" << endl;
            }
        }

    }


    char** getlayOut(){
        return layOut_;
    }

    string getString(){
        return dim_;
    }

 private:
    // The String representation of the game OOX\nXOX\nXOX
    string dim_;
    // Char 2D array saving each spot
    char** layOut_;
    // Value of the state if the game is finishe, 1 for X win, -1 for 
    //  O win and 0 for tie
    int value_;

    void setString(){
        dim_ = "";
        for (int i = 0; i < kdim; ++i) {
            for (int j = 0; j < kdim; ++j) {
                dim_ += layOut_[i][j];
            }
            dim_+="\n";
        }
    }


};
// Overriding the << operator
ostream& operator << (ostream& whereto, const State& r){
    whereto << r.dim_; 
    return whereto;
}


class Game{

friend class State;

 public:


    Game(){
        turn = 'X';
        currentState_ = new State();
    }

    char getTurn(){
        return turn;
    }

    State* play(int a, int b){
        if(currentState_->checkFinish()){
            return currentState_;
        }
        currentState_->setij(a, b, turn);
        nextTurn();
        return currentState_;
    }
    // Finding all possible next steps
    State* nextStates(){
        State* states = new State[4];
        State s = State();
        states[0] = s;
        states[1] = s;
        states[2] = s;
        return states;
    }
    // get current state
    State* getState() {return currentState_;}
 private:
    char turn;
    State* currentState_;


    void nextTurn(){
        if (turn == 'X'){
            turn = 'O';
        } else {
            turn = 'X';
        }
    }

};



int main() {
    const int kSize = 3;
    Game g = Game();
    State* s = g.getState();
    
    cout << *s << endl;

    for (int i = 0; i < kSize + 5 ; ++i) {
        g.play(0,0);
        cout << *s ;
        if(s->checkFinish()) {
            cout << s->score() << endl;
        }
    }
    g.play(0,0);
    cout << *s << endl; 
    g.play(0,0);
    cout << *s << endl; 
    // for (int i = 0; i < 10; ++i) {
    //     s.nextTurn();
    //     cout << s.getTurn() << endl;
    // }
    cout << "____" << endl;
    State j = *s;
    j.setij(2,2,'O');
    State* states;
    states = g.nextStates();
    cout << sizeof(states);
    for (int i = 0; i < 4; ++i) {
        if (&states[i] != nullptr) {
            cout << states[i];
        }else{
            cout << "NULL PTR" << endl;
        }
    }
    return 0;
}


