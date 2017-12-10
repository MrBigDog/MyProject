#include <iostream>  
#include <string>  
#include <vector>  
#include <stack>  
#include <cassert>  
using namespace std;

template<class T>
class htree_node
{
public:
	typedef htree_node<T> node_type;

	htree_node() :parent(0), format("  ")
	{}
	htree_node(const T& x) :name(x), parent(0), format("  ")
	{}
	~htree_node() {}

	T name;
	//默认为两个空格  
	std::string format;
	node_type *parent;
	std::vector<node_type*> children;
};
template<class T, class Container = htree_node<T> >

class htree
{
protected:
	typedef Container tree_node;

public:
	htree() :root(0)
	{
		Init();
	}

	htree(tree_node *node) :root(node)
	{
		Init();
	}

	~htree()
	{
		destroy(root);
	}
	//pre_order_iterator  
	class iterator
	{
	public:
		iterator() : _node(0), skip_current_children_(false)
		{
			skip_children();
		}
		iterator(tree_node *node)
			: _node(node)
			, skip_current_children_(false)
		{
			skip_children();
		}

		~iterator() {}

		T& operator*() const
		{
			return _node->name;
		}
		T* operator->() const
		{
			return &(_node->name);
		}
		tree_node* get()
		{
			return _node;
		}
		//开始位置  
		iterator begin() const
		{
			return iterator(_node);
		}
		//结束位置  const????  
		iterator end() const
		{
			return iterator(_find_end(_node));
		}

		tree_node *_node;
	protected:
		bool skip_current_children_;
		void skip_children()
		{
			skip_current_children_ = true;
		}

		tree_node* _find_end(tree_node* current) const
		{
			int pos = current->children.size() - 1;
			if (pos < 0)
				return (current);
			//这里返回iterator会被析构掉，临时对象  
			//从最后一个孩子找起，  
			else
				_find_end(current->children[pos]);
		}
	};

public:
	//注意传position的引用  
	iterator insert(iterator& position, const T& x)
	{
		tree_node *tmp = new tree_node(x);
		position._node->children.push_back(tmp);
		tmp->parent = position._node;
		return iterator(tmp);
	}
	//后序递归输出  
	void post_recurs_render(tree_node* some, unsigned recurs_level)
	{
		for (unsigned i = 0; i < some->children.size(); i++)
			post_recurs_render(some->children[i], recurs_level + 1);
		for (int i = 0; i < recurs_level; i++)
			cout << "  ";
		cout << some->name << endl;
	}
	//先序递归输出  
	void pre_recurs_render(tree_node* some, unsigned recurs_level)
	{
		for (int i = 0; i < recurs_level; i++)
			cout << "  ";
		cout << some->name << endl;
		for (unsigned i = 0; i < some->children.size(); i++)
			pre_recurs_render(some->children[i], recurs_level + 1);
	}

	//利用stack  
	//使用Transform格式化输出  
	void recurs_render(tree_node* some)
	{
		std::string temp;
		temp = form_stack.top() + some->format;
		form_stack.push(temp);

		cout << temp;
		cout << some->name;
		cout << endl;
		for (unsigned i = 0; i < some->children.size(); i++)
			recurs_render(some->children[i]);
		form_stack.pop();
	}
	tree_node *root;

private:
	void Init() 
	{
		form_stack.push(std::string(" "));
	};

	void destroy(tree_node *some)
	{
#define SAFE_DELETE(p) {if(p){delete p; p=NULL;}}  
		//后序删除  
		for (unsigned i = 0; i < some->children.size(); i++)
			destroy(some->children[i]);
		SAFE_DELETE(some);
	}
	std::stack<std::string> form_stack;
};