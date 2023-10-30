#include<fstream>
#include<vector>
#include<iostream>
#include<cmath>
#include<algorithm>
#include<time.h>
using namespace std;

#define fileSize 1000
string fileName = "points_1K.txt";
class Coordinate{
	public:
	Coordinate(int num, int xi, int yi) : number(num), x(xi), y(yi){}
	Coordinate(const Coordinate &other) : number(other.number), x(other.x), y(other.y), z(other.z){}
	Coordinate(){}
	~Coordinate(){}
	int x, y, z;
	int number;
	Coordinate &operator=(const Coordinate &other){
		if(this == &other) return *this;
		this->number = other.number;
		this->x = other.x;
		this->y = other.y;
		this->z = other.z;
		return *this;
	}
};
int DisSquare(Coordinate &p, Coordinate &q){
	return pow(p.x - q.x, 2) + pow(p.y - q.y, 2);
}
bool CompareX(Coordinate &p, Coordinate &q){
	return p.x < q.x;
}
bool CompareY(Coordinate &p, Coordinate &q){
	return p.y < q.y;
}
class returnValue{
	public:
	Coordinate P, Q;
	int disSquare;
	returnValue(Coordinate &Pi, Coordinate &Qi, int disSquarei):P(Pi), Q(Qi), disSquare(disSquarei){}
	returnValue(const returnValue &other):P(other.P), Q(other.Q),disSquare(other.disSquare){}
	returnValue(){}
	~returnValue(){}
};

returnValue ClosestPoint(vector<Coordinate> &A, vector<Coordinate> &B, int n)
{
	if(n == 2) {
		return returnValue(A[0], A[1], DisSquare(A[0], A[1]));
	}
	else if(n == 3) 
	{
		int dis01, dis12, dis20;
		dis01 = DisSquare(A[0], A[1]);
		dis12 = DisSquare(A[1], A[2]);
		dis20 = DisSquare(A[2], A[0]);
		if(dis01 <= dis12 && dis01 <= dis20) return returnValue(A[0], A[1], dis01);
		else if(dis12 < dis01 && dis12 <= dis20) return returnValue(A[1], A[2], dis12);
		else return returnValue(A[2], A[0], dis20);
	}
	
	int k = n/2, s = 0, t = 0;//划分
	vector<Coordinate> AL(k), AR(n-k), BL(k), BR(n-k);
	AR.assign(A.begin() + k, A.end());
	AL.assign(A.begin(), A.begin() + k);
	for(int i = 0; i < n; ++i) B[A[i].z].z = i;
	for(int j = 0; j < n; ++j)
	{
		if(B[j].z < k){
			BL[s] = B[j];
			AL[B[j].z].z = s;
			++s;
		}
		else{
			BR[t] = B[j];
			AR[B[j].z - k].z = t;
			++t;
		}
	}
	returnValue resultL = ClosestPoint(AL, BL, k);//递归求解
	returnValue resultR = ClosestPoint(AR, BR, n - k);
	int d2 = min(resultL.disSquare, resultR.disSquare);
	float d = sqrt(d2);
	Coordinate P, Q;
	if(d2 == resultL.disSquare){
		P = resultL.P;
		Q = resultL.Q;
	}
	else{
		P = resultR.P;
		Q = resultR.Q;
	}
	int j = 0;
	int m = AL[k-1].x;
	vector<Coordinate> R(n-k);
	vector<bool> exist(n-k);
	for(int i = 0; i < n - k; ++i){
		if(BR[i].x <= m + d){
			R[i] = BR[i];
			exist[i] = 1;
		}//过滤不在右临界点的点
		else exist[i] = 0;
	}
	for(int i = 0; i < k; ++i){
		if(BL[i].x < m - d) continue;
		while(j < n-k && (exist[j] == 0 || R[j].y < BL[i].y - d)) ++j;//跳过无关点
		int u = 0;
		if(j < n - k){
			while(R[j + u].y <= BL[i].y + d){
				if(DisSquare(BL[i], R[j + u]) < d2){
					d2 = DisSquare(BL[i], R[j + u]);
					P = BL[i];
					Q = R[j + u];
				}
				++u;
				while(j + u < n - k && exist[j + u] == 0) ++u;
				if(j + u == n - k) break;
			}
		}
	}
	return returnValue(P, Q, d2);
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
	vector<Coordinate> B(A);
	sort(A.begin(), A.end(), CompareX);
	sort(B.begin(), B.end(), CompareY);
	static vector<int> pos(fileSize);//用来记录A对应点在B中的位置
	for(int i = 0; i < fileSize; ++i)
	{
		pos[B[i].number  - 1] = i;
	}
	for(int i = 0; i < fileSize; ++i)
	{
		A[i].z = pos[A[i].number  - 1];//A[i]在B中的位置
	}
	returnValue result(ClosestPoint(A, B, fileSize));
	end = clock();
	duration = (double)(end - begin)/CLOCKS_PER_SEC;
	infile.close();
	printf("The closest points are point %d and point %d,the distance between them is %f\n", result.P.number, result.Q.number, sqrt(result.disSquare));
	printf("The time cost is %f seconds\n", duration);
	system("pause");
	return 0;
}