#include<iostream>
#include<fstream>
#include<ctime>

using namespace std;
#define W 1001
#define H 1001
enum direction {right, down, start};
int matrix[H][W];
int minCost[H][W];//记录到达点(H,W)的最小代价
int backward[H][W];//记录上一步的移动方向
int MinCostPath(){
    minCost[0][0] = matrix[0][0];
    backward[0][0] = start;
    for(int i = 1; i < W; ++i){
        minCost[0][i] = minCost[0][i - 1] + matrix[0][i];
        backward[0][i] = direction::right;
    }
    for(int i = 1; i < H; ++i){
        minCost[i][0] = minCost[i - 1][0] + matrix[i][0];
        backward[i][0] = direction::down;
    }
    for(int i = 1; i < H; ++i){
        for(int j = 1; j < W; ++j){
            if(minCost[i - 1][j] > minCost[i][j - 1]){
                minCost[i][j] = minCost[i][j - 1] + matrix[i][j];
                backward[i][j] = direction::right;
            }
            else{
                minCost[i][j] = minCost[i - 1][j];
                backward[i][j] = direction::down;
            }
        }
    }
    return minCost[H - 1][W - 1];
}

int main(){
    fstream file;
    file.open("RandomGeneratedNumber.txt");
    if(!file){
        cout << "Open file error!\n";
        return 1;
    }
    for(int i = 0; i < H; ++i){
        for(int j = 0; j < W; ++j){
            file >> matrix[i][j];
        }
    }
    time_t begin, end;
    begin = clock();
    int result = MinCostPath();
    end = clock();
    double duration = (double)(end - begin)/CLOCKS_PER_SEC;
    printf("The minimum cost of the path from point (0, 0) to point (%d, %d) is %d\nThe time cost is %f s\n", H - 1, W - 1, result, duration);
    file.close();
    system("pause");
    return 0;
}