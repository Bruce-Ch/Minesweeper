//
// Created by pc on 2020/1/20.
//

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>
#include "plate.h"

const int plate::neighbour_row[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
const int plate::neighbour_col[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

void plate::shuffle(int * p, int size) {
    srand(time(nullptr));
    for(int i = size - 1; i >= 0; i --){
        int tmp = rand() % (i + 1);
        exchange(p[tmp], p[i]);
    }
}

void plate::exchange(int &a, int &b) {
    int tmp = b;
    b = a;
    a = tmp;
}

void plate::init() {
    int total = col * row;
    int * p = new int[total];
    for(int i = 0; i < total; i ++){
        p[i] = i;
    }
    shuffle(p, total);
    for(int i = 0; i < row; i ++){
        for(int j = 0; j < col; j ++){
            state[i][j] = 0;
            data[i][j] = 0;
            mask[i][j] = 0;
        }
    }
    for(int i = 0; i < num_of_mine; i ++){
        state[p[i] / col][p[i] % col] = 1;
    }
    delete [] p;
    count();
    build_log();
    show();
}

void plate::raw_show() {
    for(int i = 0; i < row; i ++){
        for(int j = 0; j < col; j ++){
            std::cout << state[i][j] << ' ';
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    for(int i = 0; i < row; i ++){
        for(int j = 0; j < col; j ++){
            std::cout << data[i][j] << ' ';
        }
        std::cout << std::endl;
    }
}

bool plate::check(int now_row, int now_col, int idx) {
    return (now_row + neighbour_row[idx] >= 0 && now_row + neighbour_row[idx] < row
            && now_col + neighbour_col[idx] >= 0 && now_col + neighbour_col[idx] < col);
}

void plate::count() {
    for(int i = 0; i < row; i ++){
        for(int j = 0; j < col; j ++){
            if(state[i][j]){
                for(int k = 0; k < 8; k ++){
                    if(check(i, j, k)){
                        data[i + neighbour_row[k]][j + neighbour_col[k]] ++;
                    }
                }
            }
        }
    }
}

void plate::click(int pos_row, int pos_col) {
    if(pos_row < 0 || pos_row >= row || pos_col < 0 || pos_col >= col){
        return;
    }
    if(mask[pos_row][pos_col]){
        return;
    }

    if(state[pos_row][pos_col]){
        beaten = true;
        return;
    }

    mask[pos_row][pos_col] = 1;
    rest --;
    if(data[pos_row][pos_col] == 0){
        for(int i = 0; i < 8; i ++){
            if(check(pos_row, pos_col, i)){
                click(pos_row + neighbour_row[i], pos_col + neighbour_col[i]);
            }
        }
    }
}

void plate::mark(int pos_row, int pos_col) {
    if(pos_row < 0 || pos_row >= row || pos_col < 0 || pos_col >= col){
        return;
    }
    if(mask[pos_row][pos_col] == 1){
        return;
    }

    if(mask[pos_row][pos_col] == -1){
        mask[pos_row][pos_col] = 0;
        return;
    }
    if(mask[pos_row][pos_col] == 0){
        mask[pos_row][pos_col] = -1;
        return;
    }
}

void plate::auto_click(int pos_row, int pos_col) {
    if(pos_row < 0 || pos_row >= row || pos_col < 0 || pos_col >= col){
        return;
    }
    if(mask[pos_row][pos_col] != 1){
        return;
    }

    int cnt = 0;
    for(int i = 0; i < 8; i ++){
        if(check(pos_row, pos_col, i) && mask[pos_row + neighbour_row[i]][pos_col + neighbour_col[i]] == -1){
            cnt ++;
        }
    }
    if(cnt == data[pos_row][pos_col]){
        for(int i = 0; i < 8; i ++){
            if(check(pos_row, pos_col, i)){
                click(pos_row + neighbour_row[i], pos_col + neighbour_col[i]);
            }
        }
    }
}

void plate::show() {
    //输出列号
    std::cout << "   ";
    for(int i = 0; i < col; i ++){
        if(i / 100){
            std::cout << ' ' << i;
        }
        else if(i / 10){
            std::cout << ' ' << i << ' ';
        }
        else{
            std::cout << "  " << i << ' ';
        }
    }
    std::cout << std::endl;

    for(int i = 0; i < row; i ++){
        std::cout << "   ";
        for(int j = 0; j < 4 * col + 1; j ++){
            std::cout << ' ';//#';
        }
        std::cout << std::endl;
        if(i / 100){
            std::cout << i;
        }
        else if(i / 10){
            std::cout << i << ' ';
        }
        else {
            std::cout << ' ' << i << ' ';
        }
        for(int j = 0; j < col; j ++){
            std::cout << "  ";//# ";
            if(mask[i][j] == -1){
                std::cout << '*';
            }
            else if(mask[i][j] == 0){
                std::cout << '-';
            }
            else if(mask[i][j] == 1){
                if(data[i][j]){
                    std::cout << data[i][j];
                } else {
                    std::cout << ' ';
                }
            }
            std::cout << ' ';
        }
        std::cout << ' ' << std::endl;//here
    }
    std::cout << "   ";
    for(int j = 0; j < 4 * col + 1; j ++){
        std::cout << ' ';//#';
    }
    std::cout << std::endl << std::endl;
}

void plate::real_click(int pos_row, int pos_col) {
    click(pos_row, pos_col);
    build_log("click", pos_row, pos_col);
    if(won() || beaten){
        return;
    }
    show();
}

void plate::real_mark(int pos_row, int pos_col) {
    mark(pos_row, pos_col);
    build_log("mark", pos_row, pos_col);
    show();
}

void plate::real_auto_click(int pos_row, int pos_col) {
    auto_click(pos_row, pos_col);
    build_log("auto", pos_row, pos_col);
    if(won() || beaten){
        return;
    }
    show();
}

bool plate::won() {
    return rest == num_of_mine;
}

void plate::build_log() {
    std::ofstream fout("log.txt", std::ios::app);

    time_t now = time(nullptr);
    char *dt = ctime(&now);
    fout << dt << std::endl;
    for(int i = 0; i < row; i ++){
        for(int j = 0; j < col; j ++){
            fout << state[i][j] << ' ';
        }
        fout << std::endl;
    }
    fout << std::endl;
    for(int i = 0; i < row; i ++){
        for(int j = 0; j < col; j ++){
            fout << data[i][j] << ' ';
        }
        fout << std::endl;
    }
    fout << std::endl;

    for(int i = 0; i < row; i ++){
        for(int j = 0; j < col; j ++){
            fout << mask[i][j] << ' ';
        }
        fout << std::endl;
    }
    fout << std::endl;
    fout << std::endl;

    fout.close();
}

void plate::build_log(std::string com, int pos_row, int pos_col) {
    std::ofstream fout("log.txt", std::ios::app);

    fout << com << ' ' << pos_row << ' ' << pos_col << std::endl;

    for(int i = 0; i < row; i ++){
        for(int j = 0; j < col; j ++){
            fout << mask[i][j] << ' ';
        }
        fout << std::endl;
    }
    fout << std::endl;
    fout << std::endl;

    fout.close();
}