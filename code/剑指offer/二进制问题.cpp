// 输入一个整数，输出该数二进制表示中1的个数。其中负数用补码表示。
#include <iostream>
#include <vector>

using namespace std;
// class Solution {
// public:
//      int  NumberOf1(int n) {
//         int index = 1;
//         int number = 0;
         
//         while(index!=0){
//             if((n & index)!=0)
//                 number++;
//             index = index << 1;
//         }
//         return number;
//      }
// };

// 高端解法，直接统计个数，复杂度就是1的个数
class Solution {
public:
     int NumberOf1(int n) {
        int number = 0;
        while(n != 0){
            n&=(n-1);
            number++;
        }
        return number;
     }
     int NumberOf0(int n) {
        int number = 0;
        while(n != 0){
            n&=(n+1);
            number++;
        }
        return number;
     }
};


int main(int argc, char const *argv[])
{
	Solution s;
	cout << s.NumberOf1(3);
	cout << s.NumberOf0(3);
	return 0;
}