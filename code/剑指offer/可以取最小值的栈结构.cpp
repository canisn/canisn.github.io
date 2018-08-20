// 定义栈的数据结构，请在该类型中实现一个能够得到栈中所含最小元素的min函数（时间复杂度应为O（1））。

class Solution {
private:
    stack<int> sta;
    stack<int> mins;
    stack<int> temp;
    
public:
    void push(int value) {
        sta.push(value);
        while(!mins.empty() && value > mins.top()){
            temp.push(mins.top());
            mins.pop();
        }
        mins.push(value);
        while(!temp.empty()){
            mins.push(temp.top());
            temp.pop();
        }
    }
    void pop() {
        if(sta.empty()){
            return;
        }
        int value = sta.top();
        sta.pop();
        while(mins.top() != value){
            temp.push(mins.top());
            mins.pop();
        }
        mins.pop();
        while(!temp.empty()){
            mins.push(temp.top());
            temp.pop();
        }
    }
    int top() {
        return sta.top();
    }
    int min() {
        return mins.top();
    }
};