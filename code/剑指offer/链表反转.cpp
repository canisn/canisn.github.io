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
    	if(pHead == nullptr){
    		return nullptr;
    	}
    	ListNode* node = pHead;
    	ListNode* pre = nullptr;
    	ListNode* revHead = nullptr;
    	while(node != nullptr){
    		ListNode* temp = node->next;
    		if(temp == nullptr){
    			revHead = node;
    			break;
    		}
    		node->next = pre;
    		pre = node;
    		node = temp;
    	}
    	return revHead;
    }
};

int main(int argc, char const *argv[])
{
	ListNode* root = new ListNode(1);
	ListNode* p = root;
	for (int i = 0; i < 5; ++i)
	{
		ListNode* node = new ListNode(i);
		p->next = node;
		p = node;
	}
	p = root;
	while(p){
		cout << p->val;
	}

	return 0;
}