// 输入一个链表，输出该链表中倒数第k个结点。

/*
struct ListNode {
	int val;
	struct ListNode *next;
	ListNode(int x) :
			val(x), next(NULL) {
	}
};*/

// 注意两个问题：输出是一个节点是在一个链表
// 没有第0个节点
class Solution {
public:
    ListNode* FindKthToTail(ListNode* pListHead, unsigned int k) {
    	if(pListHead == nullptr || k <= 0){
    		return nullptr;
    	}
    	ListNode* p = pListHead;
    	for (int i = 1; i < k; ++i)
    	{
    		if(p->next != nullptr){
    			p = p->next;
    		}
    		else{
    			return nullptr;
    		}
    	}
    	ListNode* q = pListHead;
    	while(p->next != nullptr){
    		p = p->next;
    		q = q->next;
    	}
        //q->next = nullptr;
    	return q;
    }
};