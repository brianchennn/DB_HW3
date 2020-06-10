#include<iostream>
#include<thread>
#include<fstream>
#include<cstring>
#include<stdio.h>
#include<cstdlib>
#include<vector>
#include <unistd.h>
#include<semaphore.h>
#include<shared_mutex>
using namespace std;
vector<shared_mutex*> VSshared_mutex;
vector<shared_mutex*> VXshared_mutex;
vector<int> a;
sem_t semaphore;
void threadfunc(vector<int> regs,string str){
	//cout<<regs.size()<<endl;
	//phase 1
	unique_lock<shared_mutex> Xlock(*VSshared_mutex[regs[0]]);
	//VXshared_mutex[regs[0]]->lock();
	//VSshared_mutex[regs[0]]->lock();
	//cout<<"locking X S $"<<regs[0]<<endl;
	for(int i=1;i<regs.size();i++){
		unique_lock<shared_mutex> Slock(*VSshared_mutex[regs[i]]);
		//VSshared_mutex[regs[i]]->lock();
		//cout<<"locking S $"<<regs[i]<<endl;
	}
	int ans = 0;
	string tmp="";
	int i=0;
	for(;str[i]!=' ';i++){
		tmp.push_back(str[i]);
	}
	tmp.erase(tmp.begin());
	tmp = "";
	bool sign=0;
	i+=3;
	for(;i<str.size();i++){
		//cout<<"i="<<i<<endl;
		if(str[i]!=' ')tmp.push_back(str[i]);
		else{
			//cout<<tmp<<" ";
			if(tmp[0] == '$'){
				tmp.erase(tmp.begin());
				//cout<<"("<<a[stoi(tmp)]<<")"<<" ";
				if(sign==0) ans += a[stoi(tmp)];
				else ans-= a[stoi(tmp)];
			}else if(tmp[0]>='0' and tmp[0]<='9'){
				if(sign==0) ans += stoi(tmp);
				else ans -= stoi(tmp);
			}else if(tmp[0]=='+' or tmp[0]=='-'){
				if(tmp[0]=='+')sign=0;
				else sign=1;
			}
			tmp="";
		}
	}
	
	if(tmp[0] == '$'){
		tmp.erase(tmp.begin());
		if(sign==0) ans += a[stoi(tmp)];
		else ans-= a[stoi(tmp)];
	}else if(tmp[0]>='0' and tmp[0]<='9'){
		if(sign==0) ans += stoi(tmp);
		else ans -= stoi(tmp);
	}
	
	a[regs[0]] = ans ;
	//phase 2
	//VXshared_mutex[regs[0]]->unlock();
	VSshared_mutex[regs[0]]->unlock();
	//cout<<"unlocking X S $"<<regs[0]<<endl;
	for(int i=1;i<regs.size();i++){
		VSshared_mutex[regs[i]]->unlock();
		//cout<<"locking S $"<<regs[i]<<endl;
	}
	
}

int main(int argc,char *argv[])
{
	//fstream file;
	auto startTime = std::chrono::high_resolution_clock::now();
	fstream file_out;
	//thread Thread[stoi(argv[0])];
	//file.open("data/data3",ios::in);
	file_out.open(argv[2],ios::out);
	int n;cin>>n;
	vector<thread *> Threadvector(stoi(argv[1]));
	
	int in;
	for(int i=0;i<n;i++){
		cin>>in;
		a.push_back(in);
		shared_mutex *SMutex = new(shared_mutex);
		VSshared_mutex.push_back(SMutex);
		shared_mutex *XMutex = new(shared_mutex);
		VXshared_mutex.push_back(XMutex);
	}
	string str;
	vector<vector<int> > ReadWrite;
	int counter = 0;
	getline(cin,str);
	while(getline(cin,str)){
		
		//cout<<"str="<<str<<endl;
		if(str=="")break;
		vector<int> regs;
		string tmp="";
		int flag=0;
		for(int i=0;i<str.length();i++){
			if(str[i]!=' ')tmp.push_back(str[i]);
			else{
				if(tmp == "=" )flag=1;
				else if(tmp[0] == '$'){
					tmp.erase(tmp.begin());
					int r = stoi(tmp);
					bool flag=0;
					for(int i=0;i<regs.size();i++){
						if(regs[i]==r)flag=1;
					}
					if(flag==0){
						regs.push_back(stoi(tmp));
						//cout<<tmp<<" ";		
					}						
				}
				tmp="";
			}
		}
		if(tmp[0] == '$'){
			tmp.erase(tmp.begin());
			regs.push_back(stoi(tmp));
		}
		Threadvector[counter++%stoi(argv[1])] = new thread(threadfunc,regs,str);// 這裡出問題
		//threadfunc(regs,str);
		ReadWrite.push_back(regs);
		//cout<<"\n";
		
	}
	for(int i=0;i<stoi(argv[1]);i++){
		Threadvector[i]->join();
	}
	for(int i=0;i<n;i++){
		file_out<<"$"<<i<<" = "<<a[i]<<endl;
	}
	auto endTime = std::chrono::high_resolution_clock::now();
	float totalTime = std::chrono::duration<float, std::milli> (endTime - startTime).count();
	cout<<"totalTime = "<<totalTime<<endl;
}
