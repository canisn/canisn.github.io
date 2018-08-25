#include <stdio.h>
#include <iostream>
#include <string.h>
using namespace std;

int main(int argc, char const *argv[])
{
	int a = 18;
	int index = 1;
	while((index & a) == 0) {
		index = index << 1;
	}
	cout << index;
	return 0;
}

class Solution {
public:
    int Add(int num1, int num2)
    {
        int carry = 0;
        int bit = 0;
        int result = 0;
        int index = 1;
        while(index <= 0x8FFF){
        	bit = (num1 ^ index) ^ (num2 ^ index) | carry;
        	carry = (num1 ^ index) & (num2 ^ index) ? (num1 ^ index) & (num2 ^ index) : (num1 ^ index) ^ (num2 ^ index) & carry;
        	result | (bit << index);
        	index << 1;
        }
        return result;
    }
};