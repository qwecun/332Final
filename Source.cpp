#include <iostream>
#include <string>
#include <queue>
#include<fstream>
#include<cstring>


using namespace std;

struct job {
	int name;
	int serviceTime;
	int arrivalTime;
	int startTime;
	int endTime;
	bool status;
	bool arrivalC;//arrival control  defeult=false;
	int tatime;
        int waitTime;
};
queue <job> myJob;
void FCFS(bool detail, job jobs[], int numOfJobs) {
	double timeNow = 0;
	double  totalTatime = 0;
	double  totalWait = 0;
        double totalBusyTime = 0;
	for (int i = 0; i < numOfJobs; i++) {
		if (timeNow >= jobs[i].arrivalTime) {
			jobs[i].startTime = timeNow;
			timeNow = jobs[i].startTime + jobs[i].serviceTime;
			jobs[i].endTime = timeNow;
                        timeNow++;//cs
		}
		else {
			timeNow = jobs[i].arrivalTime;
			jobs[i].startTime = timeNow;
			timeNow = jobs[i].startTime + jobs[i].serviceTime;
			jobs[i].endTime = timeNow;
                        timeNow++;//cs
		}
                totalBusyTime = jobs[i].serviceTime+totalBusyTime;
		jobs[i].tatime = jobs[i].endTime - jobs[i].arrivalTime;
		jobs[i].waitTime = jobs[i].startTime - jobs[i].arrivalTime;
		totalTatime = jobs[i].tatime + totalTatime;
		totalWait = jobs[i].waitTime + totalWait;
	}
	cout << "First Come First Serve" << endl << endl;
	cout << "Total Time required is " << timeNow-1 << endl;
	cout << "CPU Utilization is " << totalBusyTime/(timeNow-1)*100 << "%\n";
	cout << "Average turnaround time is " << totalTatime / numOfJobs << " \n";
	cout << "Average waitting time is " << totalWait / numOfJobs << " \n\n";
	if (detail) {
		for (int j = 0; j < numOfJobs; j++) {
			cout << "Process " << jobs[j].name << endl;
			cout << "Arrival time:" << jobs[j].arrivalTime << endl;
			cout << "Start time:" << jobs[j].startTime << endl;
			cout << "Waitting time:" << jobs[j].waitTime << endl;
			cout << "Service time:" << jobs[j].serviceTime << endl;
			cout << "Turnaround time:" << jobs[j].tatime << endl;
			cout << "Finish time:" << jobs[j].endTime << endl << endl;
		}
	}

}


