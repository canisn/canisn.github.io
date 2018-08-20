// 输入一个链表，反转链表后，输出新链表的表头。
#include <iostream>
#include <vector>

using namespace std;
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
    ListNode* ReverseList(ListNode* pHead) {
        ListNode* node = pHead;
        ListNode* pre = nullptr;
        ListNode* revHead = nullptr;
        
        while(node != nullptr){
            ListNode* temp = node->next;
            if(temp == nullptr){
                revHead = node;
                // 注意这里还不能跳出，这个节点指针还没有反转
                //break;
            }
            node->next = pre;
            pre = node;
            node = temp;
        }
        return revHead;
    }
};