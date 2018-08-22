// 输入某二叉树的前序遍历和中序遍历的结果，请重建出该二叉树。假设输入的前序遍历和中序遍历的结果中都不含重复的数字。
// 例如输入前序遍历序列{1,2,4,7,3,5,6,8}和中序遍历序列{4,7,2,1,5,3,8,6}，则重建二叉树并返回。
#include <iostream>
#include <vector>

using namespace std;
/**
 * Definition for binary tree
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution {
public:
    TreeNode* reConstructBinaryTree(vector<int> pre,vector<int> vin) {
    	vector<int> preLeft, preRight, vinLeft, vinRight;
    	TreeNode* node = new  TreeNode(pre[0]);
    	int i = 0;
    	while(vin[i] != pre[0]){
    		i++;
    	}
    	// 存储左子树序列
    	for(int j = 0; j < i; j++){
    		preLeft.push_back(pre[j+1]);
    		vinLeft.push_back(vin[j]);
    	}
    	// 存储右子树序列
    	for(int j=i+1; j<pre.size(); j++){
    		preRight.push_back(pre[j]);
    		vinRight.push_back(vin[j]);
    	}
    	// 递归调用
    	if(preLeft.size() >0){
    		node->left = reConstructBinaryTree(preLeft, vinLeft);
    	}
    	if(preRight.size()>0){
    		node->right = reConstructBinaryTree(preRight, vinRight);
    	}
    	return node;
    }
}