//round robin
void RR(bool detail, job jobs[], int numOfJobs){
	double timeNow = 0; 
	int floor = 0;
	int timeQ = 10;
	double  totalTatime = 0;
        double  totalWait = 0;
        double totalBusyTime = 0;
	do{
		//read data into queue if job arrive
		for (int i = floor; i < numOfJobs; i++){
			if (jobs[i].arrivalTime <= timeNow && jobs[i].arrivalC == false){
				jobs[i].arrivalC = true;
				myJob.push(jobs[i]);
			}
			else if (jobs[i].arrivalTime>timeNow){
				floor = i;
				break;
			}
		}
				//Runing Job
		timeNow = timeNow + timeQ;
                //Job done
		if (myJob.front().serviceTime <= timeQ){ 
                        if(myJob.front().status == true){//first time run the job
			   jobs[myJob.front().name].startTime = timeNow - timeQ;  //get start time
			   timeNow = timeNow - timeQ +myJob.front().serviceTime;// update present time
			   jobs[myJob.front().name].endTime = timeNow; // finish time
			   jobs[myJob.front().name].tatime = jobs[myJob.front().name].endTime - jobs[myJob.front().name].arrivalTime;// calculate turn around time
			   jobs[myJob.front().name].status = false;
                           timeNow++;//cs
			   myJob.pop();
                        }
		        else  {// not first time 
			   timeNow = myJob.front().serviceTime + timeNow - timeQ; // update present time
			   jobs[myJob.front().name].endTime = timeNow;// finish time
			   jobs[myJob.front().name].tatime = jobs[myJob.front().name].endTime - jobs[myJob.front().name].arrivalTime;// calculate turn around time
			   myJob.pop();
                           timeNow++;//cs
						}
		}
		else //job not done
		{
                        if(jobs[myJob.front().name].status == true){ //only allow updating start time once
                           jobs[myJob.front().name].startTime = timeNow -timeQ;
                           jobs[myJob.front().name].status = false;
                        }
			job temp = myJob.front();
			temp.serviceTime = temp.serviceTime - timeQ;
			myJob.pop();
			myJob.push(temp);
                        timeNow++;//cs
		}
	} while (!myJob.empty());
	cout << "Round-robin scheduling"<< endl << endl;
	cout << "Total Time required is " << timeNow-1<< endl;
	for (int i = 0; i < numOfJobs; i++){
                totalBusyTime = totalBusyTime +jobs[i].serviceTime;
                totalTatime = totalTatime+jobs[i].tatime;
		jobs[i].waitTime = jobs[i].startTime - jobs[i].arrivalTime;
                totalWait = jobs[i].waitTime+ totalWait;
	}
	cout << "CPU Utilization is " << totalBusyTime/(timeNow-1)*100 << "%\n";
    cout<< "Average turnaround time is "<<totalTatime/numOfJobs<<" \n";
    cout<< "Average waitting time is "<<totalWait/numOfJobs<<" \n\n";     
	if (detail){
		for (int j = 0; j < numOfJobs; j++){
			cout << "Process " << jobs[j].name << endl;
			cout << "Arrival time:" << jobs[j].arrivalTime << endl;
            cout << "Start time:" << jobs[j].startTime<< endl;
            cout << "Waitting time:"<<jobs[j].waitTime<< endl;
			cout << "Service time:" << jobs[j].serviceTime << endl;
			cout << "Turnaround time:" << jobs[j].tatime << endl;
			cout << "Finish time:" << jobs[j].endTime<< endl << endl;
		}
	}	
}
//SJF
void SJF(bool detail, job jobs[], int numOfJobs) {
	double timeNow = 0;
	double totalTatime = 0;
	double totalWait = 0;
	double totalBusyTime = 0;
	int floor = 0;
	int shortestIndex = 0;
	int shortestSer;
	for (int x = 0; x < numOfJobs; x++) {
		//check jobs isArrival
		for (int j = floor; j < numOfJobs; j++) {
			if (jobs[j].arrivalTime <= timeNow &&jobs[j].arrivalC == false) {
				jobs[j].arrivalC = true;
				floor = j;
			}
			else {
				floor = j;
				break;
			}
		}
		// check shortest arrival job 
		shortestSer = 999;
		for (int j = 0; j < numOfJobs; j++) {
			if (jobs[j].serviceTime < shortestSer&&jobs[j].status == true && jobs[j].arrivalC == true) {
				shortestSer = jobs[j].serviceTime;
				shortestIndex = j;
			}
		}
		//update information
		jobs[shortestIndex].status = false;
		jobs[shortestIndex].startTime = timeNow;
		timeNow = jobs[shortestIndex].startTime + jobs[shortestIndex].serviceTime;
		jobs[shortestIndex].endTime = timeNow;
		jobs[shortestIndex].waitTime = jobs[shortestIndex].startTime - jobs[shortestIndex].arrivalTime;
		jobs[shortestIndex].tatime = jobs[shortestIndex].endTime - jobs[shortestIndex].arrivalTime;

		totalWait = jobs[shortestIndex].waitTime + totalWait;
		totalTatime = jobs[shortestIndex].tatime + totalTatime;

		totalBusyTime = jobs[shortestIndex].serviceTime +totalBusyTime;
		timeNow++;
	}
	cout << "Shortest Job First " << endl << endl;
	cout << "Total Time required is " << timeNow-1 << endl;
	cout << "CPU Utilization is " << totalBusyTime/(timeNow-1)*100<< "%\n";
	cout << "Average turnaround time is " << totalTatime / numOfJobs << " \n";
	cout << "Average waitting time is " << totalWait / numOfJobs << " \n\n";
	if (detail) {
		for (int j = 0; j < numOfJobs; j++) {
			cout << "Process " << jobs[j].name << endl;
			cout << "Arrival time:" << jobs[j].arrivalTime << endl;
			cout << "Start time:" << jobs[j].startTime << endl;
			cout << "Waitting time:" << jobs[j].waitTime << endl;
			cout << "Service time:" << jobs[j].serviceTime << endl;
			cout << "Turnaround time:" << jobs[j].tatime << endl;
			cout << "Finish time:" << jobs[j].endTime << endl << endl;
		}
	}
}
int main(int argc, char ** argv) {
        
        std::string arg_1 = argv[1];
        std::string arg_2 = argv[2];
        std::string arg_3 = argv[3];
        
        bool detail;
	int totalJobs = 0, input, totalTime = 0;
	job *jobs, *temp;


	string line;

          ifstream inFile((arg_3+".txt").c_str());
  
        if(arg_2 == "-d"){
           detail = true;
        }
        else{detail = false;}
        if (!inFile) {
           cerr << "Unable to open file datafile.txt";
           return(0);  // call system to stop
        }



	while (getline(inFile, line))//size of jobs
		totalJobs++;

	

	inFile.clear();  // reset pointer
	inFile.seekg(0);

	jobs = new job[totalJobs];

	for (int i = 0; i < totalJobs; i++) {
		jobs[i].name = i;
		jobs[i].startTime = 0;
		jobs[i].endTime = 0;
		jobs[i].status = true;
		jobs[i].arrivalC = false;
		inFile >> jobs[i].arrivalTime >> jobs[i].serviceTime;
	}

	


	temp = new job[totalJobs];
	for (int i = 0; i < totalJobs; i++) {
		temp[i].name = jobs[i].name;
		temp[i].startTime = jobs[i].startTime;
		temp[i].endTime = jobs[i].endTime;
		temp[i].status = jobs[i].status;
		temp[i].arrivalC = jobs[i].arrivalC;
		temp[i].arrivalTime = jobs[i].arrivalTime;
		temp[i].serviceTime = jobs[i].serviceTime;
	}
        if(arg_1=="-f")
        FCFS(detail,jobs, totalJobs);
        else if(arg_1=="-r")
        RR(detail, jobs, totalJobs);
        else if(arg_1=="-s")
        SJF(detail, jobs, totalJobs);
        else if(arg_1=="-a"){
        FCFS(detail,jobs, totalJobs);
        RR(detail, jobs, totalJobs);
        SJF(detail, jobs, totalJobs);
        }
	
	//system("pause");
  }
