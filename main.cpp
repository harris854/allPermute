#include<stdio.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string.h>
#include<thread>
#include<vector>
#include<mutex>
#include<algorithm>
#include <climits>
#include <chrono>
#include "JobSimulator.h"


using namespace std;


mutex myMutex1;
mutex myMutex2;
int  totalJobs = 10;//jobs
int  totalMachines = 8;//machines


vector<int>jobOrder;//job sequence
int minCmax = 0;
vector<vector<int>> minCmaxList;

int kvalue = 20000;

int **originalMatrix;

void saveResult(int* joborder, int cMax) {

	lock_guard<mutex> guard(myMutex2);
	vector<int> jobBetter;

	if (minCmax != 0) {
		//got new cMax
		if (minCmax > cMax) {
			for (int i = 0;i < totalJobs;++i) {
				jobBetter.push_back(joborder[i]);
			}
			minCmax = cMax;
			//for_each(minCmaxList.begin(), minCmaxList.end(), op_delete_array<int*>);
			minCmaxList.clear();
			/*cout << "This theread is " << std::this_thread::get_id() << " "  << "JobOrder: ";
			for (int i = 0; i < totalJobs; ++i) {
			cout<<jobOrder[i] <<" ";
			}
			cout << endl;*/

			minCmaxList.push_back(jobBetter);
		}
		//if same cmax came
		else if (minCmax == cMax) {
			/*cout << "This theread is " << std::this_thread::get_id() << " " << "JobOrder: ";
			for (int i = 0; i < totalJobs; ++i) {
				cout << jobOrder[i] << " ";
			}
			cout << endl;*/
			for (int i = 0;i < totalJobs;++i) {
				jobBetter.push_back(joborder[i]);
			}
			minCmaxList.push_back(jobBetter);
		}
		//if cmax is high
		else {
			//do nothing
		}
	}
	else {
		for (int i = 0;i < totalJobs;++i) {
			jobBetter.push_back(joborder[i]);
		}
		minCmax = cMax;
		minCmaxList.push_back(jobBetter);
	}

}

bool nextPermutation(int* vec) {
	lock_guard<mutex> guard(myMutex1);

	// Find non-increasing suffix
	if (jobOrder.size() == 0)
		return false;
	std::vector<int>::iterator i = jobOrder.end() - 1;
	while (i > jobOrder.begin() && *(i - 1) >= *i)
		--i;
	if (i == jobOrder.begin()) {

		return false;
	}

	// Find successor to pivot
	std::vector<int>::iterator j = jobOrder.end() - 1;
	while (*j <= *(i - 1))
		--j;
	std::iter_swap(i - 1, j);

	// Reverse suffix
	std::reverse(i, jobOrder.end());

	for (int i = 0;i < totalJobs;++i) {
		vec[i]=jobOrder[i];
	}
	return true;
}

void runJobOnPermutation() {
	int *myJobOrder1 = new int[totalJobs];//job sequence

	for (int i = 0;i < totalJobs;++i) {
		myJobOrder1[i]=jobOrder[i];
	}
	JobSimulator js(totalJobs, totalMachines, myJobOrder1, originalMatrix);
	saveResult(js.jobOrder, js.simulate());
	js.~JobSimulator();
	while (nextPermutation(myJobOrder1)) {

		JobSimulator js(totalJobs, totalMachines, myJobOrder1, originalMatrix);
		saveResult(js.jobOrder, js.simulate());
		js.~JobSimulator();
	}

	//vector<int> myJobOrder;//job sequence
	//for (int i = 0;i < jobOrder.size();++i) {
	//	myJobOrder.push_back(jobOrder[i]);
	//}
	//JobSimulator js(totalJobs, totalMachines, myJobOrder, originalMatrix);
	//saveResult(js.jobOrder, js.simulate());
	//js.~JobSimulator();
	//myJobOrder.clear();
	//while (nextPermutation(myJobOrder)) {

	//	JobSimulator js(totalJobs, totalMachines, myJobOrder, originalMatrix);
	//	saveResult(js.jobOrder, js.simulate());
	//	js.~JobSimulator();
	//	myJobOrder.clear();
	//}

	if (myJobOrder1) {
		delete myJobOrder1;
		myJobOrder1 = nullptr;
	}
}
	

template<class T>
T* op_join(T* t) {
	t->join();
	return t;
}

template<class T>
void op_delete(T* t) {
	if (t) {
		delete t;
		t = nullptr;
	}
}

