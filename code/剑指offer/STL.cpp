#include<iostream>
#include<map>
#include<string.h>

using namespace std;


int main(int argc, char const *argv[])
{
	map<int,string> m;
	m[1]="a"; m[4]="b"; m[3]="c"; m[7]="e";
	m[6]="f"; m[5]="g";
	m[7]+="0";//键值重复，对应的数据将修改
	cout<<m[7]<<endl;
	//结合pair插入元素//
	map<int,string>mp;
	pair<int,string>p;
	p=make_pair(1,"q");
	mp.insert(p);
	auto mb=mp.begin();
	auto me=mp.end();
	cout<<mb->first<<endl;
	cout<<(*mb).second<<endl; 
	cout<<(*mp.begin()).first<<"\t"; 
	cout<<(*mp.begin()).second<<endl;
	for(auto i=m.begin(); i!=m.end(); i++)//遍历输出
		cout<<(*i).first<<" : "<<(*i).second<<endl;
	return 0;
}

