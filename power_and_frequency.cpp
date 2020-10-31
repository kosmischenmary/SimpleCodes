#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>

using namespace std;

inline float rapl(string path){
	float f_var;
	ifstream file(path);
	file >> f_var;
	file.close();
	return f_var;
}

int main (int argc, char *argv[]){
	int control;
	if(argc == 1){
		cout << "Runtime in seconds:";
		cin >> control;
	}else if(argc >= 2){
		control = stoi(argv[1]);
	}
	//Paths to rapl's energy files
	//They must change depending on the architecture
	string path_core = "/sys/class/powercap/intel-rapl:0:0/energy_uj";
	string path_uncore = "/sys/class/powercap/intel-rapl:0:1/energy_uj";
	string path_dram = "/sys/class/powercap/intel-rapl:0:2/energy_uj";
	string path_pack = "/sys/class/powercap/intel-rapl:0/energy_uj";
	string path_cpuinfo = "/proc/cpuinfo";
	float core_0, core_1, uncore_0, uncore_1, dram_0, dram_1, pack_0, pack_1; float a;
	string aux, aux2;
	ofstream file("result.txt");
	file << "CORE0;CORE1;CORE2;CORE3;CORE4;CORE5;CORE6;CORE7;CPU;CORE;UNCORE;DRAM" << endl;
	system("clear");
	for(int i=-1;i<control;i++){
		if(i==-1){
			core_1 = rapl(path_core);
			uncore_1 = rapl(path_uncore);
			dram_1 = rapl(path_dram);
			pack_1 = rapl(path_pack);
			system("sleep 1");
		}else{
			core_0 = core_1;
			core_1 = rapl(path_core);
			uncore_0 = uncore_1;
			uncore_1 = rapl(path_uncore);
			dram_0 = dram_1;
			dram_1 = rapl(path_dram);
			pack_0 = pack_1;
			pack_1 = rapl(path_pack);
		
			ifstream file_cpuinfo (path_cpuinfo);
			while(getline (file_cpuinfo,aux)){
				if(aux.find("MHz") != string::npos){
					istringstream iss (aux);
					
					for(int i=0;i<3;i++){
						if(i==1){
							iss >> aux2;
							aux2 = "GHz";
							cout << aux2 << "\t";
						}
						if(i==2){ 
							iss >> a;
							cout << fixed << a/1000 << "\t";
							file << fixed << a/1000 << ";";
						}else{
							iss >> aux2;
							cout << aux2 << "\t";
						}
					}
					cout << endl;
				}
			}
			file << (pack_1 - pack_0)/1000000 << ";" << (core_1 - core_0)/1000000 << ";" << (uncore_1 - uncore_0)/1000000 << ";" << (dram_1 - dram_0)/1000000 << endl;

			cout << "---------" << endl;
			cout << "CPU: " << fixed << (pack_1 - pack_0)/1000000 << endl;
			cout << "CORE: " << fixed << (core_1 - core_0)/1000000 << endl;
			cout << "UNCORE: " << fixed << (uncore_1 - uncore_0)/1000000 << endl;
			cout << "DRAM: " << fixed << (dram_1 - dram_0)/1000000 << endl;
			cout << "---------" << endl;
			system("sleep 1"); system("clear");
		}
	}
	file.close();
	return 0;
}
