#include<iostream>
#include<pthread.h>
#include<fstream>
#include<string.h>
#include<cstring>
#include<stdio.h>
#include<cstdlib>
#include<vector>
#include<semaphore.h>
using namespace std;


void *threadfunc(void* arg){

	//pthread_exit();
}

int main()
{
	fstream file;
	sem_t *semaphore;
	unsigned un=0;
	sem_init(semaphore,0,un);
	file.open("data/data2",ios::in);
	int n;file>>n;
	int a[n];
	for(int i=0;i<n;i++){
		file>>a[n];
	}
	string str;
	string::size_type sz;
	vector<pair<int,vector<int> > > ReadWrite;
	while(getline(file,str)){
		vector<string> v;
		pair<int,vector<int> > pa;
		string tmp="";
		int flag=0;
		for(int i=0;i<str.length();i++){
			if(str[i]!=' ')tmp.push_back(str[i]);
			else{
				v.push_back(tmp);
				if(tmp == "=" )flag=1;
				else if(tmp[0] == '$'){
					tmp.erase(tmp.begin());
					if(flag==0)	pa.first = stoi(tmp,&sz);
					else	    pa.second.push_back(stoi(tmp,&sz));
				}
				tmp="";
			}
			
		}
		v.push_back(tmp);
		if(tmp[0] == '$'){
			tmp.erase(tmp.begin());
			if(flag==0) pa.first = stoi(tmp,&sz);
			else 		pa.second.push_back(stoi(tmp,&sz));
			
		}
		ReadWrite.push_back(pa);
	}
	cout<<ReadWrite.size();
	for(int i=0;i<ReadWrite.size();i++){
		cout<<ReadWrite[i].first<<"          ";
		for(int j=0;j<ReadWrite[i].second.size();j++){
			cout<<ReadWrite[i].second[j]<<" ";
		}
		cout<<endl;
	}

}
