# include <compiler.h>
# include <gwBase/Props.h>
# include <gwBase/LogStream.h>

#include <set>
#include <limits>
#include <sstream>
#include <iomanip>
#include <iterator>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <algorithm>

#if PROPS_STANDALONE
# include <iostream>
using std::cerr;
#else
# include <boost/algorithm/string/find_iterator.hpp>
# include <boost/algorithm/string/predicate.hpp>
# include <boost/algorithm/string/classification.hpp>
# include <boost/bind.hpp>
# include <boost/functional/hash.hpp>
# include <boost/range.hpp>

//# include <gwUtil/sg_inlines.h>
//# include "PropertyInterpolationMgr.hxx"
#endif

#define GW_UNUSED(x) (void)x

using std::endl;
using std::find;
using std::sort;
using std::vector;
using std::stringstream;

using namespace gwBase;

////////////////////////////////////////////////////////////////////////
// Local classes.
////////////////////////////////////////////////////////////////////////

/// * Comparator class for sorting by index.
class CompareIndices
{
public:
	int operator() (const GWPropertyNodePtr n1, const GWPropertyNodePtr n2) const
	{
		return (n1->getIndex() < n2->getIndex());
	}
};


////////////////////////////////////////////////////////////////////////
// Convenience macros for value access.
////////////////////////////////////////////////////////////////////////

#define TEST_READ(dflt) if (!getAttribute(READ)) return dflt
#define TEST_WRITE if (!getAttribute(WRITE)) return false

////////////////////////////////////////////////////////////////////////
// Local path normalization code.
////////////////////////////////////////////////////////////////////////

#if PROPS_STANDALONE
struct PathComponent
{
	string name;
	int index;
};
#endif

/**
 * Parse the name for a path component.
 * Name: [_a-zA-Z][-._a-zA-Z0-9]*
 */
template<typename Range>
inline Range parse_name(const GWPropertyNode *node, const Range &path)
{
	typename Range::iterator i = path.begin();
	typename Range::iterator max = path.end();
	if (*i == '.')
	{
		i++;
		if (i != path.end() && *i == '.')
		{
			i++;
		}
		if (i != max && *i != '/')
		{
			throw std::string("illegal character after . or ..");
		}
	}
	else if (isalpha(*i) || *i == '_')
	{
		i++;
		// The rules inside a name are a little
		// less restrictive.
		while (i != max)
		{
			if (isalpha(*i) || isdigit(*i) || *i == '_' || *i == '-' || *i == '.')
			{
				// name += path[i];
			}
			else if (*i == '[' || *i == '/')
			{
				break;
			}
			else
			{
				std::string err = "'";
				err.push_back(*i);
				err.append("' found in propertyname after '" + node->getNameString() + "'");
				err.append("\nname may contain only ._- and alphanumeric characters");
				throw err;
			}
			i++;
		}
	}
	else
	{
		if (path.begin() == i)
		{
			std::string err = "'";
			err.push_back(*i);
			err.append("' found in propertyname after '" + node->getNameString() + "'");
			err.append("\nname must begin with alpha or '_'");
			throw err;
		}
	}
	return Range(path.begin(), i);
}

// Validate the name of a single node
inline bool validateName(const std::string& name)
{
	using namespace boost;
	if (name.empty())
	{
		return false;
	}
	if (!isalpha(name[0]) && name[0] != '_')
	{
		return false;
	}
#if PROPS_STANDALONE
	std::string is_any_of("_-.");
	bool rv = true;
	for (unsigned i = 1; i < name.length(); ++i)
	{
		if (!isalnum(name[i]) && is_any_of.find(name[i]) == std::string::npos)
		{
			rv = false;
			break;
		}
	}
	return rv;
#else
	return all(make_iterator_range(name.begin(), name.end()), is_alnum() || is_any_of("_-."));
#endif
}

#if PROPS_STANDALONE
/**
 * Parse the name for a path component.
 * Name: [_a-zA-Z][-._a-zA-Z0-9]*
 */
static inline const string parse_name(const string &path, int &i)
{
	string name = "";
	int max = (int)path.size();

	if (path[i] == '.') 
	{
		i++;
		if (i < max && path[i] == '.') 
		{
			i++;
			name = "..";
		}
		else 
		{
			name = ".";
		}
		if (i < max && path[i] != '/')
			throw string("Illegal character after " + name);
	}

	else if (isalpha(path[i]) || path[i] == '_') 
	{
		name += path[i];
		i++;

		// The rules inside a name are a little
		// less restrictive.
		while (i < max) {
			if (isalpha(path[i]) || isdigit(path[i]) || path[i] == '_' ||
				path[i] == '-' || path[i] == '.') {
				name += path[i];
			}
			else if (path[i] == '[' || path[i] == '/') {
				break;
			}
			else {
				throw string("name may contain only ._- and alphanumeric characters");
			}
			i++;
		}
	}

	else {
		if (name.size() == 0)
			throw string("name must begin with alpha or '_'");
	}

	return name;
}


/**
 * Parse the optional integer index for a path component.
 *
 * Index: "[" [0-9]+ "]"
 */
static inline int parse_index(const string &path, int &i)
{
	int index = 0;

	if (path[i] != '[')
		return 0;
	else
		i++;

	for (int max = (int)path.size(); i < max; i++) {
		if (isdigit(path[i])) {
			index = (index * 10) + (path[i] - '0');
		}
		else if (path[i] == ']') {
			i++;
			return index;
		}
		else {
			break;
		}
	}

	throw string("unterminated index (looking for ']')");
}

/**
 * Parse a single path component.
 *
 * Component: Name Index?
 */
static inline PathComponent parse_component(const string &path, int &i)
{
	PathComponent component;
	component.name = parse_name(path, i);
	if (component.name[0] != '.')
		component.index = parse_index(path, i);
	else
		component.index = -1;
	return component;
}

/**
 * Parse a path into its components.
 */
static void parse_path(const string &path, vector<PathComponent> &components)
{
	int pos = 0;
	int max = (int)path.size();

	// Check for initial '/'
	if (path[pos] == '/') {
		PathComponent root;
		root.name = "";
		root.index = -1;
		components.push_back(root);
		pos++;
		while (pos < max && path[pos] == '/')
			pos++;
	}

	while (pos < max) {
		components.push_back(parse_component(path, pos));
		while (pos < max && path[pos] == '/')
			pos++;
	}
}
#endif


////////////////////////////////////////////////////////////////////////
// Other static utility functions.
////////////////////////////////////////////////////////////////////////
static char * copy_string(const char * s)
{
	size_t slen = strlen(s);
	char * copy = new char[slen + 1];

	// the source string length is known so no need to check for '\0'
	// when copying every single character
	memcpy(copy, s, slen);
	*(copy + slen) = '\0';
	return copy;
}

static bool compare_strings(const char * s1, const char * s2)
{
	return !strncmp(s1, s2, GWPropertyNode::MAX_STRING_LEN);
}

/// * Locate a child node by name and index.
template<typename Itr>
static int find_child(Itr begin, Itr end, int index, const PropertyList& nodes)
{
	size_t nNodes = nodes.size();
#if PROPS_STANDALONE
	for (int i = 0; i < nNodes; i++)
	{
		GWPropertyNode * node = nodes[i];
		if (node->getIndex() == index && compare_strings(node->getName(), begin))
			return i;
	}
#else
	boost::iterator_range<Itr> name(begin, end);
	for (size_t i = 0; i < nNodes; i++)
	{
		GWPropertyNode * node = nodes[i];

		// searching for a matching index is a lot less time consuming than
		// comparing two strings so do that first.
		if (node->getIndex() == index && boost::equals(node->getName(), name))
		{
			return static_cast<int>(i);
		}
	}
#endif
	return -1;
}

/// * Locate the child node with the highest index of the same name
static int find_last_child(const char * name, const PropertyList& nodes)
{
	size_t nNodes = nodes.size();
	int index = -1;

	for (size_t i = 0; i < nNodes; i++)
	{
		GWPropertyNode * node = nodes[i];
		if (compare_strings(node->getName(), name))
		{
			int idx = node->getIndex();
			if (idx > index) index = idx;
		}
	}
	return index;
}

