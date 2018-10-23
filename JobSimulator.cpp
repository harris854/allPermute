#include <iostream>
#include <utility>
#include <string.h>
#include <vector>
#include "JobSimulator.h"


using namespace std;

JobSimulator::JobSimulator() {
	totalJobs = 10;//jobs 
	totalMachines = 8;//machines
}

JobSimulator::JobSimulator(int tJobs, int tMachines, int* jOrder, int **jMatrix)
	//:jobOrder { std::move(jOrder) }
{
	totalJobs = tJobs;
	totalMachines = tMachines;
	jobOrder = jOrder;
	originalMatrix = jMatrix;
}

int JobSimulator::simulate()
{
	int prow, pcol;
	int cmax;

	int **jobs = new int*[totalMachines];
	for (int a = 0;a < totalMachines;++a) {
		jobs[a] = new int[totalJobs];
	}

	for (int r = 0;r < totalMachines;++r) {
		if (r == 0) {
			for (int c = 0; c < totalJobs; c++) {
				if (c != 0)
					jobs[r][c] = jobs[r][c - 1] + originalMatrix[r][jobOrder[c]];
				else
					jobs[r][c] = originalMatrix[r][jobOrder[c]];
			}
		}
		else {
			for (int c = 0; c < totalJobs; c++) {
				if (c == 0) {
					jobs[r][c] = jobs[r - 1][c] + originalMatrix[r][jobOrder[c]];
				}
				else {
					prow = jobs[r - 1][c];
					pcol = jobs[r][c - 1];
					jobs[r][c] = (prow > pcol ? prow : pcol) + originalMatrix[r][jobOrder[c]];
				}
			}
		}
	}
	cmax = jobs[totalMachines - 1][totalJobs - 1];
	if (jobs) {
		for (int a = 0;a < totalMachines;++a) {
			delete[] jobs[a];
			jobs[a] = nullptr;
		}
		delete[] jobs;
		jobs = nullptr;
	}

	return cmax;
}

int JobSimulator::getMatrixValue(int *matrix, int row, int col)
{
	return matrix[col + row * totalJobs];
}

void JobSimulator::setMatrixValue(int *matrix, int row, int col, int val)
{
	matrix[col + row * totalJobs] = val;
}

int JobSimulator::getNextJob(int *currentJobOrder, int currentJob) {

	for (int i = 0; i < totalJobs; ++i) {
		if (currentJobOrder[i] == currentJob) {
			if (i + 1 == totalJobs) {
				return -1;
			}
			return currentJobOrder[i + 1];
		}
	}
	return -1;
}


JobSimulator::~JobSimulator()
{

}




