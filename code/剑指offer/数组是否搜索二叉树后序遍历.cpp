// 输入一个整数数组，判断该数组是不是某二叉搜索树的后序遍历的结果。
// 如果是则输出Yes,否则输出No。假设输入的数组的任意两个数字都互不相同。
class Solution {
public:
    bool VerifySquenceOfBST(vector<int> sequence) {
        if (sequence.size())
            return is_BST(sequence, 0, sequence.size()-1);
        else
            return false;
    }
    bool is_BST(vector<int>& sequence, int start, int end){
        if (start <= end) {
            return true;
        }
        int cur = end-1;
        // 注意这里指针可能到start-1,因此上面那句判断为空的要带<
        while (cur >= start && sequence[cur] > sequence[end]){
            cur--;
        }
        for (int i = start; i < cur; i++){
            if(sequence[i] > sequence[end]){
                return false;
            }
        }
        return is_BST(sequence, start, cur) && is_BST(sequence, cur+1, end-1);
    }
};