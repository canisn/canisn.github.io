// 输入两个单调递增的链表，输出两个链表合成后的链表，当然我们需要合成后的链表满足单调不减规则。

/*
struct ListNode {
	int val;
	struct ListNode *next;
	ListNode(int x) :
			val(x), next(NULL) {
	}
};*/
class Solution {
public:
    ListNode* Merge(ListNode* pHead1, ListNode* pHead2)
    {
        ListNode* p1 = pHead1;
    	ListNode* p2 = pHead2;
    	ListNode* node = new ListNode(0);
    	ListNode* root = node;
    	// 注意这里要求两个链表都不为空，之后再检查两个链表是否为空
    	while(p1 != NULL && p2 != NULL){
    		while(((p1->val) <= (p2->val)) && (p1 != NULL)){
    			node->next = p1;
                node = p1;
    			p1 = p1->next;
    		}
    		node->next = p2;
            node = p2;
    		p2 = p2->next;
    	}
    	while(p2 != NULL){
    		node->next = p2;
            node = p2;
    		p2 = p2->next;
    	}
        while(p1 != NULL){
    		node->next = p1;
            node = p1;
    		p1 = p1->next;
    	}
    	return root->next;
    }
};