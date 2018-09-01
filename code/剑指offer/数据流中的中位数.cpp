// 如何得到一个数据流中的中位数？如果从数据流中读出奇数个数值，那么中位数就是所有数值排序之后位于中间的数值。
// 如果从数据流中读出偶数个数值，那么中位数就是所有数值排序之后中间两个数的平均值。
// 我们使用Insert()方法读取数据流，使用GetMedian()方法获取当前读取数据的中位数。

class Solution {
public:
    typedef multiset<int, greater<int>> maxset;
    typedef multiset<int, less<int>> miniset;
    typedef multiset<int>::iterator iter;
    void Insert(int num)
    {
        // 偶数，插入最大堆
        if (maxset.size() == miniset.size()) {
        	// 如果这个数大于最小堆的堆顶，则应该先插入到最小堆中，在取堆顶插入最大堆
        	if (num > miniset[0]) {
        		miniset.insert(num);
        		iter = miniset.begin();
        		num = *iter;
        		miniset.erase(iter);
        		maxset.insert(num);
        	} else { // 否则，直接插入最大堆
        		maxset.insert(num);
        	}
        } else { // 奇数，插入最小堆
        	if (num > maxset[0]) {
        		maxset.insert(num);
        		iter = maxset.begin();
        		num = *iter;
        		maxset.erase(iter);
        		miniset.insert(num);
        	} else { // 否则，直接插入最大堆
        		miniset.insert(num);
        	}
        }
    }

    double GetMedian()
    { 
    	if(maxset.size() == 0){
    		return 0;
    	}
    	if(maxset.size() == miniset.size()) {
    		return (maxset[0] + miniset[0]) / 2;
    	}
    	return maxset[0];
    }
};