#include <iostream>
#include <vector>
#include <stack>
using namespace std;

struct TreeNode
{
	int val;
	vector<TreeNode*> children;
};

//后续遍历非递归算法
void postorderTravesal(TreeNode *root)
{
	if (root == NULL) return;

	stack<TreeNode*>stk;
	stk.push(root);
	TreeNode* cur = NULL;
	while (!stk.empty())
	{
		cur = stk.top();

		if (cur->children.empty())
		{
			//path.push_back(cur->val);//deal with current tree node.

			stk.pop();
		}
		else
		{
			for (unsigned int i = 0; i < cur->children.size(); ++i)
			{
				if (cur->children[i])
				{
					stk.push(cur->children[i]);
				}
			}
		}

	}
}



int main()
{



	return 0;
}