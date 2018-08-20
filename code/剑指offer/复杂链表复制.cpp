// 输入一个复杂链表（每个节点中有节点值，以及两个指针，一个指向下一个节点，另一个特殊指针指向任意一个节点），
// 返回结果为复制后复杂链表的head。（注意，输出结果中请不要返回参数中的节点引用，否则判题程序会直接返回空）

/*
struct RandomListNode {
    int label;
    struct RandomListNode *next, *random;
    RandomListNode(int x) :
            label(x), next(NULL), random(NULL) {
    }
};
*/
class Solution {
public:
    RandomListNode* Clone(RandomListNode* pHead){
        if(pHead == nullptr) {
            return nullptr;
        }
         
        RandomListNode* currentNode = pHead;
        //1、复制每个结点，如复制结点A得到A1，将结点A1插到结点A后面；
        while(currentNode != nullptr){
            RandomListNode* cloneNode = new RandomListNode(currentNode->label);
            cloneNode->next = currentNode->next;
            currentNode->next = cloneNode;
            currentNode = cloneNode->next;
        }
         
        currentNode = pHead;
        //2、重新遍历链表，复制老结点的随机指针给新结点，如A1.random = A.random.next;
        while(currentNode != nullptr) {
            currentNode->next->random = currentNode->random == nullptr? nullptr : currentNode->random->next;
            currentNode = currentNode->next->next;
        }
         
        //3、拆分链表，将链表拆分为原链表和复制后的链表
        currentNode = pHead->next;
        RandomListNode* cloneHead = currentNode;
        while(currentNode->next != nullptr) {
            currentNode->next = currentNode->next->next;
            currentNode = currentNode->next;
        }
        return cloneHead;
    }
};