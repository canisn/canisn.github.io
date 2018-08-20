// 输入两个整数序列，第一个序列表示栈的压入顺序，请判断第二个序列是否可能为该栈的弹出顺序。假设压入栈的所有数字均不相等。
// 例如序列1,2,3,4,5是某栈的压入顺序，序列4,5,3,2,1是该压栈序列对应的一个弹出序列，但4,3,5,1,2就不可能是该压栈序列的弹出序列。
// （注意：这两个序列的长度是相等的）

class Solution {
private:
    stack<int> sta;
public:
    bool IsPopOrder(vector<int> pushV,vector<int> popV) {
        int j = 0;
        for(int i=0; i<pushV.size(); i++){
            sta.push(pushV[i]);
            while(!sta.empty() && sta.top() == popV[j]){
                sta.pop();
                j++;
            }
        }
        // 这里可可以判断栈是否为空，另外就是栈为空的时候j已经超出数组范围了。不是size()-1
        if(j == popV.size()){
            return true;
        }
        else{
            return false;
        }
    }
};