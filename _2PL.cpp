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
#include<chrono>
#include<set>
#include<map>
using namespace std;
vector<shared_mutex*> VSshared_mutex;
vector<shared_mutex*> VXshared_mutex;
map<int,int> DONT_WRITE;
map<int,int> DONT_READ;
vector<int> a;
sem_t semaphore;
void threadfunc(int write, set<int> read,string str){
	//cout<<str<<endl;
	//cout<<"write="<<write<<endl;
	
	//cout<<"DONT WRITE ";
	//for(set<int>::iterator it2=read.begin();it2!=read.end();it2++){
	//	cout<<"D"<<*it2<<" ";
	//}
	//cout<<endl;
	//phase 1
	
	//cout<<"here\n";
	
	while(DONT_WRITE[write] != 0);
	
	DONT_READ[write]++;
	//cout<<"read: ";
	for(set<int>::iterator it2=read.begin();it2!=read.end();it2++){
		//cout<<*it2<<" ";
		DONT_WRITE[*it2]++;
	}
	//cout<<"X lock "<<"X"<<write<<endl;
	unique_lock<shared_mutex> Xlock(*VSshared_mutex[write]);
	//cout<<"S lock ";
	for(set<int>::iterator it2=read.begin();it2!=read.end();it2++){
		if(*it2 != write)shared_lock<shared_mutex> Slock(*VSshared_mutex[*it2]);
		//cout<<"S"<<*it2<<" ";
	}
	//cout<<endl;
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
		if(str[i]!=' ')tmp.push_back(str[i]);
		else{
			if(tmp[0] == '$'){
				tmp.erase(tmp.begin());
				while(DONT_READ[stoi(tmp)]!=0){cout<<stoi(tmp)<<endl;};
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
		while(DONT_READ[stoi(tmp)]!=0);
		if(sign==0) ans += a[stoi(tmp)];
		else ans-= a[stoi(tmp)];
	}else if(tmp[0]>='0' and tmp[0]<='9'){
		if(sign==0) ans += stoi(tmp);
		else ans -= stoi(tmp);
	}
	a[write] = ans ;
	//phase 2
	DONT_READ[write]--;
	//cout<<"X unlock "<<write<<endl;
	VSshared_mutex[write]->unlock();
	for(set<int>::iterator it2=read.begin();it2!=read.end();it2++){
		//cout<<"S unlock "<<*it2<<endl;
		DONT_WRITE[*it2]--;
		VSshared_mutex[*it2]->unlock();
	}
}

int main(int argc,char *argv[])
{
	auto startTime = std::chrono::high_resolution_clock::now();
	//fstream file;
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
		DONT_WRITE[i]=0;
		DONT_READ[i]=0;
	}
	string str;
	//vector<vector<int> > ReadWrite;
	int counter = 0;
	getline(cin,str);
	while(getline(cin,str)){
		if(str=="")break;
		set<int> read;
		int write;
		string tmp="";
		int flag=0;
		for(int i=0;i<str.length();i++){
			if(str[i]!=' ')tmp.push_back(str[i]);
			else{
				if(tmp == "=" )flag=1;
				else if(tmp[0] == '$'){
					tmp.erase(tmp.begin());
					int r = stoi(tmp);
					if(flag==0){
						cout<<"write: "<<r<<"  read";
						write = r;
					}else{
						read.insert(r);
						cout<<r<<" ";
					}
					
										
				}
				tmp="";
			}
		}
		if(tmp[0] == '$'){
			tmp.erase(tmp.begin());
			cout<<stoi(tmp);
			read.insert(stoi(tmp));
		}
		cout<<endl;
		Threadvector[counter++%stoi(argv[1])] = new thread(threadfunc,write,read,str);
		//ReadWrite.push_back(read);
		
	}
	for(int i=0;i<stoi(argv[1]);i++){
		Threadvector[i]->join();
	}
	for(int i=0;i<n;i++){
		file_out<<"$"<<i<<" = "<<a[i]<<endl;
	}

	auto endTime = std::chrono::high_resolution_clock::now();
	float totalTime = std::chrono::duration<float, std::milli> (endTime - startTime).count();
	cout<<"totalTime = "<<totalTime<<"ms"<<endl;
}
