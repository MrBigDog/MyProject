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
	//Ĭ��Ϊ�����ո�  
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
		//��ʼλ��  
		iterator begin() const
		{
			return iterator(_node);
		}
		//����λ��  const????  
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
			//���ﷵ��iterator�ᱻ����������ʱ����  
			//�����һ����������  
			else
				_find_end(current->children[pos]);
		}
	};

public:
	//ע�⴫position������  
	iterator insert(iterator& position, const T& x)
	{
		tree_node *tmp = new tree_node(x);
		position._node->children.push_back(tmp);
		tmp->parent = position._node;
		return iterator(tmp);
	}
	//����ݹ����  
	void post_recurs_render(tree_node* some, unsigned recurs_level)
	{
		for (unsigned i = 0; i < some->children.size(); i++)
			post_recurs_render(some->children[i], recurs_level + 1);
		for (int i = 0; i < recurs_level; i++)
			cout << "  ";
		cout << some->name << endl;
	}
	//����ݹ����  
	void pre_recurs_render(tree_node* some, unsigned recurs_level)
	{
		for (int i = 0; i < recurs_level; i++)
			cout << "  ";
		cout << some->name << endl;
		for (unsigned i = 0; i < some->children.size(); i++)
			pre_recurs_render(some->children[i], recurs_level + 1);
	}

	//����stack  
	//ʹ��Transform��ʽ�����  
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
		//����ɾ��  
		for (unsigned i = 0; i < some->children.size(); i++)
			destroy(some->children[i]);
		SAFE_DELETE(some);
	}
	std::stack<std::string> form_stack;
};