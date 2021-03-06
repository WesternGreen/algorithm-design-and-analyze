#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

//========================== use this to control output
#define SITUATION_22
#define SINK_STATION_EXIST
//======================================================

#ifdef SITUATION_22
#define FILE_NAME "matrix22.txt"
#define STATION_NUM 22
#else
#define FILE_NAME "matrix42.txt"
#define STATION_NUM 42
#endif//SITUATION_22

void f(double dis[][STATION_NUM], int stationCode[], int sourceCode, int sinkCode = -1);

int main(){
	//--------------------------------------- declaration
	int stationCode[STATION_NUM];
	double dis[STATION_NUM][STATION_NUM];

	//------------------------------------------ input
	ifstream fin;
	fin.open(FILE_NAME);
	for (int i = 0; i < STATION_NUM; ++i){
		fin >> stationCode[i];
	}
	for (int i = 0; i < STATION_NUM; ++i){
		fin >> dis[i][i];//read station code
		for (int j = 0; j < STATION_NUM; ++j){
			fin >> dis[i][j];
		}
	}
	fin.close();

	//------------------------------------ output for debug
	// for (int i = 0; i < STATION_NUM; ++i){
	// 	cout << "station code: " << stationCode[i] << endl;
	// 	for (int j = 0; j < STATION_NUM; ++j){
	// 		cout << "\t" << dis[i][j] << endl;
	// 	}
	// }

#ifdef SITUATION_22
#ifndef SINK_STATION_EXIST
	f(dis, stationCode, 567443);
#else //define SINK_STATION_EXIST
	f(dis, stationCode, 567443, 33109);
#endif //SINK_STATION_EXIST
#else //not define SITUATION_22
#ifndef SINK_STATION_EXIST
	f(dis, stationCode, 565845);
#else //define SINK_STATION_EXIST
	f(dis, stationCode, 565845, 565667);
#endif //SINK_STATION_EXIST
#endif //SITUATION_22

	system("pause");
	return 0;
}

//! if sinkCode == -1 then calculate all path
void f(double dis[][STATION_NUM], int stationCode[], int sourceCode, int sinkCode){
	//--------------------------------------- get sourceIndex and sinkIndex
	int sourceIndex = -1;
	int sinkIndex = -1;
	for (int i = 0; i < STATION_NUM; ++i){
		if (stationCode[i] == sourceCode){
			sourceIndex = i;
			break;
		}
	}
	if (sinkCode != -1){
		for (int i = 0; i < STATION_NUM; ++i){
			if (stationCode[i] == sinkCode){
				sinkIndex = i;
				break;
			}
		}
	}

	if (sourceIndex == -1) return;//error

	bool set[STATION_NUM];//if station[i] is in the set, set[i] = true
	for (int i = 0; i < STATION_NUM; ++i){
		set[i] = false;
	}
	set[sourceIndex] = true;
	bool flag = true;//if set[i] = true for all i, flag = false
	vector<int> path[STATION_NUM];
	
	while (flag){
		//------------------ add one more station in set
		//get closest station which is not in set
		int closestIndex = -1;
		double closestDis = -1;
		int from = -1;
		for (int i = 0; i < STATION_NUM; ++i){
			if (set[i]){//middle station
				for (int j = 0; j < STATION_NUM; ++j){
					if (!set[j] && dis[i][j] != -1){//new point
						if (closestDis == -1 || closestDis > dis[i][j]){
							closestDis = dis[i][j];
							closestIndex = j;
							from = i;
						}
					}
				}
			}
		}

		//=================================== now we get the new point

		set[closestIndex] = true;//put closest station in set
		dis[sourceIndex][closestIndex] = dis[sourceIndex][from] + dis[from][closestIndex];//refresh distance
		path[closestIndex] = path[from];
		path[closestIndex].push_back(from);

		//judge flag
		flag = false;
		for (int i = 0; i < STATION_NUM; ++i){
			if (!set[i]){
				flag = true;
				break;
			}
		}

		//one-to-one situation, break loop
		if (sinkIndex != -1){
			if (set[sinkIndex]){
				flag = false;
			}
		}
	}

	//------------------------------- output
	ofstream fout;
#ifdef SITUATION_22
#ifndef SINK_STATION_EXIST
	fout.open("output1.txt");
#else //define SINK_STATION_EXIST
	fout.open("output2.txt");
#endif //SINK_STATION_EXIST
#else //not define SITUATION_22
#ifndef SINK_STATION_EXIST
	fout.open("output3.txt");
#else //define SINK_STATION_EXIST
	fout.open("output4.txt");
#endif //SINK_STATION_EXIST
#endif //SITUATION_22
	if (sinkIndex == -1){
		cout << "All path from " << sourceCode << ":\n";
		fout << "All path from " << sourceCode << ":\n";
		for (int i = 0; i < STATION_NUM; ++i){
			if (i != sourceIndex){
				cout << "To " << stationCode[i] << ": " << dis[sourceIndex][i] << endl;
				fout << "To " << stationCode[i] << ": " << dis[sourceIndex][i] << endl;
				cout << "\tPath:\n";
				fout << "\tPath:\n";
				for (int j = 0; j < path[i].size(); ++j){
					cout << "\t" << stationCode[path[i][j]] << endl;
					fout << "\t" << stationCode[path[i][j]] << endl;
				}
			}
		}
	} else {
		cout << "Distance from " << sourceCode << " to " << sinkCode << " is ";
		fout << "Distance from " << sourceCode << " to " << sinkCode << " is ";
		cout << dis[sourceIndex][sinkIndex] << endl;
		fout << dis[sourceIndex][sinkIndex] << endl;
		cout << "\tPath:\n";
		fout << "\tPath:\n";
		for (int i = 0; i < path[sinkIndex].size(); ++i){
			cout << "\t" << stationCode[path[sinkIndex][i]] << endl;
			fout << "\t" << stationCode[path[sinkIndex][i]] << endl;
		}
	}
}
