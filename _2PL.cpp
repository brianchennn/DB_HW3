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
	
	//phase 1
	VXmutex[regs[0]]->lock();
	VSmutex[regs[0]]->lock();
	//cout<<"locking X S $"<<regs[0]<<endl;
	for(int i=1;i<regs.size();i++){
		VSmutex[regs[i]]->lock();
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
	VXmutex[regs[0]]->unlock();
	VSmutex[regs[0]]->unlock();
	//cout<<"unlocking X S $"<<regs[0]<<endl;
	for(int i=1;i<regs.size();i++){
		VSmutex[regs[i]]->unlock();
		//cout<<"locking S $"<<regs[i]<<endl;
	}
	
}

int main(int argc,char *argv[])
{
	fstream file;
	fstream file_out;
	//thread Thread[stoi(argv[0])];
	file.open("data/data3",ios::in);
	file_out.open("data_out",ios::out);
	int n;file>>n;
	vector<thread *> Threadvector(16);
	
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
	int counter = 0;
	getline(file,str);
	while(getline(file,str)){
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
					vector<int>::iterator it = find(regs.begin(),regs.end(),stoi(tmp));
					if(it==regs.end())regs.push_back(stoi(tmp));
					
				}
				tmp="";
			}
		}
		if(tmp[0] == '$'){
			tmp.erase(tmp.begin());
			regs.push_back(stoi(tmp));
		}
		Threadvector[counter++%16] = new thread(threadfunc,regs,str);// 這裡出問題
		//threadfunc(regs,str);
		ReadWrite.push_back(regs);
	}
		
	for(int i=0;i<n;i++){
		file_out<<"$"<<i<<" = "<<a[i]<<endl;
	}


}
