#include <iostream>
#include <climits>
#include <time.h>
#include <vector>
#include <cmath>
#include <sstream>
#include <fstream>

using namespace std;

#define T0 100
#define T 0.95
#define Tf 0.01
#define IN_loop 100

bool positive(int val) {
    if(val > 0) return 1;
    else return 0;
}

int subset_eq_one(vector<vector<int>> &subsets, vector<bool> &input) {
    int num = 0;
    int result;
    for(auto subset:subsets) {
        result = 1;
        for(auto val:subset) {
            if(positive(val)^input[abs(val)-1] == 0) {
                result = 0;
                break;
            }
        }
        num += result;
    }
    return num;
}

vector<bool> get_new_input(vector<bool> &input) {
    int index = (int)(rand()%input.size());
    vector<bool> new_input(input);
    new_input[index] = !new_input[index];
    return new_input;
}

int SA(vector<vector<int>> &subsets, vector<bool> &input) {
    double t = T0;
	double delta, p;
	int i, loop_counter = 0;
    vector<bool> best_input;
    int most_subsets = 0;

	while (t > Tf)
	{
		for (i = 0; i < IN_loop; i++)
		{
			loop_counter++;

			auto new_input = get_new_input(input);
			delta = subset_eq_one(subsets, new_input) - subset_eq_one(subsets, input);
			if (delta > 0)
			{
				input = new_input;
			}
			else
			{
				p = rand()*1.0 / RAND_MAX;
				if (exp(-delta / t) > p)
					input = new_input;
			}
            
            clog << "\rThe temperature now is " << t << ", and this is loop " << i << ".The most circuits outputing 1 now is " << most_subsets << ' ' << flush;
		}
		if (subset_eq_one(subsets, input) > most_subsets)
		{
			best_input = input;
            most_subsets = subset_eq_one(subsets, input);
		}
		t = t*T;
	}
    clog << "\rDone                                                                                                      \n";
    input = best_input;
    return loop_counter;
}

int main()
{
    int most_circuit;
    int loop_counter;
    int size_of_var= 0;
    int index, i;
    vector<vector<int>> subsets;
    string line;
    ifstream inFile("lab3.dat");
    if(!inFile) cerr << "Open file wrong" << endl;
    getline(inFile, line, '\n');
    i = 0;
    while(line.length() != 0) {
        stringstream subset(line);
        subsets.push_back(vector<int>());
        while(subset >> index) {
            subsets[i].push_back(index);
        }
        ++i;
        getline(inFile, line, '\n');
    }
    inFile.close();

    for(auto subset:subsets) {
        for(auto var:subset) {
            if(size_of_var < abs(var)) {
                size_of_var = var;
            }
        }
    }
    vector<bool> input(size_of_var, 0);
    srand(time(NULL));
    time_t begin = clock();
    loop_counter = SA(subsets, input);
    time_t end = clock();
    double duration = (double)(end - begin)/CLOCKS_PER_SEC;
    printf("After %d loops, the best result is %d circuits outputing 1. The time cost is %f seconds.\n", loop_counter, subset_eq_one(subsets, input), duration);
    printf("The input is:");
    for(auto var:input) cout << var << ' ';
    cout << endl;
    system("pause");

    return 0;
}
