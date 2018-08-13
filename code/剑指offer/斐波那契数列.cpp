// 大家都知道斐波那契数列，现在要求输入一个整数n，请你输出斐波那契数列的第n项（从0开始，第0项为0）。n<=39
#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
    int Fibonacci(int n) {
        int a = 0, b = 1, sum = 0;
        for(int i = 0; i < n; i++){
            sum = a+b;
            a = b;
            b = sum;
        }
        return a;
    }
};

// 一只青蛙一次可以跳上1级台阶，也可以跳上2级。求该青蛙跳上一个n级的台阶总共有多少种跳法（先后次序不同算不同的结果）。
class Solution {
public:
    int jumpFloor(int number) {
        if(number <= 0){
            return 0;
        }
        else if(number == 1){
            return 1;
        }
        else if(number == 2){
            return 2;
        }
        return jumpFloor(number-1) + jumpFloor(number-2);
    }
};
// 非递归方法
class Solution {
public:
    int jumpFloor(int number) {
        int a = 0;
        int b = 1;
        int sum = 0;
        for(int i = 0; i < number; i++){
            sum = a + b;
            a = b;
            b = sum;
        }
        return a;
    }
};


// 一只青蛙一次可以跳上1级台阶，也可以跳上2级……它也可以跳上n级。求该青蛙跳上一个n级的台阶总共有多少种跳法。
class Solution {
public:
    int jumpFloorII(int number) {
        if(number <= 0){
            return 0;
        }
        int m[number+1];
        memset(m, 0, sizeof(m));
        m[0] = 0;
        for(int i = 1; i < number+1; i++){
            for(int j = 0; j < i; j++){
                m[i] += m[j];
            }
            m[i] += 1;
        }
        return m[number];
    }
};

// 我们可以用2*1的小矩形横着或者竖着去覆盖更大的矩形。请问用n个2*1的小矩形无重叠地覆盖一个2*n的大矩形，总共有多少种方法？
class Solution {
public:
    int rectCover(int number) {
        int a = 0;
        int b = 1;
        int sum = 0;
        for(int i = 0; i < number; i++){
            sum = a + b;
            a = b;
            b = sum;
        }
        return sum;
    }
}；