/// * Get first unused index for child nodes with the given name
static int first_unused_index(const char * name, const PropertyList& nodes, int min_index)
{
	const char* nameEnd = name + strlen(name);
	for (int index = min_index; index < std::numeric_limits<int>::max(); ++index)
	{
		if (find_child(name, nameEnd, index, nodes) < 0)
		{
			return index;
		}
	}
	GW_LOG(GW_GENERAL, GW_ALERT, "Too many nodes: " << name);
	return -1;
}

template<typename Itr>
inline GWPropertyNode* GWPropertyNode::getExistingChild(Itr begin, Itr end, int index)
{
	int pos = find_child(begin, end, index, _children);
	if (pos >= 0)
	{
		return _children[pos];
	}
	return 0;
}

template<typename Itr>
GWPropertyNode * GWPropertyNode::getChildImpl(Itr begin, Itr end, int index, bool create)
{
	GWPropertyNode* node = getExistingChild(begin, end, index);
	if (node)
	{
		return node;
	}
	else if (create)
	{
		node = new GWPropertyNode(begin, end, index, this);
		_children.push_back(node);
		fireChildAdded(node);
		return node;
	}
	else
	{
		return 0;
	}
}

template<typename SplitItr>
GWPropertyNode* find_node_aux(GWPropertyNode * current, SplitItr& itr, bool create, int last_index)
{
	typedef typename SplitItr::value_type Range;
	// Run off the end of the list
	if (current == 0)
	{
		return 0;
	}

	// Success! This is the one we want.
	if (itr.eof())
	{
		return current;
	}
	Range token = *itr;
	// Empty name at this point is empty, not root.
	if (token.empty())
	{
		return find_node_aux(current, ++itr, create, last_index);
	}
	Range name = parse_name(current, token);
	if (equals(name, "."))
	{
		return find_node_aux(current, ++itr, create, last_index);
	}
	if (equals(name, ".."))
	{
		GWPropertyNode * parent = current->getParent();
		if (parent == 0)
		{
			throw std::string("attempt to move past root with '..'");
		}
		return find_node_aux(parent, ++itr, create, last_index);
	}
	int index = -1;
	if (last_index >= 0)
	{
		// If we are at the last token and last_index is valid, use
		// last_index as the index value
		bool lastTok = true;
		while (!(++itr).eof())
		{
			if (!itr->empty())
			{
				lastTok = false;
				break;
			}
		}
		if (lastTok)
		{
			index = last_index;
		}
	}
	else
	{
		++itr;
	}

	if (index < 0)
	{
		index = 0;
		if (name.end() != token.end())
		{
			if (*name.end() == '[')
			{
				typename Range::iterator i = name.end() + 1, end = token.end();
				for (; i != end; ++i)
				{
					if (isdigit(*i))
					{
						index = (index * 10) + (*i - '0');
					}
					else
					{
						break;
					}
				}
				if (i == token.end() || *i != ']')
				{
					throw std::string("unterminated index (looking for ']')");
				}
			}
			else
			{
				throw std::string("illegal characters in token: ")
					+ std::string(name.begin(), name.end());
			}
		}
	}
	return find_node_aux(current->getChildImpl(name.begin(), name.end(),
		index, create), itr, create,
		last_index);
}

// Internal function for parsing property paths. last_index provides
// and index value for the last node name token, if supplied.
#if PROPS_STANDALONE
static GWPropertyNode *
find_node(GWPropertyNode * current,
	const vector<PathComponent> &components,
	int position,
	bool create)
{
	// Run off the end of the list
	if (current == 0) {
		return 0;
	}

	// Success! This is the one we want.
	else if (position >= (int)components.size()) {
		return (current->getAttribute(GWPropertyNode::REMOVED) ? 0 : current);
	}

	// Empty component means root.
	else if (components[position].name == "") {
		return find_node(current->getRootNode(), components, position + 1, create);
	}

	// . means current directory
	else if (components[position].name == ".") {
		return find_node(current, components, position + 1, create);
	}

	// .. means parent directory
	else if (components[position].name == "..") {
		GWPropertyNode * parent = current->getParent();
		if (parent == 0)
			throw string("Attempt to move past root with '..'");
		else
			return find_node(parent, components, position + 1, create);
	}

	// Otherwise, a child name
	else {
		GWPropertyNode * child =
			current->getChild(components[position].name.c_str(),
				components[position].index,
				create);
		return find_node(child, components, position + 1, create);
	}
}
#else
template<typename Range>
GWPropertyNode* find_node(GWPropertyNode * current, const Range& path, bool create, int last_index = -1)
{
	using namespace boost;
	typedef split_iterator<typename range_result_iterator<Range>::type>	PathSplitIterator;

	PathSplitIterator itr = make_split_iterator(path, first_finder("/", is_equal()));
	if (*path.begin() == '/')
	{
		return find_node_aux(current->getRootNode(), itr, create, last_index);
	}
	else
	{
		return find_node_aux(current, itr, create, last_index);
	}
}
#endif

////////////////////////////////////////////////////////////////////////
// Private methods from GWPropertyNode (may be inlined for speed).
////////////////////////////////////////////////////////////////////////

inline bool GWPropertyNode::get_bool() const
{
	if (_tied)
	{
		return static_cast<GWRawValue<bool>*>(_value.val)->getValue();
	}
	else
	{
		return _local_val.bool_val;
	}
}

inline int GWPropertyNode::get_int() const
{
	if (_tied)
	{
		return (static_cast<GWRawValue<int>*>(_value.val))->getValue();
	}
	else
	{
		return _local_val.int_val;
	}
}

inline long GWPropertyNode::get_long() const
{
	if (_tied)
	{
		return static_cast<GWRawValue<long>*>(_value.val)->getValue();
	}
	else
	{
		return _local_val.long_val;
	}
}

inline float GWPropertyNode::get_float() const
{
	if (_tied)
	{
		return static_cast<GWRawValue<float>*>(_value.val)->getValue();
	}
	else
	{
		return _local_val.float_val;
	}
}

inline double GWPropertyNode::get_double() const
{
	if (_tied)
	{
		return static_cast<GWRawValue<double>*>(_value.val)->getValue();
	}
	else
	{
		return _local_val.double_val;
	}
}

inline const char * GWPropertyNode::get_string() const
{
	if (_tied)
	{
		return static_cast<GWRawValue<const char*>*>(_value.val)->getValue();
	}
	else
	{
		return _local_val.string_val;
	}
}

