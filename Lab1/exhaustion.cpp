#include<fstream>
#include<vector>
#include<iostream>
#include<time.h>
#include<cmath>
using namespace std;

#define fileSize 1000
string fileName = "points_1K.txt";
class Coordinate{
	public:
	Coordinate(int num, int xi, int yi) : number(num), x(xi), y(yi){}
	Coordinate(const Coordinate &other) : number(other.number), x(other.x), y(other.y){}
	Coordinate(){}
	~Coordinate(){}
	int x, y;
	int number;
};
int DisSquare(Coordinate &p, Coordinate &q){
	return pow(p.x - q.x, 2) + pow(p.y - q.y, 2);
}
int main()
{
	ifstream infile;
	infile.open("F:/files/vscode/cpp_source/points_1K.txt",ios::in);
	if(!infile){
		cout << "Error opening file!\n";
		return -1; 
	}
	vector<Coordinate> A;
	char c;
	int num;
	int x, y;
	for(int i = 0; i < fileSize; ++i)
	{
		num = i + 1;
		while(c = infile.get() != '(');
		infile >> x;
		c = infile.get();
		infile >> y;
		A.push_back(Coordinate(num, x, y));
	}
    clock_t begin, end;//用于记算算法的耗时
	double duration;
	begin = clock();
    int P = 0, Q = 1, d2 = DisSquare(A[0], A[1]);
	for(int i = 0; i < fileSize; ++i){
        for(int j = i + 1; j < fileSize; ++j){
            if(d2 > DisSquare(A[i], A[j])){
                P = i;
                Q = j;
                d2 = DisSquare(A[i], A[j]);
            }
        }
    }
	end = clock();
	duration = (double)(end - begin)/CLOCKS_PER_SEC;
	infile.close();
	printf("The closest points are point %d and point %d,the distance between them is %f\n", P, Q, sqrt(d2));
	printf("The time cost is %f seconds\n", duration);
	system("pause");
	return 0;
}