int main() {


	unsigned int hwthd = thread::hardware_concurrency();
	vector<thread*> threadsList;
	unsigned int threadCount = hwthd;


	//if ((dir = opendir("/home/sapkota/Thesis/Data/Smalll")) != NULL) {
	//if ((dir = opendir("D:/Thesis/Data/Smalll/")) != NULL) {

	//	while ((ent = readdir(dir)) != NULL) {
	string name = "test.txt";

	/*if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0)
	{*/
	//	cout << name.c_str() << endl;

		//string file = name.substr(0, name.find_last_of("."));
		string filename = "/home/sapkota/Thesis/Data/Small/" + name;
		string result = "/home/sapkota/Thesis/Data/Results/result_a" + name;
	//string filename = "D://Thesis//Data//Small//" + name;
	//string result = "D://Thesis//Data//Results//result_a" + name;

	ofstream myfile(result);



	int x;
	int y;
	////D:\Thesis\Data\Small
	ifstream f(filename);
	if (!f) {
		cout << "Cannot find the file";
	}
	f >> totalJobs >> totalMachines;

	//
	originalMatrix = new int*[totalMachines];
	for (int a = 0;a < totalMachines;++a) {
		originalMatrix[a] = new int[totalJobs];
	}


	for (int j = 0; j < totalJobs; j++)
	{
		for (int i = 0; i < totalMachines; i++)
		{
			f >> x;
			f >> y;
			originalMatrix[i][j] = y;
		}
	}
	f.close();
	//job order 0 1 2 3 4 5 6 7 8 9 like this
	for (int i = 0; i < totalJobs; ++i) {
		jobOrder.push_back(i);
	}
	//
	myfile << "Result summary file.\n";
	myfile << "File: " << name.c_str() << " \n";
	myfile << "Gernerator: Hari Prasad Sapkota \n\n";

	//print jobs schedule matrix
	myfile << "#######################################################################################" << endl;
	myfile << "The Jobs Matrix is as: " << endl;
	for (int j = 0; j < totalJobs; ++j) {
		if (j == 0) myfile << setw(6) << " ";
		myfile << setw(4) << "J" << j + 1;
	}
	myfile << endl;
	for (int i = 0; i < totalMachines; ++i) {
		//jobOrder = getMatrixRow(jobsOriginal, i);
		myfile << setw(3) << "M" << i + 1 << " : ";
		for (int j = 0; j < totalJobs; ++j)
			myfile << setw(4) << originalMatrix[i][j] << ' ';
		//myfile << setw(4) << jobOrder[j] << ' ';
		myfile << endl;
	}
	myfile << endl << "#######################################################################################" << endl;
	std::cout << "Job matrix printed.";

	// get start timer...
	auto t1 = chrono::high_resolution_clock::now();
	//calling the threads
	for (unsigned int N = 0; N < threadCount; ++N) {
		threadsList.push_back(new thread(runJobOnPermutation));
	}

	transform(threadsList.cbegin(), threadsList.cend(), threadsList.begin(), op_join<std::thread>);

	for_each(threadsList.begin(), threadsList.end(), op_delete<std::thread>);
	threadsList.clear();
	// get end time...
	auto t2 = chrono::high_resolution_clock::now();


	int bestCount = 0;
	myfile << endl << "***************************************************************************************" << endl;
	myfile << "Result for Minimum Cmax Value " << minCmax << endl;
	myfile << "***************************************************************************************" << endl;
	for (unsigned int i = 0; i < minCmaxList.size(); i++) {
		++bestCount;
		//	myfile << "Cmax: " << setw(2) << cmax[i] << " => Job Order: ";
		jobOrder = minCmaxList[i];
		for (int j = 0; j < totalJobs; ++j)
			myfile << jobOrder[j] + 1 << ' ';
		myfile << '\n';
	}
	myfile << endl << "Result count for optimal value: " << bestCount << endl;


	myfile << "***************************************************************************************" << endl;
	myfile << hwthd << " Threads took :" << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " milliseconds" << endl;
	myfile << "***************************************************************************************" << endl;
	
	
	if(originalMatrix){
	for (int a = 0;a < totalMachines;++a) {
		delete[] originalMatrix[a];
		originalMatrix[a]= nullptr;
		
	}

		delete[] originalMatrix;
		originalMatrix= nullptr;
	}
	
	return 0;
}