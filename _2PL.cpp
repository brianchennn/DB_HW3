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
void threadfunc(vector<int> regs,string str){
	//cout<<regs.size()<<endl;
	//phase 1
	//VXmutex[regs[0]]->lock();
	//VSmutex[regs[0]]->lock();
	//cout<<"locking X S $"<<regs[0]<<endl;
	//for(int i=1;i<regs.size();i++){
		//VSmutex[regs[i]]->lock();
		//cout<<"locking S $"<<regs[i]<<endl;
	//}
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
	//VXmutex[regs[0]]->unlock();
	//VSmutex[regs[0]]->unlock();
	//cout<<"unlocking X S $"<<regs[0]<<endl;
	//for(int i=1;i<regs.size();i++){
		//VSmutex[regs[i]]->unlock();
		//cout<<"locking S $"<<regs[i]<<endl;
	//}
	
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
		mutex *SMutex = new(mutex);
		VSmutex.push_back(SMutex);
		mutex *XMutex = new(mutex);
		VXmutex.push_back(XMutex);
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
		VXmutex[regs[0]]->lock();
		VSmutex[regs[0]]->lock();
		for(int i=1;i<regs.size();i++){
			VSmutex[regs[i]]->lock();
		}
		Threadvector[counter++%stoi(argv[1])] = new thread(threadfunc,regs,str);// 這裡出問題
		VXmutex[regs[0]]->unlock();
		VSmutex[regs[0]]->unlock();
		for(int i=1;i<regs.size();i++){
			VSmutex[regs[i]]->unlock();
		}	
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
	cout<<"totalTime "<<totalTime<<endl;

}
