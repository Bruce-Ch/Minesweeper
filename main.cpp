#include "plate.h"
#include <iostream>
#include <string>
using namespace std;

int row, col, num_of_mine;

int welcome(){
    cout << "Welcome to Minesweeper!\nIt's your first time to play this game, let's configure your game!\nIf you "
            "do not know how to play it, type in 'help'. Otherwise, type in 'continue' to start configuring your game." << endl;
    string tmp;
    cin >> tmp;
    if(tmp == "help"){
        return 1;
    } else {
        return 0;
    }
}

int configure(){
    cout << "Please type in the total number of rows." << endl;
    cin >> row;
    cout << "Please type in the total number of columns." << endl;
    cin >> col;
    cout << "Please type in the total number of mines." << endl;
    cin >> num_of_mine;
    if(num_of_mine <= row * col && num_of_mine >= 0) {
        cout << "Your configuration is below. Do you want to start the game now?(y/n)" << endl;
        cout << "row: " << row << "\tcol: " << col << "\tmines: " << num_of_mine << endl;
        char tmp;
        cin >> tmp;
        if(tmp == 'y'){
            return 1;
        } else {
            return 0;
        }
    } else {
        return -1;
    }
}

int reconfigure(bool with_wrong){
    if(with_wrong){
        cout << "There is something wrong in your configuration, please reconfigure it.\n";
        cout << "row: " << row << "\tcol: " << col << "\tmines: " << num_of_mine << endl;
    } else {
        cout << "Let's reconfigure your game!" << endl;
    }
    return configure();
}

int game(){
    auto * p = new plate(row, col, num_of_mine);
    while(true){
        if(p->won() || p->beaten){
            if(p->beaten){
                cout << "You lose." << endl;
            } else {
                cout << "You win." << endl;
            }
            break;
        }

        string tmp;
        cin >> tmp;
        if(tmp == "click"){
            int pos_row, pos_col;
            cin >> pos_row >> pos_col;
            p->real_click(pos_row, pos_col);
        }
        else if(tmp == "mark"){
            int pos_row, pos_col;
            cin >> pos_row >> pos_col;
            p->real_mark(pos_row, pos_col);
        }
        else if(tmp == "auto"){
            int pos_row, pos_col;
            cin >> pos_row >> pos_col;
            p->real_auto_click(pos_row, pos_col);
        }
    }
    delete p;

    cout << "Restart: 1\tReconfigure: 2\tExit: 3" << endl;
    int tmp;
    cin >> tmp;
    return tmp;
}

void help(){
    cout << "This is the game's help document. You will know how to play the game here.\n"
            "Now you should have known the basic rule of Minesweeper. But in this game, you have to type in all your commands.\n"
            "Here are all the three commands available.\n"
            "1.You can use 'click' to click some place like this: 'click 4 4', which is like you click 4 4 in normal Minesweeper game.\n"
            "2.You can use 'mark' to mark that there's a mine like this: 'mark 4 4'. To clear the mark, just use 'mark 4 4' again.\n"
            "3.When you have mark all the mines around a place, you can auto_click the place like this: 'auto 4 4', to click \n"
            "  all the place around it.\n"
            "Now type in 'continue' to continue.\n";
    string tmp;
    cin >> tmp;
}

void bye(){
    cout << "Thanks for playing the game. See you next time!" << endl;
}

int main(){
    if(welcome() == 1){
        help();
    }
    int tmp = configure();
    while(tmp != 1){
        tmp = reconfigure(tmp == -1);
    }

    while(true){
        tmp = game();
        if(tmp == 3){
            break;
        }
        else if(tmp == 1){
            continue;
        }
        else if(tmp == 2){
            do{
                tmp = reconfigure(tmp == -1);
            }while(tmp != 1);
        }
    }

    bye();
    return 0;
}