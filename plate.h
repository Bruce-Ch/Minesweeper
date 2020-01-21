//
// Created by pc on 2020/1/20.
//

#ifndef MINESWEEPER_PLATE_H
#define MINESWEEPER_PLATE_H

#include <string>

class plate{
    int rest;
    int ** state;
    int ** data;
    int ** mask;
    int row, col, num_of_mine;
    const static int neighbour_row[8];
    const static int neighbour_col[8];

public:
    plate(int row, int col, int num_of_mine): beaten(false), rest(row * col) {
        this->row = row;
        this->col = col;
        this->num_of_mine = num_of_mine;

        state = new int * [row];
        data = new int * [row];
        mask = new int * [row];
        for(int i = 0; i < row; i ++){
            state[i] = new int [col];
            data[i] = new int [col];
            mask[i] = new int [col];
        }
        init();
    }
    ~plate(){
        for(int i = 0; i < row; i ++){
            delete [] state[i];
            delete [] data[i];
            delete [] mask[i];
        }
        delete [] state;
        delete [] data;
        delete [] mask;
    }

    bool beaten;
    bool won();

    void raw_show();
    void real_click(int pos_row, int pos_col);
    void real_mark(int pos_row, int pos_col);
    void real_auto_click(int pos_row, int pos_col);

private:
    void init();
    void static shuffle(int * p, int size);//将一维数组打乱
    void static exchange(int & a, int & b);
    bool check(int now_row, int now_col, int idx);
    void count();
    void click(int pos_row, int pos_col);
    void mark(int pos_row, int pos_col);
    void auto_click(int pos_row, int pos_col);
    void show();
    void build_log();
    void build_log(std::string com, int pos_row, int pos_col);
};

#endif //MINESWEEPER_PLATE_H