inline bool GWPropertyNode::set_bool(bool val)
{
	if (_tied)
	{
		if (static_cast<GWRawValue<bool>*>(_value.val)->setValue(val))
		{
			fireValueChanged();
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		_local_val.bool_val = val;
		fireValueChanged();
		return true;
	}
}

inline bool GWPropertyNode::set_int(int val)
{
	if (_tied)
	{
		if (static_cast<GWRawValue<int>*>(_value.val)->setValue(val))
		{
			fireValueChanged();
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		_local_val.int_val = val;
		fireValueChanged();
		return true;
	}
}

inline bool GWPropertyNode::set_long(long val)
{
	if (_tied)
	{
		if (static_cast<GWRawValue<long>*>(_value.val)->setValue(val))
		{
			fireValueChanged();
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		_local_val.long_val = val;
		fireValueChanged();
		return true;
	}
}

inline bool GWPropertyNode::set_float(float val)
{
	if (_tied)
	{
		if (static_cast<GWRawValue<float>*>(_value.val)->setValue(val))
		{
			fireValueChanged();
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		_local_val.float_val = val;
		fireValueChanged();
		return true;
	}
}

inline bool
GWPropertyNode::set_double(double val)
{
	if (_tied)
	{
		if (static_cast<GWRawValue<double>*>(_value.val)->setValue(val))
		{
			fireValueChanged();
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		_local_val.double_val = val;
		fireValueChanged();
		return true;
	}
}

inline bool GWPropertyNode::set_string(const char * val)
{
	if (_tied)
	{
		if (static_cast<GWRawValue<const char*>*>(_value.val)->setValue(val))
		{
			fireValueChanged();
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		delete[] _local_val.string_val;
		_local_val.string_val = copy_string(val);
		fireValueChanged();
		return true;
	}
}

void GWPropertyNode::clearValue()
{
	if (_type == props::ALIAS)
	{
		{
			//put(_value.alias);
			(_value.alias)->unref();
		}
		_value.alias = 0;
	}
	else if (_type != props::NONE)
	{
		switch (_type)
		{
		case props::BOOL:
			_local_val.bool_val = GWRawValue<bool>::DefaultValue();
			break;
		case props::INT:
			_local_val.int_val = GWRawValue<int>::DefaultValue();
			break;
		case props::LONG:
			_local_val.long_val = GWRawValue<long>::DefaultValue();
			break;
		case props::FLOAT:
			_local_val.float_val = GWRawValue<float>::DefaultValue();
			break;
		case props::DOUBLE:
			_local_val.double_val = GWRawValue<double>::DefaultValue();
			break;
		case props::STRING:
		case props::UNSPECIFIED:
			if (!_tied)
			{
				delete[] _local_val.string_val;
			}
			_local_val.string_val = 0;
			break;
		default: // avoid compiler warning
			break;
		}
		delete _value.val;
		_value.val = 0;
	}
	_tied = false;
	_type = props::NONE;
}

/// * Get the value as a string.
const char * GWPropertyNode::make_string() const
{
	if (!getAttribute(READ))
	{
		return "";
	}
	switch (_type)
	{
	case props::ALIAS:
		return _value.alias->getStringValue();
	case props::BOOL:
		return get_bool() ? "true" : "false";
	case props::STRING:
	case props::UNSPECIFIED:
		return get_string();
	case props::NONE:
		return "";
	default:
		break;
	}
	stringstream sstr;
	switch (_type)
	{
	case props::INT:
		sstr << get_int();
		break;
	case props::LONG:
		sstr << get_long();
		break;
	case props::FLOAT:
		sstr << get_float();
		break;
	case props::DOUBLE:
		sstr << std::setprecision(10) << get_double();
		break;
	case props::EXTENDED:
	{
		props::Type realType = _value.val->getType();
		// Perhaps this should be done for all types?
		if (realType == props::VEC3D || realType == props::VEC4D)
		{
			sstr.precision(10);
		}
		static_cast<GWRawExtended*>(_value.val)->printOn(sstr);
	}
	break;
	default: return "";
	}
	_buffer = sstr.str();
	return _buffer.c_str();
}

/// * Trace a write access for a property.
void GWPropertyNode::trace_write() const
{
	GW_LOG(GW_GENERAL, GW_ALERT, "TRACE: Write node " << getPath() << ", value \"" << make_string() << '"');
}

/// * Trace a read access for a property.
void GWPropertyNode::trace_read() const
{
	GW_LOG(GW_GENERAL, GW_ALERT, "TRACE: Read node " << getPath()<< ", value \"" << make_string() << '"');
}

////////////////////////////////////////////////////////////////////////
// Public methods from GWPropertyNode.
////////////////////////////////////////////////////////////////////////

/**
 * Last used attribute
 * Update as needed when enum Attribute is changed
 */
const int GWPropertyNode::LAST_USED_ATTRIBUTE = PRESERVE;

GWPropertyNode::GWPropertyNode()
	: _index(0)
	, _parent(0)
	, _type(props::NONE)
	, _tied(false)
	, _attr(READ | WRITE)
	, _listeners(0)
{
	_local_val.string_val = 0;
	_value.val = 0;
}


GWPropertyNode::GWPropertyNode(const GWPropertyNode &node)
/*: SGReferenced(node),*/
	: _index(node._index)
	, _name(node._name)
	, _parent(0)			// don't copy the parent
	, _type(node._type)
	, _tied(node._tied)
	, _attr(node._attr)
	, _listeners(0)		// CHECK!!
{
	_local_val.string_val = 0;
	_value.val = 0;
	if (_type == props::NONE)
	{
		return;
	}
	if (_type == props::ALIAS)
	{
		_value.alias = node._value.alias;
		{
			//get(_value.alias);?????
			_value.alias->ref();
		}
		_tied = false;
		return;
	}
	if (_tied || _type == props::EXTENDED)
	{
		_value.val = node._value.val->clone();
		return;
	}
	switch (_type)
	{
	case props::BOOL:
		set_bool(node.get_bool());
		break;
	case props::INT:
		set_int(node.get_int());
		break;
	case props::LONG:
		set_long(node.get_long());
		break;
	case props::FLOAT:
		set_float(node.get_float());
		break;
	case props::DOUBLE:
		set_double(node.get_double());
		break;
	case props::STRING:
	case props::UNSPECIFIED:
		set_string(node.get_string());
		break;
	default:
		break;
	}
}


template<typename Itr>
GWPropertyNode::GWPropertyNode(Itr begin, Itr end, int index, GWPropertyNode * parent)
	: _index(index)
	, _name(begin, end)
	, _parent(parent)
	, _type(props::NONE)
	, _tied(false)
	, _attr(READ | WRITE)
	, _listeners(0)
{
	_local_val.string_val = 0;
	_value.val = 0;
	if (!validateName(_name))
	{
		throw std::string("plain name expected instead of '") + _name + '\'';
	}
}

GWPropertyNode::GWPropertyNode(const std::string& name, int index, GWPropertyNode * parent)
	: _index(index)
	, _name(name)
	, _parent(parent)
	, _type(props::NONE)
	, _tied(false)
	, _attr(READ | WRITE)
	, _listeners(0)
{
	_local_val.string_val = 0;
	_value.val = 0;
	if (!validateName(name))
	{
		throw std::string("plain name expected instead of '") + _name + '\'';
	}
}

GWPropertyNode::~GWPropertyNode()
{
	// zero out all parent pointers, else they might be dangling
	for (unsigned i = 0; i < _children.size(); ++i)
	{
		_children[i]->_parent = 0;
	}
	clearValue();

	if (_listeners)
	{
		vector<GWPropertyChangeListener*>::iterator it;
		for (it = _listeners->begin(); it != _listeners->end(); ++it)
		{
			(*it)->unregister_property(this);
		}
		delete _listeners;
	}
}


/// * Alias to another node.
bool GWPropertyNode::alias(GWPropertyNode * target)
{
	if (target && (_type != props::ALIAS) && (!_tied))
	{
		clearValue();
		{
			//get(target);
			_value.alias->ref();
		}
		_value.alias = target;
		_type = props::ALIAS;
		return true;
	}
	if (!target)
	{
		GW_LOG(GW_GENERAL, GW_ALERT, "Failed to create alias for " << getPath() << ". The target property does not exist.");
	}
	else if (_type == props::ALIAS)
	{
		if (_value.alias == target)
		{
			return true; // ok, identical alias requested
		}
		GW_LOG(GW_GENERAL, GW_ALERT, "Failed to create alias at " << target->getPath() << ". Source "<< getPath() << " is already aliasing another property.");
	}
	else if (_tied)
	{
		GW_LOG(GW_GENERAL, GW_ALERT, "Failed to create alias at " << target->getPath() << ". Source " << getPath() << " is a tied property.");
	}
	return false;
}

/// * Alias to another node by path.
bool GWPropertyNode::alias(const char * path)
{
	return alias(getNode(path, true));
}

/// * Remove an alias.
bool GWPropertyNode::unalias()
{
	if (_type != props::ALIAS)
	{
		return false;
	}

	clearValue();
	return true;
}

/// * Get the target of an alias.
GWPropertyNode * GWPropertyNode::getAliasTarget()
{
	return (_type == props::ALIAS ? _value.alias : 0);
}

const GWPropertyNode * GWPropertyNode::getAliasTarget() const
{
	return (_type == props::ALIAS ? _value.alias : 0);
}

/// * create a non-const child by name after the last node with the same name.
GWPropertyNode * GWPropertyNode::addChild(const char * name, int min_index, bool append)
{
	int pos = append
		? std::max(find_last_child(name, _children) + 1, min_index)
		: first_unused_index(name, _children, min_index);

	GWPropertyNodePtr node;
	node = new GWPropertyNode(name, name + strlen(name), pos, this);
	_children.push_back(node);
	fireChildAdded(node);
	return node;
}

/// * Create multiple children with unused indices
gwBase::PropertyList GWPropertyNode::addChildren(const std::string& name, size_t count, int min_index, bool append)
{
	gwBase::PropertyList nodes;
	std::set<int> used_indices;
	if (!append)
	{
		// First grab all used indices. This saves us of testing every index if it
		// is used for every element to be created
		for (size_t i = 0; i < nodes.size(); i++)
		{
			const GWPropertyNode* node = nodes[i];

			if (node->getNameString() == name && node->getIndex() >= min_index)
			{
				used_indices.insert(node->getIndex());
			}
		}
	}
	else
	{
		// If we don't want to fill the holes just find last node
		min_index = std::max(find_last_child(name.c_str(), _children) + 1, min_index);
	}

	for (int index = min_index; index < std::numeric_limits<int>::max() && nodes.size() < count; ++index)
	{
		if (used_indices.find(index) == used_indices.end())
		{
			GWPropertyNodePtr node;
			node = new GWPropertyNode(name, index, this);
			_children.push_back(node);
			fireChildAdded(node);
			nodes.push_back(node);
		}
	}

	return nodes;
}

/// * Get a non-const child by position.
GWPropertyNode * GWPropertyNode::getChild(int position)
{
	if (position >= 0 && position < nChildren())
	{
		return _children[position];
	}
	else
	{
		return 0;
	}
}

/// * Get a const child by position.
const GWPropertyNode * GWPropertyNode::getChild(int position) const
{
	if (position >= 0 && position < nChildren())
	{
		return _children[position];
	}
	else
	{
		return 0;
	}
}

unsigned int GWPropertyNode::getPosition() const
{
	if (_parent == nullptr)
	{
		return 0;
	}
	auto it = std::find(_parent->_children.begin(), _parent->_children.end(), this);
	assert(it != _parent->_children.end());
	return std::distance(_parent->_children.begin(), it);
}

/// * Get a non-const child by name and index, creating if necessary.
GWPropertyNode * GWPropertyNode::getChild(const char * name, int index, bool create)
{
	return getChildImpl(name, name + strlen(name), index, create);
}

GWPropertyNode * GWPropertyNode::getChild(const std::string& name, int index, bool create)
{
#if PROPS_STANDALONE
	const char *n = name.c_str();
	int pos = find_child(n, n + strlen(n), index, _children);
	if (pos >= 0)
	{
		return _children[pos];
#else
	GWPropertyNode* node = getExistingChild(name.begin(), name.end(), index);
	if (node)
	{
		return node;
#endif
	}
	else if (create)
	{
		GWPropertyNode* node = new GWPropertyNode(name, index, this);
		_children.push_back(node);
		fireChildAdded(node);
		return node;
	}
	else
	{
		return 0;
	}
}

/// * Get a const child by name and index.
const GWPropertyNode * GWPropertyNode::getChild(const char * name, int index) const
{
	int pos = find_child(name, name + strlen(name), index, _children);
	if (pos >= 0)
	{
		return _children[pos];
	}
	else
	{
		return 0;
	}
}

/// * Get all children with the same name (but different indices).
PropertyList GWPropertyNode::getChildren(const char * name) const
{
	PropertyList children;
	size_t max = _children.size();
	for (size_t i = 0; i < max; i++)
	{
		if (compare_strings(_children[i]->getName(), name))
		{
			children.push_back(_children[i]);
		}
	}
	sort(children.begin(), children.end(), CompareIndices());
	return children;
}

//------------------------------------------------------------------------------
bool GWPropertyNode::removeChild(GWPropertyNode* node)
{
	if (node->_parent != this)
	{
		return false;
	}

	PropertyList::iterator it = std::find(_children.begin(), _children.end(), node);
	if (it == _children.end())
	{
		return false;
	}

	eraseChild(it);
	return true;
}

//------------------------------------------------------------------------------
GWPropertyNodePtr GWPropertyNode::removeChild(int pos)
{
	if (pos < 0 || pos >= (int)_children.size())
		return GWPropertyNodePtr();

	return eraseChild(_children.begin() + pos);
}

/// * Remove a child node
GWPropertyNodePtr GWPropertyNode::removeChild(const char * name, int index)
{
	GWPropertyNodePtr ret;
	int pos = find_child(name, name + strlen(name), index, _children);
	if (pos >= 0)
		ret = removeChild(pos);
	return ret;
}

/// * Remove all children with the specified name.
PropertyList GWPropertyNode::removeChildren(const char * name)
{
	PropertyList children;
	for (int pos = static_cast<int>(_children.size() - 1); pos >= 0; pos--)
	{
		if (compare_strings(_children[pos]->getName(), name))
		{
			children.push_back(removeChild(pos));
		}
	}
	sort(children.begin(), children.end(), CompareIndices());
	return children;
}

void GWPropertyNode::removeAllChildren()
{
	for (unsigned i = 0; i < _children.size(); ++i)
	{
		GWPropertyNodePtr& node = _children[i];
		node->_parent = 0;
		node->setAttribute(REMOVED, true);
		node->clearValue();
		fireChildRemoved(node);
	}

	_children.clear();
}

std::string GWPropertyNode::getDisplayName(bool simplify) const
{
	std::string display_name = _name;
	if (_index != 0 || !simplify)
	{
		stringstream sstr;
		sstr << '[' << _index << ']';
		display_name += sstr.str();
	}
	return display_name;
}

std::string GWPropertyNode::getPath(bool simplify) const
{
	typedef std::vector<GWConstPropertyNodePtr> PList;
	PList pathList;
	for (const GWPropertyNode* node = this; node->_parent; node = node->_parent)
	{
		pathList.push_back(node);
	}
	std::string result;
	for (PList::reverse_iterator itr = pathList.rbegin(), rend = pathList.rend(); itr != rend; ++itr)
	{
		result += '/';
		result += (*itr)->getDisplayName(simplify);
	}
	return result;
}

props::Type GWPropertyNode::getType() const
{
	if (_type == props::ALIAS)
	{
		return _value.alias->getType();
	}
	else if (_type == props::EXTENDED)
	{
		return _value.val->getType();
	}
	else
	{
		return _type;
	}
}


bool GWPropertyNode::getBoolValue() const
{
	// Shortcut for common case
	if (_attr == (READ | WRITE) && _type == props::BOOL)
	{
		return get_bool();
	}
	if (getAttribute(TRACE_READ))
	{
		trace_read();
	}
	if (!getAttribute(READ))
	{
		return GWRawValue<bool>::DefaultValue();
	}
	switch (_type)
	{
	case props::ALIAS:
		return _value.alias->getBoolValue();
	case props::BOOL:
		return get_bool();
	case props::INT:
		return get_int() == 0 ? false : true;
	case props::LONG:
		return get_long() == 0L ? false : true;
	case props::FLOAT:
		return get_float() == 0.0 ? false : true;
	case props::DOUBLE:
		return get_double() == 0.0L ? false : true;
	case props::STRING:
	case props::UNSPECIFIED:
		return (compare_strings(get_string(), "true") || getDoubleValue() != 0.0L);
	case props::NONE:
	default:
		return GWRawValue<bool>::DefaultValue();
	}
}

int
GWPropertyNode::getIntValue() const
{
	// Shortcut for common case
	if (_attr == (READ | WRITE) && _type == props::INT)
		return get_int();

	if (getAttribute(TRACE_READ))
		trace_read();
	if (!getAttribute(READ))
		return GWRawValue<int>::DefaultValue();
	switch (_type) {
	case props::ALIAS:
		return _value.alias->getIntValue();
	case props::BOOL:
		return int(get_bool());
	case props::INT:
		return get_int();
	case props::LONG:
		return int(get_long());
	case props::FLOAT:
		return int(get_float());
	case props::DOUBLE:
		return int(get_double());
	case props::STRING:
	case props::UNSPECIFIED:
		return atoi(get_string());
	case props::NONE:
	default:
		return GWRawValue<int>::DefaultValue();
	}
}

long GWPropertyNode::getLongValue() const
{
	// Shortcut for common case
	if (_attr == (READ | WRITE) && _type == props::LONG)
	{
		return get_long();
	}
	if (getAttribute(TRACE_READ))
	{
		trace_read();
	}
	if (!getAttribute(READ))
	{
		return GWRawValue<long>::DefaultValue();
	}
	switch (_type)
	{
	case props::ALIAS:
		return _value.alias->getLongValue();
	case props::BOOL:
		return long(get_bool());
	case props::INT:
		return long(get_int());
	case props::LONG:
		return get_long();
	case props::FLOAT:
		return long(get_float());
	case props::DOUBLE:
		return long(get_double());
	case props::STRING:
	case props::UNSPECIFIED:
		return strtol(get_string(), 0, 0);
	case props::NONE:
	default:
		return GWRawValue<long>::DefaultValue();
	}
}

float GWPropertyNode::getFloatValue() const
{
	// Shortcut for common case
	if (_attr == (READ | WRITE) && _type == props::FLOAT)
	{
		return get_float();
	}

	if (getAttribute(TRACE_READ))
	{
		trace_read();
	}
	if (!getAttribute(READ))
	{
		return GWRawValue<float>::DefaultValue();
	}
	switch (_type) {
	case props::ALIAS:
		return _value.alias->getFloatValue();
	case props::BOOL:
		return float(get_bool());
	case props::INT:
		return float(get_int());
	case props::LONG:
		return float(get_long());
	case props::FLOAT:
		return get_float();
	case props::DOUBLE:
		return float(get_double());
	case props::STRING:
	case props::UNSPECIFIED:
		return atof(get_string());
	case props::NONE:
	default:
		return GWRawValue<float>::DefaultValue();
	}
}

double GWPropertyNode::getDoubleValue() const
{
	// Shortcut for common case
	if (_attr == (READ | WRITE) && _type == props::DOUBLE)
	{
		return get_double();
	}
	if (getAttribute(TRACE_READ))
	{
		trace_read();
	}
	if (!getAttribute(READ))
	{
		return GWRawValue<double>::DefaultValue();
	}

	switch (_type)
	{
	case props::ALIAS:
		return _value.alias->getDoubleValue();
	case props::BOOL:
		return double(get_bool());
	case props::INT:
		return double(get_int());
	case props::LONG:
		return double(get_long());
	case props::FLOAT:
		return double(get_float());
	case props::DOUBLE:
		return get_double();
	case props::STRING:
	case props::UNSPECIFIED:
		return strtod(get_string(), 0);
	case props::NONE:
	default:
		return GWRawValue<double>::DefaultValue();
	}
}

const char * GWPropertyNode::getStringValue() const
{
	// Shortcut for common case
	if (_attr == (READ | WRITE) && _type == props::STRING)
	{
		return get_string();
	}
	if (getAttribute(TRACE_READ))
	{
		trace_read();
	}
	if (!getAttribute(READ))
	{
		return GWRawValue<const char *>::DefaultValue();
	}
	return make_string();
}

bool GWPropertyNode::setBoolValue(bool value)
{
	// Shortcut for common case
	if (_attr == (READ | WRITE) && _type == props::BOOL)
		return set_bool(value);

	bool result = false;
	TEST_WRITE;
	if (_type == props::NONE || _type == props::UNSPECIFIED)
	{
		clearValue();
		_tied = false;
		_type = props::BOOL;
	}

	switch (_type)
	{
	case props::ALIAS:
		result = _value.alias->setBoolValue(value);
		break;
	case props::BOOL:
		result = set_bool(value);
		break;
	case props::INT:
		result = set_int(int(value));
		break;
	case props::LONG:
		result = set_long(long(value));
		break;
	case props::FLOAT:
		result = set_float(float(value));
		break;
	case props::DOUBLE:
		result = set_double(double(value));
		break;
	case props::STRING:
	case props::UNSPECIFIED:
		result = set_string(value ? "true" : "false");
		break;
	case props::NONE:
	default:
		break;
	}

	if (getAttribute(TRACE_WRITE))
		trace_write();
	return result;
}

bool GWPropertyNode::setIntValue(int value)
{
	// Shortcut for common case
	if (_attr == (READ | WRITE) && _type == props::INT)
		return set_int(value);

	bool result = false;
	TEST_WRITE;
	if (_type == props::NONE || _type == props::UNSPECIFIED)
	{
		clearValue();
		_type = props::INT;
		_local_val.int_val = 0;
	}

	switch (_type)
	{
	case props::ALIAS:
		result = _value.alias->setIntValue(value);
		break;
	case props::BOOL:
		result = set_bool(value == 0 ? false : true);
		break;
	case props::INT:
		result = set_int(value);
		break;
	case props::LONG:
		result = set_long(long(value));
		break;
	case props::FLOAT:
		result = set_float(float(value));
		break;
	case props::DOUBLE:
		result = set_double(double(value));
		break;
	case props::STRING:
	case props::UNSPECIFIED:
	{
		char buf[128];
		sprintf(buf, "%d", value);
		result = set_string(buf);
		break;
	}
	case props::NONE:
	default:
		break;
	}

	if (getAttribute(TRACE_WRITE))
		trace_write();
	return result;
}

bool
GWPropertyNode::setLongValue(long value)
{
	// Shortcut for common case
	if (_attr == (READ | WRITE) && _type == props::LONG)
		return set_long(value);

	bool result = false;
	TEST_WRITE;
	if (_type == props::NONE || _type == props::UNSPECIFIED)
	{
		clearValue();
		_type = props::LONG;
		_local_val.long_val = 0L;
	}

	switch (_type)
	{
	case props::ALIAS:
		result = _value.alias->setLongValue(value);
		break;
	case props::BOOL:
		result = set_bool(value == 0L ? false : true);
		break;
	case props::INT:
		result = set_int(int(value));
		break;
	case props::LONG:
		result = set_long(value);
		break;
	case props::FLOAT:
		result = set_float(float(value));
		break;
	case props::DOUBLE:
		result = set_double(double(value));
		break;
	case props::STRING:
	case props::UNSPECIFIED:
	{
		char buf[128];
		sprintf(buf, "%ld", value);
		result = set_string(buf);
		break;
	}
	case props::NONE:
	default:
		break;
	}

	if (getAttribute(TRACE_WRITE))
		trace_write();
	return result;
}

bool GWPropertyNode::setFloatValue(float value)
{
	// Shortcut for common case
	if (_attr == (READ | WRITE) && _type == props::FLOAT)
	{
		return set_float(value);
	}

	bool result = false;
	TEST_WRITE;
	if (_type == props::NONE || _type == props::UNSPECIFIED)
	{
		clearValue();
		_type = props::FLOAT;
		_local_val.float_val = 0;
	}

	switch (_type)
	{
	case props::ALIAS:
		result = _value.alias->setFloatValue(value);
		break;
	case props::BOOL:
		result = set_bool(value == 0.0 ? false : true);
		break;
	case props::INT:
		result = set_int(int(value));
		break;
	case props::LONG:
		result = set_long(long(value));
		break;
	case props::FLOAT:
		result = set_float(value);
		break;
	case props::DOUBLE:
		result = set_double(double(value));
		break;
	case props::STRING:
	case props::UNSPECIFIED:
	{
		char buf[128];
		sprintf(buf, "%f", value);
		result = set_string(buf);
		break;
	}
	case props::NONE:
	default:
		break;
	}
	if (getAttribute(TRACE_WRITE))
	{
		trace_write();
	}
	return result;
}

bool GWPropertyNode::setDoubleValue(double value)
{
	// Shortcut for common case
	if (_attr == (READ | WRITE) && _type == props::DOUBLE)
	{
		return set_double(value);
	}

	bool result = false;
	TEST_WRITE;
	if (_type == props::NONE || _type == props::UNSPECIFIED)
	{
		clearValue();
		_local_val.double_val = value;
		_type = props::DOUBLE;
	}
	switch (_type)
	{
	case props::ALIAS:
		result = _value.alias->setDoubleValue(value);
		break;
	case props::BOOL:
		result = set_bool(value == 0.0L ? false : true);
		break;
	case props::INT:
		result = set_int(int(value));
		break;
	case props::LONG:
		result = set_long(long(value));
		break;
	case props::FLOAT:
		result = set_float(float(value));
		break;
	case props::DOUBLE:
		result = set_double(value);
		break;
	case props::STRING:
	case props::UNSPECIFIED:
	{
		char buf[128];
		sprintf(buf, "%f", value);
		result = set_string(buf);
		break;
	}
	case props::NONE:
	default:
		break;
	}

	if (getAttribute(TRACE_WRITE))
	{
		trace_write();
	}
	return result;
}

bool GWPropertyNode::setStringValue(const char * value)
{
	// Shortcut for common case
	if (_attr == (READ | WRITE) && _type == props::STRING)
	{
		return set_string(value);
	}

	bool result = false;
	TEST_WRITE;
	if (_type == props::NONE || _type == props::UNSPECIFIED)
	{
		clearValue();
		_type = props::STRING;
	}

	switch (_type)
	{
	case props::ALIAS:
		result = _value.alias->setStringValue(value);
		break;
	case props::BOOL:
		result = set_bool((compare_strings(value, "true")
			|| atoi(value)) ? true : false);
		break;
	case props::INT:
		result = set_int(atoi(value));
		break;
	case props::LONG:
		result = set_long(strtol(value, 0, 0));
		break;
	case props::FLOAT:
		result = set_float(atof(value));
		break;
	case props::DOUBLE:
		result = set_double(strtod(value, 0));
		break;
	case props::STRING:
	case props::UNSPECIFIED:
		result = set_string(value);
		break;
	case props::EXTENDED:
	{
		stringstream sstr(value);
		static_cast<GWRawExtended*>(_value.val)->readFrom(sstr);
	}
	break;
	case props::NONE:
	default: break;
	}

	if (getAttribute(TRACE_WRITE))
	{
		trace_write();
	}
	return result;
}

bool GWPropertyNode::setUnspecifiedValue(const char * value)
{
	bool result = false;
	TEST_WRITE;
	if (_type == props::NONE)
	{
		clearValue();
		_type = props::UNSPECIFIED;
	}
	props::Type type = _type;
	if (type == props::EXTENDED)
	{
		type = _value.val->getType();
	}

	switch (type)
	{
	case props::ALIAS:
		result = _value.alias->setUnspecifiedValue(value);
		break;
	case props::BOOL:
		result = set_bool((compare_strings(value, "true")
			|| atoi(value)) ? true : false);
		break;
	case props::INT:
		result = set_int(atoi(value));
		break;
	case props::LONG:
		result = set_long(strtol(value, 0, 0));
		break;
	case props::FLOAT:
		result = set_float(atof(value));
		break;
	case props::DOUBLE:
		result = set_double(strtod(value, 0));
		break;
	case props::STRING:
	case props::UNSPECIFIED:
		result = set_string(value);
		break;
#if !PROPS_STANDALONE
	case props::VEC3D:
		result = static_cast<GWRawValue<osg::Vec3d>*>(_value.val)->setValue(parseString<osg::Vec3d>(value));
		break;
	case props::VEC4D:
		result = static_cast<GWRawValue<osg::Vec4d>*>(_value.val)->setValue(parseString<osg::Vec4d>(value));
		break;
#endif
	case props::NONE:
	default:
		break;
	}

	if (getAttribute(TRACE_WRITE))
	{
		trace_write();
	}
	return result;
}

//------------------------------------------------------------------------------
//#if !PROPS_STANDALONE
//bool GWPropertyNode::interpolate(const std::string& type, const GWPropertyNode& target, double duration, const std::string& easing)
//{
//	if (!_interpolation_mgr)
//	{
//		//GW_LOG(GW_GENERAL, GW_WARN, "No property interpolator available");
//		// no interpolation possible -> set to target immediately
//		setUnspecifiedValue(target.getStringValue());
//		return false;
//	}
//	return _interpolation_mgr->interpolate(this, type, target, duration, easing);
//}
//
////------------------------------------------------------------------------------
//bool GWPropertyNode::interpolate(const std::string& type,
//	const gwBase::PropertyList& values,
//	const double_list& deltas,
//	const std::string& easing)
//{
//	if (!_interpolation_mgr)
//	{
//		//GW_LOG(GW_GENERAL, GW_WARN, "No property interpolator available");
//
//		// no interpolation possible -> set to last value immediately
//		if (!values.empty())
//		{
//			setUnspecifiedValue(values.back()->getStringValue());
//		}
//		return false;
//	}
//	return _interpolation_mgr->interpolate(this, type, values, deltas, easing);
//}
//
////------------------------------------------------------------------------------
//void GWPropertyNode::setInterpolationMgr(gwBase::PropertyInterpolationMgr* mgr)
//{
//	_interpolation_mgr = mgr;
//}
//
////------------------------------------------------------------------------------
//gwBase::PropertyInterpolationMgr* GWPropertyNode::getInterpolationMgr()
//{
//	return _interpolation_mgr;
//}
//
//gwBase::PropertyInterpolationMgr* GWPropertyNode::_interpolation_mgr = 0;
//#endif

//------------------------------------------------------------------------------
std::ostream& GWPropertyNode::printOn(std::ostream& stream) const
{
	if (!getAttribute(READ)) return stream;
	switch (_type)
	{
	case props::ALIAS:
		return _value.alias->printOn(stream);
	case props::BOOL:
		stream << (get_bool() ? "true" : "false");
		break;
	case props::INT:
		stream << get_int();
		break;
	case props::LONG:
		stream << get_long();
		break;
	case props::FLOAT:
		stream << get_float();
		break;
	case props::DOUBLE:
		stream << get_double();
		break;
	case props::STRING:
	case props::UNSPECIFIED:
		stream << get_string();
		break;
	case props::EXTENDED:
		static_cast<GWRawExtended*>(_value.val)->printOn(stream);
		break;
	case props::NONE:
		break;
	default: // avoid compiler warning
		break;
	}
	return stream;
}

template<>
bool GWPropertyNode::tie(const GWRawValue<const char *> &rawValue, bool useDefault)
{
	if (_type == props::ALIAS || _tied)
	{
		return false;
	}
	useDefault = useDefault && hasValue();
	std::string old_val;
	if (useDefault)
	{
		old_val = getStringValue();
	}
	clearValue();
	_type = props::STRING;
	_tied = true;
	_value.val = rawValue.clone();
	if (useDefault)
	{
		int save_attributes = getAttributes();
		setAttribute(WRITE, true);
		setStringValue(old_val.c_str());
		setAttributes(save_attributes);
	}
	return true;
}

bool GWPropertyNode::untie()
{
	if (!_tied) return false;

	switch (_type)
	{
	case props::BOOL:
	{
		bool val = getBoolValue();
		clearValue();
		_type = props::BOOL;
		_local_val.bool_val = val;
		break;
	}
	case props::INT:
	{
		int val = getIntValue();
		clearValue();
		_type = props::INT;
		_local_val.int_val = val;
		break;
	}
	case props::LONG:
	{
		long val = getLongValue();
		clearValue();
		_type = props::LONG;
		_local_val.long_val = val;
		break;
	}
	case props::FLOAT:
	{
		float val = getFloatValue();
		clearValue();
		_type = props::FLOAT;
		_local_val.float_val = val;
		break;
	}
	case props::DOUBLE:
	{
		double val = getDoubleValue();
		clearValue();
		_type = props::DOUBLE;
		_local_val.double_val = val;
		break;
	}
	case props::STRING:
	case props::UNSPECIFIED:
	{
		std::string val = getStringValue();
		clearValue();
		_type = props::STRING;
		_local_val.string_val = copy_string(val.c_str());
		break;
	}
	case props::EXTENDED:
	{
		GWRawExtended* val = static_cast<GWRawExtended*>(_value.val);
		_value.val = 0;             // Prevent clearValue() from deleting
		clearValue();
		_type = props::EXTENDED;
		_value.val = val->makeContainer();
		delete val;
		break;
	}
	case props::NONE:
	default:
		break;
	}

	_tied = false;
	return true;
}

GWPropertyNode * GWPropertyNode::getRootNode()
{
	if (_parent == 0)
	{
		return this;
	}
	else
	{
		return _parent->getRootNode();
	}
}

const GWPropertyNode * GWPropertyNode::getRootNode() const
{
	if (_parent == 0)
	{
		return this;
	}
	else
	{
		return _parent->getRootNode();
	}
}

GWPropertyNode * GWPropertyNode::getNode(const char * relative_path, bool create)
{
#if PROPS_STANDALONE
	vector<PathComponent> components;
	parse_path(relative_path, components);
	return find_node(this, components, 0, create);

#else
	using namespace boost;

	return find_node(this, make_iterator_range(relative_path, relative_path
		+ strlen(relative_path)),
		create);
#endif
}

GWPropertyNode * GWPropertyNode::getNode(const char * relative_path, int index, bool create)
{
#if PROPS_STANDALONE
	vector<PathComponent> components;
	parse_path(relative_path, components);
	if (components.size() > 0)
		components.back().index = index;
	return find_node(this, components, 0, create);

#else
	using namespace boost;

	return find_node(this, make_iterator_range(relative_path, relative_path
		+ strlen(relative_path)),
		create, index);
#endif
}

const GWPropertyNode * GWPropertyNode::getNode(const char * relative_path) const
{
	return ((GWPropertyNode *)this)->getNode(relative_path, false);
}

const GWPropertyNode * GWPropertyNode::getNode(const char * relative_path, int index) const
{
	return ((GWPropertyNode *)this)->getNode(relative_path, index, false);
}

////////////////////////////////////////////////////////////////////////
// Convenience methods using relative paths.
////////////////////////////////////////////////////////////////////////

/// * Test whether another node has a value attached.
bool GWPropertyNode::hasValue(const char * relative_path) const
{
	const GWPropertyNode * node = getNode(relative_path);
	return (node == 0 ? false : node->hasValue());
}

/// * Get the value type for another node.
props::Type GWPropertyNode::getType(const char * relative_path) const
{
	const GWPropertyNode * node = getNode(relative_path);
	return (node == 0 ? props::UNSPECIFIED : node->getType());
}

/// * Get a bool value for another node.
bool GWPropertyNode::getBoolValue(const char * relative_path, bool defaultValue) const
{
	const GWPropertyNode * node = getNode(relative_path);
	return (node == 0 ? defaultValue : node->getBoolValue());
}

/// * Get an int value for another node.
int GWPropertyNode::getIntValue(const char * relative_path, int defaultValue) const
{
	const GWPropertyNode * node = getNode(relative_path);
	return (node == 0 ? defaultValue : node->getIntValue());
}

/// * Get a long value for another node.
long GWPropertyNode::getLongValue(const char * relative_path, long defaultValue) const
{
	const GWPropertyNode * node = getNode(relative_path);
	return (node == 0 ? defaultValue : node->getLongValue());
}

/// * Get a float value for another node.
float GWPropertyNode::getFloatValue(const char * relative_path, float defaultValue) const
{
	const GWPropertyNode * node = getNode(relative_path);
	return (node == 0 ? defaultValue : node->getFloatValue());
}

/// * Get a double value for another node.
double GWPropertyNode::getDoubleValue(const char * relative_path, double defaultValue) const
{
	const GWPropertyNode * node = getNode(relative_path);
	return (node == 0 ? defaultValue : node->getDoubleValue());
}

/// * Get a string value for another node.
const char * GWPropertyNode::getStringValue(const char * relative_path, const char * defaultValue) const
{
	const GWPropertyNode * node = getNode(relative_path);
	return (node == 0 ? defaultValue : node->getStringValue());
}

/// * Set a bool value for another node.
bool GWPropertyNode::setBoolValue(const char * relative_path, bool value)
{
	return getNode(relative_path, true)->setBoolValue(value);
}

/// * Set an int value for another node.
bool GWPropertyNode::setIntValue(const char * relative_path, int value)
{
	return getNode(relative_path, true)->setIntValue(value);
}

/// * Set a long value for another node.
bool GWPropertyNode::setLongValue(const char * relative_path, long value)
{
	return getNode(relative_path, true)->setLongValue(value);
}

/// * Set a float value for another node.
bool GWPropertyNode::setFloatValue(const char * relative_path, float value)
{
	return getNode(relative_path, true)->setFloatValue(value);
}

/// * Set a double value for another node.
bool GWPropertyNode::setDoubleValue(const char * relative_path, double value)
{
	return getNode(relative_path, true)->setDoubleValue(value);
}

/// * Set a string value for another node.
bool GWPropertyNode::setStringValue(const char * relative_path, const char * value)
{
	return getNode(relative_path, true)->setStringValue(value);
}

/// * Set an unknown value for another node.
bool GWPropertyNode::setUnspecifiedValue(const char * relative_path, const char * value)
{
	return getNode(relative_path, true)->setUnspecifiedValue(value);
}

/// * Test whether another node is tied.
bool GWPropertyNode::isTied(const char * relative_path) const
{
	const GWPropertyNode * node = getNode(relative_path);
	return (node == 0 ? false : node->isTied());
}

/// * Tie a node reached by a relative path, creating it if necessary.
bool GWPropertyNode::tie(const char * relative_path, const GWRawValue<bool> &rawValue, bool useDefault)
{
	return getNode(relative_path, true)->tie(rawValue, useDefault);
}

/// * Tie a node reached by a relative path, creating it if necessary.
bool GWPropertyNode::tie(const char * relative_path, const GWRawValue<int> &rawValue, bool useDefault)
{
	return getNode(relative_path, true)->tie(rawValue, useDefault);
}

/// * Tie a node reached by a relative path, creating it if necessary.
bool GWPropertyNode::tie(const char * relative_path, const GWRawValue<long> &rawValue, bool useDefault)
{
	return getNode(relative_path, true)->tie(rawValue, useDefault);
}

/// * Tie a node reached by a relative path, creating it if necessary.
bool GWPropertyNode::tie(const char * relative_path, const GWRawValue<float> &rawValue, bool useDefault)
{
	return getNode(relative_path, true)->tie(rawValue, useDefault);
}

/// * Tie a node reached by a relative path, creating it if necessary.
bool GWPropertyNode::tie(const char * relative_path, const GWRawValue<double> &rawValue, bool useDefault)
{
	return getNode(relative_path, true)->tie(rawValue, useDefault);
}

/// * Tie a node reached by a relative path, creating it if necessary.
bool GWPropertyNode::tie(const char * relative_path, const GWRawValue<const char *> &rawValue, bool useDefault)
{
	return getNode(relative_path, true)->tie(rawValue, useDefault);
}


///* Attempt to untie another node reached by a relative path.
bool GWPropertyNode::untie(const char * relative_path)
{
	GWPropertyNode * node = getNode(relative_path);
	return (node == 0 ? false : node->untie());
}

void GWPropertyNode::addChangeListener(GWPropertyChangeListener * listener, bool initial)
{
	if (_listeners == 0)
	{
		_listeners = new vector<GWPropertyChangeListener*>;
	}
	_listeners->push_back(listener);
	listener->register_property(this);
	if (initial)
	{
		listener->valueChanged(this);
	}
}

void GWPropertyNode::removeChangeListener(GWPropertyChangeListener * listener)
{
	if (_listeners == 0)
	{
		return;
	}

	vector<GWPropertyChangeListener*>::iterator it =
		find(_listeners->begin(), _listeners->end(), listener);
	if (it != _listeners->end())
	{
		_listeners->erase(it);
		listener->unregister_property(this);
		if (_listeners->empty())
		{
			vector<GWPropertyChangeListener*>* tmp = _listeners;
			_listeners = 0;
			delete tmp;
		}
	}
}

void GWPropertyNode::fireValueChanged()
{
	fireValueChanged(this);
}

void GWPropertyNode::fireChildAdded(GWPropertyNode * child)
{
	fireChildAdded(this, child);
}

void GWPropertyNode::fireCreatedRecursive(bool fire_self)
{
	if (fire_self)
	{
		_parent->fireChildAdded(this);
		if (_children.empty() && getType() != gwBase::props::NONE)
		{
			return fireValueChanged();
		}
	}
	for (size_t i = 0; i < _children.size(); ++i)
	{
		_children[i]->fireCreatedRecursive(true);
	}
}

void GWPropertyNode::fireChildRemoved(GWPropertyNode * child)
{
	fireChildRemoved(this, child);
}

void GWPropertyNode::fireChildrenRemovedRecursive()
{
	for (size_t i = 0; i < _children.size(); ++i)
	{
		GWPropertyNode* child = _children[i];
		fireChildRemoved(this, child);
		child->fireChildrenRemovedRecursive();
	}
}

void GWPropertyNode::fireValueChanged(GWPropertyNode * node)
{
	if (_listeners != 0)
	{
		for (unsigned int i = 0; i < _listeners->size(); i++)
		{
			(*_listeners)[i]->valueChanged(node);
		}
	}
	if (_parent != 0)
	{
		_parent->fireValueChanged(node);
	}
}

void GWPropertyNode::fireChildAdded(GWPropertyNode * parent, GWPropertyNode * child)
{
	if (_listeners != 0)
	{
		for (unsigned int i = 0; i < _listeners->size(); i++)
		{
			(*_listeners)[i]->childAdded(parent, child);
		}
	}
	if (_parent != 0)
	{
		_parent->fireChildAdded(parent, child);
	}
}

void GWPropertyNode::fireChildRemoved(GWPropertyNode * parent, GWPropertyNode * child)
{
	if (_listeners != 0)
	{
		for (unsigned int i = 0; i < _listeners->size(); i++)
		{
			(*_listeners)[i]->childRemoved(parent, child);
		}
	}
	if (_parent != 0)
	{
		_parent->fireChildRemoved(parent, child);
	}
}

//------------------------------------------------------------------------------
GWPropertyNodePtr GWPropertyNode::eraseChild(gwBase::PropertyList::iterator child)
{
	GWPropertyNodePtr node = *child;
	node->setAttribute(REMOVED, true);
	node->clearValue();
	fireChildRemoved(node);

	_children.erase(child);
	return node;
}

////////////////////////////////////////////////////////////////////////
// Implementation of GWPropertyChangeListener.
////////////////////////////////////////////////////////////////////////
GWPropertyChangeListener::GWPropertyChangeListener(bool recursive)
{
	GW_UNUSED(recursive); // for the moment, all listeners are recursive
}

GWPropertyChangeListener::~GWPropertyChangeListener()
{
	for (int i = static_cast<int>(_properties.size() - 1); i >= 0; i--)
	{
		_properties[i]->removeChangeListener(this);
	}
}

void GWPropertyChangeListener::valueChanged(GWPropertyNode * node)
{
	// NO-OP
}

void GWPropertyChangeListener::childAdded(GWPropertyNode * node, GWPropertyNode * child)
{
	// NO-OP
}

void GWPropertyChangeListener::childRemoved(GWPropertyNode * parent, GWPropertyNode * child)
{
	// NO-OP
}

void GWPropertyChangeListener::register_property(GWPropertyNode * node)
{
	_properties.push_back(node);
}

void GWPropertyChangeListener::unregister_property(GWPropertyNode * node)
{
	vector<GWPropertyNode *>::iterator it = find(_properties.begin(), _properties.end(), node);
	if (it != _properties.end())
	{
		_properties.erase(it);
	}
}

#if !PROPS_STANDALONE
template<>
std::ostream& GWRawBase<osg::Vec3d>::printOn(std::ostream& stream) const
{
	const osg::Vec3d vec = static_cast<const GWRawValue<osg::Vec3d>*>(this)->getValue();
	for (int i = 0; i < 3; ++i)
	{
		stream << vec[i];
		if (i < 2)
		{
			stream << ' ';
		}
	}
	return stream;
}

namespace gwBase
{
	template<>
	std::istream& readFrom<osg::Vec3d>(std::istream& stream, osg::Vec3d& result)
	{
		for (int i = 0; i < 3; ++i)
		{
			stream >> result[i];
		}
		return stream;
	}
}
template<>
std::ostream& GWRawBase<osg::Vec4d>::printOn(std::ostream& stream) const
{
	const osg::Vec4d vec = static_cast<const GWRawValue<osg::Vec4d>*>(this)->getValue();
	for (int i = 0; i < 4; ++i)
	{
		stream << vec[i];
		if (i < 3)
		{
			stream << ' ';
		}
	}
	return stream;
}
#endif

namespace gwBase
{
#if !PROPS_STANDALONE
	template<>
	std::istream& readFrom<osg::Vec4d>(std::istream& stream, osg::Vec4d& result)
	{
		for (int i = 0; i < 4; ++i)
		{
			stream >> result[i];
		}
		return stream;
	}
#endif

	namespace
	{
		bool compareNodeValue(const GWPropertyNode& lhs, const GWPropertyNode& rhs)
		{
			props::Type ltype = lhs.getType();
			props::Type rtype = rhs.getType();
			if (ltype != rtype)
			{
				return false;
			}

			switch (ltype)
			{
			case props::NONE:
				return true;
			case props::ALIAS:
				return false;           // XXX Should we look in aliases?
			case props::BOOL:
				return lhs.getValue<bool>() == rhs.getValue<bool>();
			case props::INT:
				return lhs.getValue<int>() == rhs.getValue<int>();
			case props::LONG:
				return lhs.getValue<long>() == rhs.getValue<long>();
			case props::FLOAT:
				return lhs.getValue<float>() == rhs.getValue<float>();
			case props::DOUBLE:
				return lhs.getValue<double>() == rhs.getValue<double>();
			case props::STRING:
			case props::UNSPECIFIED:
				return !strcmp(lhs.getStringValue(), rhs.getStringValue());
#if !PROPS_STANDALONE
			case props::VEC3D:
				return lhs.getValue<osg::Vec3d>() == rhs.getValue<osg::Vec3d>();
			case props::VEC4D:
				return lhs.getValue<osg::Vec4d>() == rhs.getValue<osg::Vec4d>();
#endif
			default:
				return false;
			}
		}
	}
}

bool GWPropertyNode::compare(const GWPropertyNode& lhs, const GWPropertyNode& rhs)
{
	if (&lhs == &rhs)
	{
		return true;
	}

	int lhsChildren = lhs.nChildren();
	int rhsChildren = rhs.nChildren();
	if (lhsChildren != rhsChildren)
	{
		return false;
	}

	if (lhsChildren == 0)
	{
		return compareNodeValue(lhs, rhs);
	}

	for (size_t i = 0; i < lhs._children.size(); ++i)
	{
		const GWPropertyNode* lchild = lhs._children[i];
		const GWPropertyNode* rchild = rhs._children[i];
		// I'm guessing that the nodes will usually be in the same
		// order.
		if (lchild->getIndex() != rchild->getIndex() || lchild->getNameString() != rchild->getNameString())
		{
			rchild = 0;
			for (PropertyList::const_iterator itr = rhs._children.begin(), end = rhs._children.end(); itr != end; ++itr)
			{
				if (lchild->getIndex() == (*itr)->getIndex() && lchild->getNameString() == (*itr)->getNameString())
				{
					rchild = *itr;
					break;
				}
			}

			if (!rchild)
			{
				return false;
			}
		}
		if (!compare(*lchild, *rchild))
		{
			return false;
		}
	}
	return true;
}

struct PropertyPlaceLess
{
	typedef bool result_type;
	bool operator()(GWPropertyNodePtr lhs, GWPropertyNodePtr rhs) const
	{
		int comp = lhs->getNameString().compare(rhs->getNameString());
		if (comp == 0)
		{
			return lhs->getIndex() < rhs->getIndex();
		}
		else
		{
			return comp < 0;
		}
	}
};

#if !PROPS_STANDALONE
size_t hash_value(const GWPropertyNode& node)
{
	using namespace boost;
	if (node.nChildren() == 0)
	{
		switch (node.getType())
		{
		case props::NONE:
			return 0;
		case props::BOOL:
			return hash_value(node.getValue<bool>());
		case props::INT:
			return hash_value(node.getValue<int>());
		case props::LONG:
			return hash_value(node.getValue<long>());
		case props::FLOAT:
			return hash_value(node.getValue<float>());
		case props::DOUBLE:
			return hash_value(node.getValue<double>());
		case props::STRING:
		case props::UNSPECIFIED:
		{
			const char *val = node.getStringValue();
			return hash_range(val, val + strlen(val));
		}
		case props::VEC3D:
		{
			const osg::Vec3d val = node.getValue<osg::Vec3d>();
			return hash_range(&val.ptr()[0], &val.ptr()[3]);
		}
		case props::VEC4D:
		{
			const osg::Vec4d val = node.getValue<osg::Vec4d>();
			return hash_range(&val.ptr()[0], &val.ptr()[4]);
		}
		case props::ALIAS:      // XXX Should we look in aliases?
		default:
			return 0;
		}
	}
	else
	{
		size_t seed = 0;
		PropertyList children(node._children.begin(), node._children.end());
		sort(children.begin(), children.end(), PropertyPlaceLess());
		for (PropertyList::const_iterator itr = children.begin(), end = children.end(); itr != end; ++itr)
		{
			hash_combine(seed, (*itr)->_name);
			hash_combine(seed, (*itr)->_index);
			hash_combine(seed, hash_value(**itr));
		}
		return seed;
	}
}
#endif

// end of props.cxx
