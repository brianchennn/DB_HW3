#include<iostream>
#include<thread>
#include<fstream>
#include<cstring>
#include<stdio.h>
#include<cstdlib>
#include<vector>
#include <unistd.h>
#include<semaphore.h>
#include<mutex>
using namespace std;
vector<mutex*> VSmutex;
vector<mutex*> VXmutex;
vector<int> a;
sem_t semaphore;
void threadfunc(vector<int> regs){
	lock_guard<mutex> XLock(*VXmutex[regs[0]]);
	for(int i=1;i<regs.size();i++){
		int reg = regs[i];
		lock_guard<mutex> SLock(*VSmutex[reg]);
	}
	
}

int main()
{
	fstream file;
	//thread Thread[stoi(argv[0])];
	file.open("data/data2",ios::in);
	int n;file>>n;
	vector<thread*> Threadvector; 
	int in;
	for(int i=0;i<n;i++){
		file>>in;
		a.push_back(in);
		mutex *SMutex = new(mutex);
		VSmutex.push_back(SMutex);
		mutex *XMutex = new(mutex);
		VXmutex.push_back(XMutex);
	}
	string str;
	vector<vector<int> > ReadWrite;
	vector<string> line;
	int counter = 0;
	while(getline(file,str)){
		line.push_back(str);
		
		vector<int> regs;
		string tmp="";
		int flag=0;
		for(int i=0;i<str.length();i++){
			if(str[i]!=' ')tmp.push_back(str[i]);
			else{
				
				if(tmp == "=" )flag=1;
				else if(tmp[0] == '$'){
					tmp.erase(tmp.begin());
					regs.push_back(stoi(tmp));
				}
				tmp="";
			}
		}
		
		if(tmp[0] == '$'){
			tmp.erase(tmp.begin());
			regs.push_back(stoi(tmp));
		}
		thread Thread(threadfunc,regs);
		Threadvector.push_back(&Thread);
		//Thread.join();
		counter++;
		ReadWrite.push_back(regs);
	}
	for(int i=0;i<Threadvector.size();i++){
		Threadvector[i]->join();
	}
	cout<<ReadWrite.size();
	


}
