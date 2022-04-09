#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include "strutils.h"
// Mehmet Faruk Komurculer

using namespace std;
class Node
{
	public:
		string txtChars;
		Node *diger;
};
class Queue
{
    Node *front;
    Node *back;
    Node *prev;
    Node *temp;
    bool isEmpty()
	{
        return front == NULL;
    }
public:
	Queue()
	{
        front = NULL;
        back = NULL;
    }
	void dequeue()
	{
        temp = front;
        front = front -> diger;
        delete temp;
    }
    void enqueue(string x)
    {
        temp = new Node;
        temp -> txtChars = x;
        temp -> diger = NULL;
        if(isEmpty())
		{
            front = temp;
            back = temp;
        }
        else
		{
            prev = back;
            back -> diger = temp;
            back = temp;
        }
    }
    
	string displayFirst()
	{
		if (!isEmpty())
		{
			return front -> txtChars;
		}
	}
	bool isEmpty2()
	{
		return front == NULL;
	}
};
int main (){
	string folderName;
	cout << "Please enter the process folder name: " ;
	cin >> folderName;
	cout << "When all processes are completed, you can find execution sequence in" <<  endl;
	cout << "\"" << folderName << "/output.txt\"." << endl;
	//reading configuration txt in the given folder
	ifstream in(folderName + "/configuration.txt");
	// defining integer variables
	int numberOfQueues, numberOfProcesses, size_S;
	//creating a vector that includes defined integer variables
	vector<int> config;
	int config_ints;
	while (in >> config_ints){
		config.push_back(config_ints);
	}
	//0th element of the vector = number of Queues to be created
	numberOfQueues = config[0];
	//1st element of the vector = number of process files to be processed
	numberOfProcesses = config[1];
	//2nd element of the vector = time slice
	size_S = config[2];
	ofstream outputFile;
	outputFile.open (folderName + "/output.txt");
	// creating multiple vectors
	vector<vector<string>> processVectors(numberOfProcesses);
	// creating multiple queues for reading the inputs easily. 
	vector<Queue> queues(numberOfQueues + numberOfProcesses);
	//creating priority queues 
	vector <queue<int>> realQueues(numberOfQueues);
	//reading process files
	for (int i = 0; i < numberOfProcesses; i++)
	{
		string fileName = "p" + itoa(i+1);
		ifstream in2(folderName + "/" + fileName + ".txt");
		string fileInts;
		int count = 0;
		while (in2 >> fileInts)
		{
			processVectors[i].push_back(fileInts);
		}
	}
	// pushing 1, ... , N in to the topmost queue. 
	for (int i = 0; i < numberOfProcesses; i++)
	{
		realQueues[numberOfQueues-1].push(i+1);
	}
	// removing last 2 lements from the vectors of process files and adding - to be able to read execution step of the process.
	for (int i = 0; i < numberOfProcesses; i++)
	{
		processVectors[i].pop_back();
		processVectors[i].pop_back();
		processVectors[i].push_back("-");
	}
	// to read process infos easily, pushing elements into the queues different than priority queues
	for (int i = 0; i < numberOfProcesses; i++)
	{
		for (int j = 0; j < processVectors[i].size(); j++)
		{
			queues[numberOfQueues+i].enqueue(processVectors[i][j]);
		}
	}
	// reading priority queues starting from topmost queue, and changing 
	//the priorities of content of the queues wrto the process files. 
	int size_Scounter = 0;
	while (size_Scounter < size_S)
	{
		int k = numberOfQueues-1;
		while (k > -1)
		{
			if (!realQueues[k].empty())
			{
				if (!queues[numberOfQueues -1 + realQueues[k].front()].isEmpty2())
				{
					if (queues[numberOfQueues -1 + realQueues[k].front()].displayFirst() == "1")
					{
						if (k-1 != -1)
						{
							queues[numberOfQueues -1 + realQueues[k].front()].dequeue();
							outputFile  << "1, PC" << realQueues[k].front() << ", Q" << k << endl;
							realQueues[k-1].push(realQueues[k].front());
							realQueues[k].pop();
							size_Scounter++;								
							if (size_Scounter == size_S)
							{
								int ab = numberOfQueues-2;
								while (ab != -1)
								{
									if (!realQueues[ab].empty())
									{
										while (!realQueues[ab].empty())
										{
											outputFile  << "B, PC" << realQueues[ab].front() << ", Q" << numberOfQueues << endl;
											realQueues[numberOfQueues-1].push(realQueues[ab].front());
											realQueues[ab].pop();
										}
										ab--;
									}
									else
									{
										ab--;
									}		
								}
								size_Scounter = 0;
							}
						}
						else if (k-1 == -1)
						{
							queues[numberOfQueues -1 + realQueues[k].front()].dequeue();
							outputFile  << "1, PC" << realQueues[k].front() << ", Q" << k+1 << endl;
							realQueues[k].push(realQueues[k].front());
							realQueues[k].pop();
							size_Scounter++;
							if (size_Scounter == size_S)
							{
								int ab = numberOfQueues-2;
								while (ab != -1)
								{
									if (!realQueues[ab].empty())
									{
										while (!realQueues[ab].empty())
										{
											outputFile  << "B, PC" << realQueues[ab].front() << ", Q" << numberOfQueues << endl;
											realQueues[numberOfQueues-1].push(realQueues[ab].front());
											realQueues[ab].pop();
										}
										ab--;
									}
									else
									{
										ab--;
									}		
								}
								size_Scounter = 0;
							}
						}
					}
					else if (queues[numberOfQueues -1 + realQueues[k].front()].displayFirst() == "0")
					{
						queues[numberOfQueues -1 + realQueues[k].front()].dequeue();
						outputFile  << "0, PC" << realQueues[k].front() << ", Q" << k+1 << endl;
						realQueues[k].push(realQueues[k].front());
						realQueues[k].pop();
						size_Scounter++;
						if (size_Scounter == size_S)
						{
							int ab = numberOfQueues-2;
							while (ab != -1)
							{
								if (!realQueues[ab].empty())
								{
									while (!realQueues[ab].empty())
									{
										outputFile  << "B, PC" << realQueues[ab].front() << ", Q" << numberOfQueues << endl;
										realQueues[numberOfQueues-1].push(realQueues[ab].front());
										realQueues[ab].pop();
									}
									ab--;
								}
								else
								{
									ab--;
								}			
							}
							size_Scounter = 0;
						}
					}
					else if (queues[numberOfQueues -1 + realQueues[k].front()].displayFirst() == "-")
					{
						outputFile  << "E, PC" << realQueues[k].front() << ", QX" << endl;
						realQueues[k].pop();
						size_Scounter++;
						if (size_Scounter == size_S)
						{
							int ab = numberOfQueues-2;
							while (ab != -1)
							{
								if (!realQueues[ab].empty())
								{
									while (!realQueues[ab].empty())
									{
										outputFile  << "B, PC" << realQueues[ab].front() << ", Q" << numberOfQueues << endl;
										realQueues[numberOfQueues-1].push(realQueues[ab].front());
										realQueues[ab].pop();
									}
									ab--;
								}
								else
								{
									ab--;
								}		
							}
							size_Scounter = 0;
						}
					}
				}
				else
				{
					k--;
				}
			}
			else
			{
				k--;
			}
		}
	}
	outputFile.close();
	return 0;
}													
