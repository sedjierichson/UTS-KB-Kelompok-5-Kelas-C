#include <iostream>
#include <utility>
#include <fstream>
#include <stack>
#include <vector>
#include <string>
#include <time.h>
#include <math.h>

using namespace std;

class HillClimbing {
private:
	pair <int, int> graph[10][10]; //Untuk menyimpan rute jalan
	int dimension; // Besar graph

	vector<char> state; //yang sekarang sedang di eksplorasi dan menjadi induk
	vector<int> cur_state; //ini explored, yang sekarang sedang di eksplorasi dan menjadi induk

	vector<int> best_solution; //Menyimpan Hasil terbaik.

	vector<int> tmp;

	int current_energy; // Objective function value.

	int bestEnergy; // Menyimpan hasil yang paling otpimal disaat itu.

	float temperature, cooling_rate; //Temperature dan cooling rate untuk simulated annealing.

public:

	HillClimbing() {
		dimension = 0;
		current_energy = 0;
		temperature = 9999999999999999999999.0;
		cooling_rate = 0.00001;
		bestEnergy = 0;
	}
	
	void inputFromFile(string namaFile, string traffic = "default") { //Untuk mengambil data dari dalam file kemudian memasukkan kedalam array.
		fstream in_file(namaFile, ios::in); 
		fstream f_traffic(traffic, ios::in);
		if (in_file.is_open()) {
			in_file >> dimension;

			for (int i = 0; i < dimension; i++) {
				for (int j = 0; j < dimension; j++) {

					in_file >> graph[i][j].first;
					if (traffic == "default") {
						graph[i][j].second = 0;
					}
					else if (traffic == "rand") {
						if (graph[i][j].first == 0) { //Graph First menandakan panjang jalan.
							graph[i][j].second = 0;   //Graph Second menandakan tingkat keramaian jalan.
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

		//Inisialisasi best solution
		for (int i = 0; i < dimension; i++) {
			best_solution.push_back(NULL);
		}
		displayCurrentFile();
	}

	//Untuk menampilkan isi dari array
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

	//Untuk menginisialisasi awal(cur_state, current_energy, & bestEnergy)
	void definingInitState() {
		int current = 0; //Melambangkan posisi sekarang.
		for (int i = 0; i < dimension; i++) {
			current = i;

			if (cur_state.empty()) { // 
				cur_state.push_back(current);
			}
			int co = 0;
			for (int j = 0; j < dimension; j++) {

				if (graph[i][j].first != 0 && j != current && !count(cur_state.begin(), cur_state.end(), j)) { //Untuk mengecek apakah ada jalur dan mengecek apakah sudah ter-eksplore atau belum
					cur_state.push_back(j);
					current_energy += graph[i][j].first + graph[i][j].second;
					break;
				}
				co = j;
			}

			if (co == dimension - 1) { // Apabila tidak menemukan jalan dan harus melalui jalur yang sama 2x, maka akan dibreak.
				break;
			}
		}
		bestEnergy = current_energy;
	}

	//Untuk membuat solusi-solusi yang ada.
	void createChildNode() {
		cout << "Dimension :" << dimension << endl;
		int xyz = 0, i = 0, j = 0;

		while (temperature > 1) //Konsep Simulated annealing.
		{
				if (swap(i, j, xyz) == true) {

					if (simulatedAnealing(xyz, tmp) == true) {
						j = 0;
						i = 0;
					}

					for (int q = 0; q < dimension; q++) {
						tmp[q] = cur_state[q];
					}
				}
				xyz = 0;
		
				if (i >= dimension - 1) {
					i = 0;
				}

				if (j >= dimension - 1) {
					j = 0;
					i++;
				}
				j++;
				
				temperature *= 1 - cooling_rate;				
		}
	}

	//Untuk menghitung acceptance probability
	double acceptanceProbability(int energy, int newEnergy, double temperature) {
		// If the new solution is better, accept it
		// Jika solusi yang baru lebih baik, maka akan 
		if (newEnergy < energy) {
			return 1.0;
		}
		// Jika solusi yang baru lebih buruk dari solusi sebelumnya, maka akan dilakukan penghitungan probabilitas yang bisa diterima.
		return exp(double(energy - newEnergy) / temperature);
	}

	bool simulatedAnealing(int neighbourEnergy, vector<int> new_solution) {
		
		//acceptance test

		float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		//cout << r << endl;

		cout << "Current State: ";
		for (int i = 0; i < dimension; i++) {
			cout << char(65 + cur_state[i]) << " ";
		} cout << "\n";

		//Jika nilai acceptanceProbabilitynya lebih besar dari r (nilai random antara 0-1)
		if(acceptanceProbability(current_energy, neighbourEnergy, temperature) > r) {
			for (int i = 0; i < dimension; i++) {
				cur_state[i] = new_solution[i]; //maka solusi yang lebih buruk akan di terima
			}
			current_energy = neighbourEnergy; //dan current_energy akan menjadi neighbourEnergy 
		}

		cout << "Neighbor State: ";
		for (int i = 0; i < dimension; i++) {
			cout << char(65 + new_solution[i]) << " ";
		} cout << "\n";

		cout << "Best Solution / Best State: ";
		for (int i = 0; i < dimension; i++) {
			cout << char(65 + best_solution[i]) << " ";
		}

		cout << "\nNeighbor Energy: " << neighbourEnergy;
		cout << "\nCurrent Energy: " << current_energy;
		cout << "\nBest Energy: " << bestEnergy << endl << "\n";

		//Bila current_energy < bestEnergy
		if (current_energy < bestEnergy) {
			for (int i = 0; i < dimension; i++) {
				best_solution[i] = cur_state[i]; //maka solusi yang ada sekarang dianggap sebagai solusi yang optimal
			}
			bestEnergy = current_energy;
			return true;
		}

		return false;
	}

	bool swap(int i, int j, int& xyz)
	{
		int half = dimension / 2;
		i = rand() % half;
		j = rand() % half + half; //supaya hasil random lebih acak & bervariasi hasilnya

		for (int q = 0; q < dimension; q++) {
			tmp.push_back(cur_state[q]);
		} //meng copy solusi yang ada ke neighbour_temporary

		int temp = tmp[i];
		tmp[i] = tmp[j];
		tmp[j] = temp;

		for (int i = 0; i < dimension - 1; i++) {
			if (graph[tmp[i]][tmp[i + 1]].first == 0) { //cek apakah jalur tersedia atau tidak
				return false;
			}
			xyz += graph[tmp[i]][tmp[i + 1]].first + graph[tmp[i]][tmp[i + 1]].second; //hitung energy nya
		}
		return true;
	}

	//Menampilkan solusi yang sudah ada
	bool viewInitState() {
		for (int q = 0; q < cur_state.size(); q++) {
			state.push_back(char(65 + cur_state[q]));
			cout << state[q] << " ";
		}
		cout << "Val: " << current_energy << "\n";
		if (cur_state.size() != dimension) {
			cout << "\nSolution not found (berarti ada node yang harus dilewati 2x)" << endl;
			return false;
		}

		state.clear();
		return true;
	}

	//Menampilkan solusi yang optimal
	bool viewBestState() {
		for (int q = 0; q < best_solution.size(); q++) {
			cout << char(65 + best_solution[q]) << " ";
		}

		cout << "Val: " << bestEnergy << "\n";

		if (best_solution.size() != dimension) {
			cout << "\nSolution not found (berarti ada node yang harus dilewati 2x)" << endl;
			return false;
		}
		return true;
	}
};

//IMPORTANT VAR FOR SIMULATED ANNEALING
	//current energy
	//neighbor energy
	//best energy
	//current solution
	//neighbor solution
	//best solution

int main() {
	srand(time(NULL));
	string destinations = "cba2.txt";
	string traffic = "traffic_cba2.txt";
	//string traffic = "default";

	HillClimbing GPS;

	GPS.inputFromFile(destinations, traffic);
	GPS.definingInitState();

	bool error = false;

	int currentEnergy, neighborEnergy; //neighbor = newEnergy

	cout << "Current State: ";
	if (GPS.viewInitState() == false) {
		error = true;
	}


	if (error == false) {
		GPS.createChildNode();

		cout << "Best State: ";
		GPS.viewBestState();
	}

	return 0;
}