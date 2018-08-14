// 输入一个整数数组，实现一个函数来调整该数组中数字的顺序，使得所有的奇数位于数组的前半部分，
// 所有的偶数位于数组的后半部分，并保证奇数和奇数，偶数和偶数之间的相对位置不变。

class Solution {
public:
    void reOrderArray(vector<int> &array) {
        int length = array.size();
        if(length <= 0){
        	return;
        }
        int i = 0, j = 0, k = 0, temp = 0;
        while(i < length){
        	while(i < length && (array[i] % 2 == 0)){
        		i++;
        	}
        	if(i >= length){
        		break;
        	}
        	temp = array[i];
        	for(j = i; j > k; j--){
        		array[j] = array[j-1];
        	}
        	array[j] = temp;
        	i++;
        	k++;
        }
    }
};