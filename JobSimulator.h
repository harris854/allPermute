#pragma once

class JobSimulator
{
public:
	JobSimulator(int totalJobs, int totalMachines, int* jOrder, int ** jobMatrix);
	JobSimulator();

	int simulate();
	~JobSimulator();

	int getMatrixValue(int *matrix, int row, int col);
	void setMatrixValue(int *matrix, int row, int col, int val);
	int getNextJob(int *currentJobOrder, int currentJob);
	int* jobOrder;
private:
	int  totalJobs;
	int  totalMachines;

	int *jobs;
	int *jobStatus;
	int *currentMJob;

	int **originalMatrix;
// = new  int[totalJobs];//job sequence
	int kValue;
};

