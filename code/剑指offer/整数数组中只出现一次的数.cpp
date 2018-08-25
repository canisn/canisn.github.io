#include <stdio.h>
#include <iostream>
#include <string.h>
#include <vector>
using namespace std;
// 一个整型数组里除了两个数字之外，其他的数字都出现了偶数次。请写程序找出这两个只出现一次的数字。

class Solution {
public:
    void FindNumsAppearOnce(vector<int> data,int* num1,int *num2) {
    	if (data.size() < 2) {
    		return;
    	}
    	int temp = 0;
    	for (int i = 0; i < data.size(); ++i)
    	{
    		temp ^= data[i];
    	}
    	if(temp == 0) {
    		return;
    	}
    	// 找出不同数据中第一个bit不同的位置。
    	int index = 1;
    	while((temp & index) == 0) {
    		index = index << 1;
    	}
    	*num1 = 0;
    	*num2 = 0;
    	for (int i = 0; i < data.size(); ++i)
    	{
    		if((data[i] & index) == 0) {
    			*num1 ^= data[i];
    		}
    		else {
    			*num2 ^= data[i];
    		}
    	}
    }
};

int main(int argc, char const *argv[])
{
	int array[6] = {1,2,3,2,3,4};
	vector<int> data;
	for (int i = 0; i < sizeof(array) / sizeof(array[0]); ++i) {
		data.push_back(array[i]);
	}
	int num1 = 0;
	int num2 = 0;
	Solution solution;
	solution.FindNumsAppearOnce(data, &num1, &num2);
	cout << num1 << "\t" << num2 << endl;
	return 0;
}