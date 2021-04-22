#include <iostream>
#include <utility>
#include <fstream>
#include <stack>
#include <vector>
#include <string>
#include <time.h>

using namespace std;

class HillClimbing {
private:
	pair <int, int> graph[10][10];
	int dimension;

	vector<char> state; //yang sekarang sedang di eksplorasi dan menjadi induk
	vector<int> cur_state; //ini explored, yang sekarang sedang di eksplorasi dan menjadi induk

	vector<int> tmp;

	vector<vector<int>> resulting_child;

	vector<vector<int>> obj_func_val_checker;
	int obj_func_val;
	vector<int> func_val_tmp;

	int stage;

	//int xyz;

public:

	HillClimbing() {
		dimension = 0;
		obj_func_val = 0;
		stage = 0;
		//xyz = 0;
	}

	void inputFromFile(string namaFile, string traffic = "default") {
		fstream in_file(namaFile, ios::in);
		fstream f_traffic(traffic, ios::in);
		if (in_file.is_open()) {
			in_file >> dimension;

			/*while (!in_file.eof()) {
			
				in_file >> graph[co_row][co_col].first;
				graph[co_row][co_col].second = 1;


				if (co_col == 10) {
					co_col = -1;
					co_row++;
				}
				co_col++;
				
			}*/
			for (int i = 0; i < dimension; i++) {
				for (int j = 0; j < dimension; j++) {
					
					in_file >> graph[i][j].first;
					if (traffic == "default") {
						graph[i][j].second = 0;
					} else if (traffic == "rand") {
						if (graph[i][j].first == 0) {
							graph[i][j].second = 0;
						}
						else {
							graph[i][j].second = rand() % 5 + 1;
						}
					}
					else {
						f_traffic >> graph[i][j].second;
					}
				}
			}
		}
		displayCurrentFile();
	}

	void displayCurrentFile() {
		for (int i = 0; i < dimension; i++) {
			for (int j = 0; j < dimension; j++) {
				cout << "<";
				cout << graph[i][j].first << ", ";
				cout << graph[i][j].second;
				cout << "> ";
			} cout << endl;
		}
	}

	void definingInitState() {
		int current = 0;
		for (int i = 0; i < dimension; i++) {
			current = i;
			
			if (cur_state.empty()) {
				cur_state.push_back(current);
			}
			int co = 0;
			for (int j = 0; j < dimension; j++) {

				if (graph[i][j].first != 0 && j != current && !count(cur_state.begin(), cur_state.end(), j)) {
					cur_state.push_back(j);
					obj_func_val += graph[i][j].first + graph[i][j].second;
					break;
				}
				co = j;
			}

			if (co == dimension - 1) {
				break;
			}
			 
		}
		//obj_func_val_checker[stage].push_back(obj_func_val);
		stage++;

		func_val_tmp.push_back(obj_func_val);

		obj_func_val_checker.push_back(func_val_tmp);
		func_val_tmp.clear();		
	}

	void createChildNode() {
		int xyz = 0;
		for (int i = 0; i < dimension; i++) {
			for (int j = 0; j < dimension; j++) {
				if (swap(i, j, xyz) == true) {
					
					int checker = 0, checker2 = 0;
					bool redundant = false;

					if (resulting_child.empty()) {
						for (int q = 0; q < dimension; q++) {
							if (cur_state[q] == tmp[q]) {
								checker++;
							}
						}
						if (checker == dimension) {
							//redundant = true;
						}
					}
					
					for (int q = 0; q < resulting_child.size(); q++) {
						
						for (int w = 0; w < dimension; w++) {
							if (resulting_child[q][w] == tmp[w]) {
								checker++;
							}
							if (cur_state[w] == tmp[w]) {
								checker2++;
							}
						}

						if (checker == dimension) {
							redundant = true;
							break;
						}

						if (checker2 == dimension) {
							redundant = true;
							break;
						}

						checker = 0;
						checker2 = 0;
					}

					if (redundant != true) {
						resulting_child.push_back(tmp);
						func_val_tmp.push_back(xyz);
					}
					
					
					for (int q = 0; q < dimension; q++) {
						tmp[q] = cur_state[q];
					}
				}
				xyz = 0;
			}
		}

		obj_func_val_checker.push_back(func_val_tmp);
		func_val_tmp.clear();
	}

	bool swap(int i, int j, int &xyz)
	{
		/*if (graph[i][j].first != 0 && graph[i][j + 1].first != 0) {
			if (i - 1 > 0 && graph[j][i - 1].first != 0) {
				cout << i << ", " << j << " = "<< graph[i][j].first << "\t";
				cout << i << ", " << j + 1 << " = " << graph[i][j + 1].first << "\t";
				cout << j << ", " << i - 1 << " = " << graph[j][i - 1].first << endl;

				return true;
			}
			else {
				return false;
			}
		}
		else {
			return false;
		}*/

		for (int q = 0; q < dimension; q++) {
			tmp.push_back(cur_state[q]);
		}

		int temp = tmp[i];
		tmp[i] = tmp[j];
		tmp[j] = temp;
		
		for (int i = 0; i < dimension - 1; i++) {
			if (graph[tmp[i]][tmp[i+1]].first == 0) {
				return false;
			}
			xyz += graph[tmp[i]][tmp[i + 1]].first + graph[tmp[i]][tmp[i + 1]].second;
		}
		return true;
	}

	int checkNextSolution() {
		for (int i = 0; i < obj_func_val_checker[stage - 1].size(); i++) {
			if (obj_func_val_checker[stage - 1][i] < obj_func_val) {
				obj_func_val = obj_func_val_checker[stage - 1][i];

				//replace current with new
				for (int q = 0; q < dimension; q++) {
					cur_state[q] = resulting_child[i][q];
				}
				resulting_child.clear();

				return obj_func_val;
			}
		}
		return 0;
	}

	bool viewInitState() {
		for (int q = 0; q < cur_state.size(); q++) {
			state.push_back(char(65 + cur_state[q]));
			cout << state[q] << " ";
		}
		cout << "Val: " << obj_func_val << "\n";
		if (cur_state.size() != dimension) {
			cout << "\nSolution not found (berarti ada node yang harus dilewati 2x)" << endl;
			return false;
		}

		state.clear();
		return true;
	}

	void viewChildState() {
		for (int q = 0; q < resulting_child.size(); q++) {
			for (int w = 0; w < dimension; w++) {
				cout << char(65 + resulting_child[q][w]) << " ";
			}
			cout << "Val: " << obj_func_val_checker[stage][q] << endl;
		}


		state.clear();
	}

	void increaseStage() {
		stage++;
	}

	int probability() {
		return 0;
	}

};

int main() {
	srand(time(NULL));
	string destinations = "cba_ppt.txt";
	string traffic = "traffic_cba_ppt.txt";
	//string traffic = "default";

	HillClimbing tesla;

	tesla.inputFromFile(destinations, traffic);
	tesla.definingInitState();
	
	bool error = false;

	do
	{
		cout << "Current State: ";
		if (tesla.viewInitState() == false) {
			error = true;
			break;
		}
		tesla.createChildNode();
		tesla.viewChildState(); 
		tesla.increaseStage();

	} while (tesla.checkNextSolution() != 0);


	if (error == false) {
		cout << "Current State: ";
		tesla.viewInitState();
	}

	return 0;
}