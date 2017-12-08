#ifndef GWBASE_PROPS_H
#define GWBASE_PROPS_H 1

#ifndef PROPS_STANDALONE
#define PROPS_STANDALONE 0
#endif

#include <compiler.h>
#include <gwBase/export.h>
#include <osg/Vec3d>
#include <osg/Vec4d>
#include <osg/ref_ptr>
#include <osg/Referenced>

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <typeinfo>

typedef std::vector<double> double_list;

#if PROPS_STANDALONE
// taken from: boost/utility/enable_if.hpp
#ifndef GW_LOG
# define GW_GENERAL	0
# define GW_ALERT	0
# define GW_WARN	1
# define GW_LOG(type, level, message) (type) ? (std::cerr <<message << endl) : (std::cout <<message << endl)
#endif
namespace boost
{
	template <bool B, class T = void>
	struct enable_if_c
	{
		typedef T type;
	};

	template <class T>
	struct enable_if_c<false, T> {};

	template <class Cond, class T = void>
	struct enable_if : public enable_if_c<Cond::value, T> {};

	template <bool B, class T = void>
	struct disable_if_c
	{
		typedef T type;
	};

	template <class T>
	struct disable_if_c<true, T> {};

	template <class Cond, class T = void>
	struct disable_if : public disable_if_c<Cond::value, T> {};
}

#else
# include <boost/utility.hpp>
# include <boost/type_traits/is_enum.hpp>
# include <gwBase/LogStream.h>
#endif

using namespace std;

namespace gwBase
{
	//class PropertyInterpolationMgr;
	template<typename T>
	std::istream& readFrom(std::istream& stream, T& result)
	{
		stream >> result;
		return stream;
	}

	/**
	 * Parse a string as an object of a given type.
	 * XXX no error behavior yet.
	 *
	 * @tparam T the return type
	 * @param str the string
	 * @return the object.
	 */
	template<typename T>
	inline T parseString(const std::string& str)
	{
		std::istringstream stream(str);
		T result;
		readFrom(stream, result);
		return result;
	}

	/// * Property value types.
#ifdef NONE
#pragma warn A sloppy coder has defined NONE as a macro!
#undef NONE
#endif

#ifdef ALIAS
#pragma warn A sloppy coder has defined ALIAS as a macro!
#undef ALIAS
#endif

#ifdef UNSPECIFIED
#pragma warn A sloppy coder has defined UNSPECIFIED as a macro!
#undef UNSPECIFIED
#endif

#ifdef BOOL
#pragma warn A sloppy coder has defined BOOL as a macro!
#undef BOOL
#endif

#ifdef INT
#pragma warn A sloppy coder has defined INT as a macro!
#undef INT
#endif

#ifdef LONG
#pragma warn A sloppy coder has defined LONG as a macro!
#undef LONG
#endif

#ifdef FLOAT
#pragma warn A sloppy coder has defined FLOAT as a macro!
#undef FLOAT
#endif

#ifdef DOUBLE
#pragma warn A sloppy coder has defined DOUBLE as a macro!
#undef DOUBLE
#endif

#ifdef STRING
#pragma warn A sloppy coder has defined STRING as a macro!
#undef STRING
#endif

	namespace props
	{
		/**
		 * The possible types of an GWPropertyNode. Types that appear after
		 * EXTENDED are not stored in the GWPropertyNode itself.
		 */
		enum Type
		{
			NONE = 0, /**< The node hasn't been assigned a value yet. */
			ALIAS, /**< The node "points" to another node. */
			BOOL,
			INT,
			LONG,
			FLOAT,
			DOUBLE,
			STRING,
			UNSPECIFIED,
			EXTENDED, /**< The node's value is not stored in the property;
					   * the actual value and type is retrieved from an
					   * GWRawValue node. This type is never returned by @see
					   * GWPropertyNode::getType.
					   */
					   // Extended properties
					   VEC3D,
					   VEC4D
		};

		template<typename T> struct PropertyTraits;

#define DEFINTERNALPROP(TYPE, PROP) \
template<> \
struct PropertyTraits<TYPE> \
{ \
    static const Type type_tag = PROP; \
    enum  { Internal = 1 }; \
};

		DEFINTERNALPROP(bool, BOOL);
		DEFINTERNALPROP(int, INT);
		DEFINTERNALPROP(long, LONG);
		DEFINTERNALPROP(float, FLOAT);
		DEFINTERNALPROP(double, DOUBLE);
		DEFINTERNALPROP(const char *, STRING);
		DEFINTERNALPROP(const char[], STRING);

		template<>
		struct PropertyTraits<osg::Vec3d>
		{
			static const Type type_tag = VEC3D;
			enum { Internal = 0 };
		};

		template<>
		struct PropertyTraits<osg::Vec4d>
		{
			static const Type type_tag = VEC4D;
			enum { Internal = 0 };
		};
#undef DEFINTERNALPROP

	}

	// Extended properties
	template<>
	std::istream& readFrom</*SGVec3d*/osg::Vec3d>(std::istream& stream, /*SGVec3d*/osg::Vec3d& result);
	template<>
	std::istream& readFrom</*SGVec4d*/osg::Vec4d>(std::istream& stream, /*SGVec4d*/osg::Vec4d& result);
}

////////////////////////////////////////////////////////////////////////
// A raw value.
//
// This is the mechanism that information-providing routines can
// use to link their own values to the property manager.  Any
// SGValue can be tied to a raw value and then untied again.
//
// Note: we are forced to use inlined methods here to ensure
// that the templates will be instantiated.  We're probably taking
// a small performance hit for that.
////////////////////////////////////////////////////////////////////////

/**
 * Base class for GWRawValue classes that holds no type
 * information. This allows some generic manipulation of the
 * GWRawValue object.
 */
class GWRaw
{
public:
	/**
	 * Get the type enumeration for the raw value.
	 *
	 * @return the type.
	 */
	virtual gwBase::props::Type getType() const = 0;
	virtual ~GWRaw() {}

	/**
	 * Create a new deep copy of this raw value.
	 *
	 * The copy will contain its own version of the underlying value
	 * as well, and will be the same type.
	 *
	 * @return A deep copy of the current object.
	 */
	virtual GWRaw* clone() const = 0;

};

class GWRawExtended : public GWRaw
{
public:
	/**
	 * Make an GWRawValueContainer from the GWRawValue.
	 *
	 * This is a virtual function of GWRawExtended so that
	 * GWPropertyNode::untie doesn't need to know the type of an
	 * extended property.
	 */
	virtual GWRawExtended* makeContainer() const = 0;
	/**
	 * Write value out to a stream
	 */
	virtual std::ostream& printOn(std::ostream& stream) const = 0;
	/**
	 * Read value from a stream and store it.
	 */
	virtual std::istream& readFrom(std::istream& stream) = 0;
};

// Choose between different base classes based on whether the value is
// stored internal to the property node. This frees us from defining
// the virtual functions in the GWRawExtended interface where they
// don't make sense, e.g. readFrom for the const char* type.
template<typename T, int internal = gwBase::props::PropertyTraits<T>::Internal>
class GWRawBase;

template<typename T>
class GWRawBase<T, 1> : public GWRaw
{
};

template<typename T>
class GWRawBase<T, 0> : public GWRawExtended
{
	virtual GWRawExtended* makeContainer() const;
	virtual std::ostream& printOn(std::ostream& stream) const;
	virtual std::istream& readFrom(std::istream& stream);
};

/**
 * Abstract base class for a raw value.
 *
 * The property manager is implemented in two layers. The GWPropertyNode is the
 * highest and most abstract layer, representing an LValue/RValue pair: it
 * records the position of the property in the property tree and contains
 * facilities for navigation to other nodes. It is guaranteed to be persistent:
 * the GWPropertyNode will not change during a session, even if the property is
 * bound and unbound multiple times.
 *
 * When the property value is not managed internally in the
 * GWPropertyNode, the GWPropertyNode will contain a reference to an
 * GWRawValue (this class), which provides an abstract way to get,
 * set, and clone the underlying value.  The GWRawValue may change
 * frequently during a session as a value is retyped or bound and
 * unbound to various data source, but the abstract GWPropertyNode
 * layer insulates the application from those changes.
 *
 * The GWPropertyNode class always keeps a *copy* of a raw value, not the
 * original one passed to it; if you override a derived class but do not replace
 * the {@link GWRaw::clone clone()} method, strange things will happen.
 *
 * All derived GWRawValue classes must implement getValue(), setValue(), and
 * {@link GWRaw::clone clone()} for the appropriate type.
 *
 * @see GWPropertyNode
 * @see GWRawValuePointer
 * @see GWRawValueFunctions
 * @see GWRawValueFunctionsIndexed
 * @see GWRawValueMethods
 * @see GWRawValueMethodsIndexed
 * @see GWRawValueContainer
 */
template <class T>
class GWRawValue : public GWRawBase<T>
{
public:
	/**
	 * The default underlying value for this type.
	 *
	 * Every raw value has a default; the default is false for a
	 * boolean, 0 for the various numeric values, and "" for a string.
	 * If additional types of raw values are added in the future, they
	 * may need different kinds of default values (such as epoch for a
	 * date type).  The default value is used when creating new values.
	 */
	static T DefaultValue()
	{
		return T();
	}

	GWRawValue() {}
	virtual ~GWRawValue() {}


	/**
	 * Return the underlying value.
	 *
	 * @return The actual value for the property.
	 * @see #setValue
	 */
	virtual T getValue() const = 0;

	/**
	 * Assign a new underlying value.
	 *
	 * If the new value cannot be set (because this is a read-only
	 * raw value, or because the new value is not acceptable for
	 * some reason) this method returns false and leaves the original
	 * value unchanged.
	 *
	 * @param value The actual value for the property.
	 * @return true if the value was set successfully, false otherwise.
	 * @see #getValue
	 */
	virtual bool setValue(T value) = 0;

	/// * Return the type tag for this raw value type.
	virtual gwBase::props::Type getType() const
	{
		return gwBase::props::PropertyTraits<T>::type_tag;
	}
};



////////////////////////////////////////////////////////////////////////
// Default values for every type.
////////////////////////////////////////////////////////////////////////
template<> inline bool GWRawValue<bool>::DefaultValue()
{
	return false;
}

template<> inline const char * GWRawValue<const char *>::DefaultValue()
{
	return "";
}

/**
 * A raw value bound to a pointer.
 *
 * This is the most efficient way to tie an external value, but also
 * the most dangerous, because there is no way for the supplier to
 * perform bounds checking and derived calculations except by polling
 * the variable to see if it has changed.  There is no default
 * constructor, because this class would be meaningless without a
 * pointer.
 */
template <class T>
class GWRawValuePointer : public GWRawValue<T>
{
public:
	/**
	 * Explicit pointer constructor.
	 *
	 * Create a new raw value bound to the value of the variable
	 * referenced by the pointer.
	 *
	 * @param ptr The pointer to the variable to which this raw value
	 * will be bound.
	 */
	GWRawValuePointer(T * ptr) : _ptr(ptr) {}
	virtual ~GWRawValuePointer() {}

	/**
	 * Get the underlying value.
	 *
	 * This method will dereference the pointer and return the
	 * variable's value.
	 */
	virtual T getValue() const { return *_ptr; }

	/**
	 * Set the underlying value.
	 *
	 * This method will dereference the pointer and change the
	 * variable's value.
	 */
	virtual bool setValue(T value) { *_ptr = value; return true; }

	/**
	 * Create a copy of this raw value.
	 *
	 * The copy will use the same external pointer as the original.
	 */
	virtual GWRaw* clone() const
	{
		return new GWRawValuePointer(_ptr);
	}

private:
	T * _ptr;
};


/**
 * A value managed through static functions.
 *
 * A read-only value will not have a setter; a write-only value will
 * not have a getter.
 */
template <class T>
class GWRawValueFunctions : public GWRawValue<T>
{
public:
	/// * The template type of a static getter function.
	typedef T(*getter_t)();

	/// * The template type of a static setter function.
	typedef void(*setter_t)(T);

	/**
	 * Explicit constructor.
	 *
	 * Create a new raw value bound to the getter and setter supplied.
	 *
	 * @param getter A static function for getting a value, or 0
	 * to read-disable the value.
	 * @param setter A static function for setting a value, or 0
	 * to write-disable the value.
	 */
	GWRawValueFunctions(getter_t getter = 0, setter_t setter = 0)
		: _getter(getter)
		, _setter(setter)
	{}

	virtual ~GWRawValueFunctions() {}

	/**
	 * Get the underlying value.
	 *
	 * This method will invoke the getter function to get a value.
	 * If no getter function was supplied, this method will always
	 * return the default value for the type.
	 */
	virtual T getValue() const
	{
		if (_getter)
		{
			return (*_getter)();
		}
		else
		{
			return GWRawValue<T>::DefaultValue();
		}
	}

	/**
	 * Set the underlying value.
	 *
	 * This method will invoke the setter function to change the
	 * underlying value.  If no setter function was supplied, this
	 * method will return false.
	 */
	virtual bool setValue(T value)
	{
		if (_setter)
		{
			(*_setter)(value);
			return true;
		}
		else
		{
			return false;
		}
	}

	/// * Create a copy of this raw value, bound to the same functions.
	virtual GWRaw* clone() const
	{
		return new GWRawValueFunctions(_getter, _setter);
	}

private:
	getter_t _getter;
	setter_t _setter;
};


/**
 * An indexed value bound to static functions.
 *
 * A read-only value will not have a setter; a write-only value will
 * not have a getter.  An indexed value is useful for binding one
 * of a list of possible values (such as multiple engines for a
 * plane).  The index is hard-coded at creation time.
 *
 * @see GWRawValue
 */
template <class T>
class GWRawValueFunctionsIndexed : public GWRawValue<T>
{
public:
	typedef T(*getter_t)(int);
	typedef void(*setter_t)(int, T);
	GWRawValueFunctionsIndexed(int index, getter_t getter = 0, setter_t setter = 0)
		: _index(index)
		, _getter(getter)
		, _setter(setter)
	{}
	virtual ~GWRawValueFunctionsIndexed() {}
	virtual T getValue() const
	{
		if (_getter)
		{
			return (*_getter)(_index);
		}
		else
		{
			return GWRawValue<T>::DefaultValue();
		}
	}
	virtual bool setValue(T value)
	{
		if (_setter)
		{
			(*_setter)(_index, value);
			return true;
		}
		else
		{
			return false;
		}
	}
	virtual GWRaw* clone() const
	{
		return new GWRawValueFunctionsIndexed(_index, _getter, _setter);
	}

private:
	int _index;
	getter_t _getter;
	setter_t _setter;
};


/**
 * A value managed through an object and access methods.
 *
 * A read-only value will not have a setter; a write-only value will
 * not have a getter.
 */
template <class C, class T>
class GWRawValueMethods : public GWRawValue<T>
{
public:
	typedef T(C::*getter_t)() const;
	typedef void (C::*setter_t)(T);
	GWRawValueMethods(C &obj, getter_t getter = 0, setter_t setter = 0)
		: _obj(obj)
		, _getter(getter)
		, _setter(setter)
	{}

	virtual ~GWRawValueMethods() {}
	virtual T getValue() const
	{
		if (_getter)
		{
			return (_obj.*_getter)();
		}
		else
		{
			return GWRawValue<T>::DefaultValue();
		}
	}
	virtual bool setValue(T value)
	{
		if (_setter)
		{
			(_obj.*_setter)(value);
			return true;
		}
		else
		{
			return false;
		}
	}
	virtual GWRaw* clone() const
	{
		return new GWRawValueMethods(_obj, _getter, _setter);
	}

private:
	C &_obj;
	getter_t _getter;
	setter_t _setter;
};


/**
 * An indexed value managed through an object and access methods.
 *
 * A read-only value will not have a setter; a write-only value will
 * not have a getter.
 */
template <class C, class T>
class GWRawValueMethodsIndexed : public GWRawValue<T>
{
public:
	typedef T(C::*getter_t)(int) const;
	typedef void (C::*setter_t)(int, T);
	GWRawValueMethodsIndexed(C &obj, int index, getter_t getter = 0, setter_t setter = 0)
		: _obj(obj)
		, _index(index)
		, _getter(getter)
		, _setter(setter)
	{}

	virtual ~GWRawValueMethodsIndexed() {}
	virtual T getValue() const
	{
		if (_getter)
		{
			return (_obj.*_getter)(_index);
		}
		else
		{
			return GWRawValue<T>::DefaultValue();
		}
	}
	virtual bool setValue(T value)
	{
		if (_setter)
		{
			(_obj.*_setter)(_index, value);
			return true;
		}
		else
		{
			return false;
		}
	}
	virtual GWRaw* clone() const 
	{
		return new GWRawValueMethodsIndexed(_obj, _index, _getter, _setter);
	}

private:
	C &_obj;
	int _index;
	getter_t _getter;
	setter_t _setter;
};

/**
 * A raw value that contains its value. This provides a way for
 * property nodes to contain values that shouldn't be stored in the
 * property node itself.
 */
template <class T>
class GWRawValueContainer : public GWRawValue<T>
{
public:
	/// * Explicit constructor.
	GWRawValueContainer(const T& obj) : _obj(obj) {}

	virtual ~GWRawValueContainer() {}

	/// * Get the underlying value.
	virtual T getValue() const { return _obj; }

	/**
	 * Set the underlying value.
	 *
	 * This method will dereference the pointer and change the
	 * variable's value.
	 */
	virtual bool setValue(T value) { _obj = value; return true; }

	/// * Create a copy of this raw value.
	virtual GWRaw* clone() const
	{
		return new GWRawValueContainer(_obj);
	}

private:
	T _obj;
};

template<typename T>
GWRawExtended* GWRawBase<T, 0>::makeContainer() const
{
	return new GWRawValueContainer<T>(static_cast<const GWRawValue<T>*>(this)->getValue());
}

template<typename T>
std::ostream& GWRawBase<T, 0>::printOn(std::ostream& stream) const
{
	return stream << static_cast<GWRawValue<T>*>(this)->getValue();
}

template<typename T>
std::istream& GWRawBase<T, 0>::readFrom(std::istream& stream)
{
	T value;
	gwBase::readFrom(stream, value);
	static_cast<GWRawValue<T>*>(this)->setValue(value);
	return stream;
}

template<>
std::ostream& GWRawBase</*SGVec3d*/osg::Vec3d>::printOn(std::ostream& stream) const;

template<>
std::ostream& GWRawBase</*SGVec4d*/osg::Vec4d>::printOn(std::ostream& stream) const;


 ///* The smart pointer that manage reference counting*/
class GWPropertyNode;
typedef /*SGSharedPtr*/osg::ref_ptr<GWPropertyNode> GWPropertyNodePtr;
typedef /*SGSharedPtr*/osg::ref_ptr<const GWPropertyNode> GWConstPropertyNodePtr;

namespace gwBase
{
	typedef std::vector<GWPropertyNodePtr> PropertyList;
}

/**
 * The property change listener interface.
 *
 * <p>Any class that needs to listen for property changes must implement
 * this interface.</p>
 */
class GW_BASE_EXPORT GWPropertyChangeListener
{
public:
	virtual ~GWPropertyChangeListener();

	/// Called if value of \a node has changed.
	virtual void valueChanged(GWPropertyNode * node);

	/// Called if \a child has been added to the given \a parent.
	virtual void childAdded(GWPropertyNode * parent, GWPropertyNode * child);

	/// Called if \a child has been removed from its \a parent.
	virtual void childRemoved(GWPropertyNode * parent, GWPropertyNode * child);

protected:
	GWPropertyChangeListener(bool recursive = false);
	friend class GWPropertyNode;
	virtual void register_property(GWPropertyNode * node);
	virtual void unregister_property(GWPropertyNode * node);

private:
	std::vector<GWPropertyNode *> _properties;
};

/************************************************************************/
/*            A node in a property tree.                                */
/************************************************************************/
class GW_BASE_EXPORT GWPropertyNode : public osg::Referenced
{
public:
	enum
	{
		MAX_STRING_LEN = 1024
	};

	/**
	 * Access mode attributes.
	 *
	 * <p>The ARCHIVE attribute is strictly advisory, and controls
	 * whether the property should normally be saved and restored.</p>
	 */
	enum Attribute
	{
		NO_ATTR = 0,
		READ = 1,
		WRITE = 2,
		ARCHIVE = 4,
		REMOVED = 8,
		TRACE_READ = 16,
		TRACE_WRITE = 32,
		USERARCHIVE = 64,
		PRESERVE = 128
		// beware: if you add another attribute here,
		// also update value of "LAST_USED_ATTRIBUTE".
	};


	/**
	 * Last used attribute
	 * Update as needed when enum Attribute is changed
	 */
	static const int LAST_USED_ATTRIBUTE;

	GWPropertyNode();
	GWPropertyNode(const GWPropertyNode &node);
	virtual ~GWPropertyNode();

	//
	// Basic properties.
	//

	/// * Test whether this node contains a primitive leaf value.
	bool hasValue() const { return (_type != gwBase::props::NONE); }

	/// * Get the node's simple (XML) name.
	const char * getName() const { return _name.c_str(); }

	/// * Get the node's simple name as a string.
	const std::string& getNameString() const { return _name; }

	/// * Get the node's pretty display name, with subscript when needed.
	std::string getDisplayName(bool simplify = false) const;

	/// * Get the node's integer index.
	int getIndex() const { return _index; }

	/// * Get a non-const pointer to the node's parent.
	GWPropertyNode * getParent() { return _parent; }

	/// * Get a const pointer to the node's parent.
	const GWPropertyNode * getParent() const { return _parent; }

	/// * Get node's position in parent (*NOT* index)
	unsigned int getPosition() const;

	//
	// Children.
	//

	/// * Get the number of child nodes.
	int nChildren() const { return (int)_children.size(); }

	/// * Get a child node by position (*NOT* index).
	GWPropertyNode * getChild(int position);

	/// * Get a const child node by position (*NOT* index).
	const GWPropertyNode * getChild(int position) const;

	///* Test whether a named child exists.
	bool hasChild(const char * name, int index = 0) const
	{
		return (getChild(name, index) != 0);
	}

	/// * Test whether a named child exists.
	bool hasChild(const std::string& name, int index = 0) const
	{
		return (getChild(name, index) != 0);
	}

	/**
	 * Create a new child node with the given name and an unused index
	 *
	 * @param min_index Minimal index for new node (skips lower indices)
	 * @param append    Whether to simply use the index after the last used index
	 *                  or use a lower, unused index if it exists
	 */
	GWPropertyNode * addChild(const char* name, int min_index = 0, bool append = true);
	GWPropertyNode * addChild(const std::string& name, int min_index = 0, bool append = true)
	{
		return addChild(name.c_str(), min_index, append);
	}

	/**
	 * Create multiple child nodes with the given name an unused indices
	 *
	 * @param count     The number of nodes create
	 * @param min_index Minimal index for new nodes (skips lower indices)
	 * @param append    Whether to simply use the index after the last used index
	 *                  or use a lower, unused index if it exists
	 */
	gwBase::PropertyList addChildren(const std::string& name, size_t count, int min_index = 0, bool append = true);

	/// * Get a child node by name and index.
	GWPropertyNode * getChild(const char* name, int index = 0, bool create = false);
	GWPropertyNode * getChild(const std::string& name, int index = 0, bool create = false);

	///* Get a const child node by name and index.
	const GWPropertyNode * getChild(const char * name, int index = 0) const;

	///* Get a const child node by name and index.
	const GWPropertyNode * getChild(const std::string& name, int index = 0) const
	{
		return getChild(name.c_str(), index);
	}

	/// * Get a vector of all children with the specified name.
	gwBase::PropertyList getChildren(const char * name) const;

	/// * Get a vector of all children with the specified name.
	gwBase::PropertyList getChildren(const std::string& name) const
	{
		return getChildren(name.c_str());
	}

	/**
	 * Remove child by pointer (if it is a child of this node).
	 *
	 * @return true, if the node was deleted.
	 */
	bool removeChild(GWPropertyNode* node);

	// TODO do we need the removeXXX methods to return the deleted nodes?
	/// * Remove child by position.
	GWPropertyNodePtr removeChild(int pos);

	///Remove a child node
	GWPropertyNodePtr removeChild(const char * name, int index = 0);

	///* Remove a child node
	GWPropertyNodePtr removeChild(const std::string& name, int index = 0)
	{
		return removeChild(name.c_str(), index);
	}

	///* Remove all children with the specified name.
	gwBase::PropertyList removeChildren(const char * name);

	///* Remove all children with the specified name.
	gwBase::PropertyList removeChildren(const std::string& name)
	{
		return removeChildren(name.c_str());
	}

	/// * Remove all children (does not change the value of the node)
	void removeAllChildren();

	//
	// Alias support.
	//
	/// * Alias this node's leaf value to another's.
	bool alias(GWPropertyNode * target);

	///* Alias this node's leaf value to another's by relative path.
	bool alias(const char * path);

	/// * Alias this node's leaf value to another's by relative path.
	bool alias(const std::string& path)
	{
		return alias(path.c_str());
	}

	/// * Remove any alias for this node.
	bool unalias();

	///* Test whether the node's leaf value is aliased to another's.
	bool isAlias() const { return (_type == gwBase::props::ALIAS); }

	/// * Get a non-const pointer to the current alias target, if any.
	GWPropertyNode * getAliasTarget();

	/// * Get a const pointer to the current alias target, if any.
	const GWPropertyNode * getAliasTarget() const;

	//
	// Path information.
	//
	/// * Get the path to this node from the root.
	std::string getPath(bool simplify = false) const;

	///* Get a pointer to the root node.
	GWPropertyNode * getRootNode();

	/// * Get a const pointer to the root node.
	const GWPropertyNode * getRootNode() const;

	/// * Get a pointer to another node by relative path.
	GWPropertyNode * getNode(const char * relative_path, bool create = false);

	///* Get a pointer to another node by relative path.
	GWPropertyNode * getNode(const std::string& relative_path, bool create = false)
	{
		return getNode(relative_path.c_str(), create);
	}

	/**
	 * Get a pointer to another node by relative path.
	 *
	 * This method leaves the index off the last member of the path,
	 * so that the user can specify it separately (and save some
	 * string building).  For example, getNode("/bar[1]/foo", 3) is
	 * exactly equivalent to getNode("bar[1]/foo[3]").  The index
	 * provided overrides any given in the path itself for the last
	 * component.
	 */
	GWPropertyNode * getNode(const char * relative_path, int index, bool create = false);

	/**
	 * Get a pointer to another node by relative path.
	 *
	 * This method leaves the index off the last member of the path,
	 * so that the user can specify it separately (and save some
	 * string building).  For example, getNode("/bar[1]/foo", 3) is
	 * exactly equivalent to getNode("bar[1]/foo[3]").  The index
	 * provided overrides any given in the path itself for the last
	 * component.
	 */
	GWPropertyNode * getNode(const std::string& relative_path, int index, bool create = false)
	{
		return getNode(relative_path.c_str(), index, create);
	}

	/// * Get a const pointer to another node by relative path.
	const GWPropertyNode * getNode(const char * relative_path) const;

	/// * Get a const pointer to another node by relative path.
	const GWPropertyNode * getNode(const std::string& relative_path) const
	{
		return getNode(relative_path.c_str());
	}

	/**
	 * Get a const pointer to another node by relative path.
	 *
	 * This method leaves the index off the last member of the path,
	 * so that the user can specify it separate.
	 */
	const GWPropertyNode * getNode(const char * relative_path, int index) const;

	/**
	 * Get a const pointer to another node by relative path.
	 *
	 * This method leaves the index off the last member of the path,
	 * so that the user can specify it separate.
	 */
	const GWPropertyNode * getNode(const std::string& relative_path, int index) const
	{
		return getNode(relative_path.c_str(), index);
	}

	//
	// Access Mode.
	//
	///* Check a single mode attribute for the property node.
	bool getAttribute(Attribute attr) const
	{
		return ((_attr & attr) != 0);
	}

	/// * Set a single mode attribute for the property node.
	void setAttribute(Attribute attr, bool state)
	{
		(state ? _attr |= attr : _attr &= ~attr);
	}

	/// * Get all of the mode attributes for the property node.
	int getAttributes() const { return _attr; }

	/// * Set all of the mode attributes for the property node.
	void setAttributes(int attr) { _attr = attr; }

	//
	// Leaf Value (primitive).
	//
	//
	/// * Get the type of leaf value, if any, for this node.
	gwBase::props::Type getType() const;

	///* Get a bool value for this node.
	bool getBoolValue() const;

	/// * Get an int value for this node.
	int getIntValue() const;

	/// * Get a long int value for this node.
	long getLongValue() const;

	/// * Get a float value for this node.
	float getFloatValue() const;

	/// * Get a double value for this node.
	double getDoubleValue() const;

	/// * Get a string value for this node.
	const char * getStringValue() const;

	/**
	 * Get a value from a node. If the actual type of the node doesn't
	 * match the desired type, a conversion isn't guaranteed.
	 */
	template<typename T>
	T getValue(typename boost::enable_if_c<gwBase::props::PropertyTraits<T>::Internal>::type* dummy = 0) const;
	// Getter for extended property
	template<typename T>
	T getValue(typename boost::disable_if_c<gwBase::props::PropertyTraits<T>::Internal>::type* dummy = 0) const;


	///* Get a list of values from all children with the given name
	template<typename T, typename T_get /* = T */> // TODO use C++11 or traits
	std::vector<T> getChildValues(const std::string& name) const;

	/// * Get a list of values from all children with the given name
	template<typename T>
	std::vector<T> getChildValues(const std::string& name) const;

	///* Set a bool value for this node.
	bool setBoolValue(bool value);

	/// * Set an int value for this node.
	bool setIntValue(int value);

	/// * Set a long int value for this node.
	bool setLongValue(long value);

	/// * Set a float value for this node.
	bool setFloatValue(float value);

	/// * Set a double value for this node.
	bool setDoubleValue(double value);

	/// * Set a string value for this node.
	bool setStringValue(const char * value);

	/// * Set a string value for this node.
	bool setStringValue(const std::string& value)
	{
		return setStringValue(value.c_str());
	}

	/// * Set a value of unspecified type for this node.
	bool setUnspecifiedValue(const char * value);

	template<typename T>
	bool setValue(const T& val, typename boost::enable_if_c<gwBase::props::PropertyTraits<T>::Internal>::type* dummy = 0);

	template<typename T>
	bool setValue(const T& val, typename boost::disable_if_c<gwBase::props::PropertyTraits<T>::Internal>::type* dummy = 0);

	template<int N>
	bool setValue(const char(&val)[N])
	{
		return setValue(&val[0]);
	}

	/**
	 * Set relative node to given value and afterwards make read only.
	 *
	 * @param relative_path   Path to node
	 * @param value           Value to set
	 *
	 * @return whether value could be set
	 */
	template<typename T>
	bool setValueReadOnly(const std::string& relative_path, const T& value)
	{
		GWPropertyNode* node = getNode(relative_path, true);
		bool ret = node->setValue(value);
		node->setAttributes(READ);
		return ret;
	}

//#if !PROPS_STANDALONE
//	/**
//	 * Interpolate current value to target value within given time.
//	 *
//	 * @param type        Type of interpolation ("numeric", "color", etc.)
//	 * @param target      Node containing target value
//	 * @param duration    Duration of interpolation (in seconds)
//	 * @param easing      Easing function (http://easings.net/)
//	 */
//	bool interpolate(const std::string& type, const GWPropertyNode& target,
//		double duration = 0.6, const std::string& easing = "swing");
//
//	/**
//	 * Interpolate current value to a series of values within given durations.
//	 *
//	 * @param type        Type of interpolation ("numeric", "color", etc.)
//	 * @param values      Nodes containing intermediate and target values
//	 * @param deltas      Durations for each interpolation step (in seconds)
//	 * @param easing      Easing function (http://easings.net/)
//	 */
//	bool interpolate(const std::string& type, const gwBase::PropertyList& values,
//		const double_list& deltas, const std::string& easing = "swing");
//
//	/// * Set the interpolation manager used by the interpolate methods.
//	static void setInterpolationMgr(gwBase::PropertyInterpolationMgr* mgr);
//
//	///* Get the interpolation manager
//	static gwBase::PropertyInterpolationMgr* getInterpolationMgr();
//#endif

	/// * Print the value of the property to a stream.
	std::ostream& printOn(std::ostream& stream) const;

	//
	// Data binding.
	//
	//
	/// * Test whether this node is bound to an external data source.
	bool isTied() const { return _tied; }

	/// * Bind this node to an external source.
	template<typename T>
	bool tie(const GWRawValue<T> &rawValue, bool useDefault = true);

	/// * Unbind this node from any external data source.
	bool untie();

	//
	// Convenience methods using paths.
	// TODO: add attribute methods
	//
	/// * Get another node's type.
	gwBase::props::Type getType(const char * relative_path) const;

	/// * Get another node's type.
	gwBase::props::Type getType(const std::string& relative_path) const
	{
		return getType(relative_path.c_str());
	}

	/// * Test whether another node has a leaf value.
	bool hasValue(const char * relative_path) const;

	/// * Test whether another node has a leaf value.
	bool hasValue(const std::string& relative_path) const
	{
		return hasValue(relative_path.c_str());
	}

	/// * Get another node's value as a bool.
	bool getBoolValue(const char * relative_path, bool defaultValue = false) const;

	/// * Get another node's value as a bool.
	bool getBoolValue(const std::string& relative_path, bool defaultValue = false) const
	{
		return getBoolValue(relative_path.c_str(), defaultValue);
	}

	/// * Get another node's value as an int.
	int getIntValue(const char * relative_path, int defaultValue = 0) const;

	/// * Get another node's value as an int.
	int getIntValue(const std::string& relative_path, int defaultValue = 0) const
	{
		return getIntValue(relative_path.c_str(), defaultValue);
	}

	/// * Get another node's value as a long int.
	long getLongValue(const char * relative_path, long defaultValue = 0L) const;

	/// * Get another node's value as a long int.
	long getLongValue(const std::string& relative_path, long defaultValue = 0L) const
	{
		return getLongValue(relative_path.c_str(), defaultValue);
	}

	/// * Get another node's value as a float.
	float getFloatValue(const char * relative_path, float defaultValue = 0.0f) const;

	/// * Get another node's value as a float.
	float getFloatValue(const std::string& relative_path, float defaultValue = 0.0f) const
	{
		return getFloatValue(relative_path.c_str(), defaultValue);
	}

	/// * Get another node's value as a double.
	double getDoubleValue(const char * relative_path, double defaultValue = 0.0) const;

	/// * Get another node's value as a double.
	double getDoubleValue(const std::string& relative_path,
		double defaultValue = 0.0) const
	{
		return getDoubleValue(relative_path.c_str(), defaultValue);
	}

	/// * Get another node's value as a string.
	const char * getStringValue(const char * relative_path, const char * defaultValue = "") const;

	/// * Get another node's value as a string.
	const char * getStringValue(const std::string& relative_path, const char * defaultValue = "") const
	{
		return getStringValue(relative_path.c_str(), defaultValue);
	}

	/// * Set another node's value as a bool.
	bool setBoolValue(const char * relative_path, bool value);

	/// * Set another node's value as a bool.
	bool setBoolValue(const std::string& relative_path, bool value)
	{
		return setBoolValue(relative_path.c_str(), value);
	}

	/// * Set another node's value as an int.
	bool setIntValue(const char * relative_path, int value);

	/// * Set another node's value as an int.
	bool setIntValue(const std::string& relative_path, int value)
	{
		return setIntValue(relative_path.c_str(), value);
	}

	/// * Set another node's value as a long int.
	bool setLongValue(const char * relative_path, long value);

	/// * Set another node's value as a long int.
	bool setLongValue(const std::string& relative_path, long value)
	{
		return setLongValue(relative_path.c_str(), value);
	}

	/// * Set another node's value as a float.
	bool setFloatValue(const char * relative_path, float value);

	///* Set another node's value as a float.
	bool setFloatValue(const std::string& relative_path, float value)
	{
		return setFloatValue(relative_path.c_str(), value);
	}

	/// * Set another node's value as a double.
	bool setDoubleValue(const char * relative_path, double value);

	/// * Set another node's value as a double.
	bool setDoubleValue(const std::string& relative_path, double value)
	{
		return setDoubleValue(relative_path.c_str(), value);
	}

	/// * Set another node's value as a string.
	bool setStringValue(const char * relative_path, const char * value);
	bool setStringValue(const char * relative_path, const std::string& value)
	{
		return setStringValue(relative_path, value.c_str());
	}

	///* Set another node's value as a string.
	bool setStringValue(const std::string& relative_path, const char * value)
	{
		return setStringValue(relative_path.c_str(), value);
	}
	bool setStringValue(const std::string& relative_path, const std::string& value)
	{
		return setStringValue(relative_path.c_str(), value.c_str());
	}

	/// * Set another node's value with no specified type.
	bool setUnspecifiedValue(const char * relative_path, const char * value);

	///* Test whether another node is bound to an external data source.
	bool isTied(const char * relative_path) const;

	/// * Test whether another node is bound to an external data source.
	bool isTied(const std::string& relative_path) const
	{
		return isTied(relative_path.c_str());
	}

	/// * Bind another node to an external bool source.
	bool tie(const char * relative_path, const GWRawValue<bool> &rawValue, bool useDefault = true);

	/// * Bind another node to an external bool source.
	bool tie(const std::string& relative_path, const GWRawValue<bool> &rawValue, bool useDefault = true)
	{
		return tie(relative_path.c_str(), rawValue, useDefault);
	}

	/// * Bind another node to an external int source.
	bool tie(const char * relative_path, const GWRawValue<int> &rawValue, bool useDefault = true);

	/// * Bind another node to an external int source.
	bool tie(const std::string& relative_path, const GWRawValue<int> &rawValue, bool useDefault = true)
	{
		return tie(relative_path.c_str(), rawValue, useDefault);
	}

	/// * Bind another node to an external long int source.
	bool tie(const char * relative_path, const GWRawValue<long> &rawValue, bool useDefault = true);

	/// * Bind another node to an external long int source.
	bool tie(const std::string& relative_path, const GWRawValue<long> &rawValue, bool useDefault = true)
	{
		return tie(relative_path.c_str(), rawValue, useDefault);
	}

	/// * Bind another node to an external float source.
	bool tie(const char * relative_path, const GWRawValue<float> &rawValue, bool useDefault = true);

	/// * Bind another node to an external float source.
	bool tie(const std::string& relative_path, const GWRawValue<float> &rawValue, bool useDefault = true)
	{
		return tie(relative_path.c_str(), rawValue, useDefault);
	}

	/// * Bind another node to an external double source.
	bool tie(const char * relative_path, const GWRawValue<double> &rawValue, bool useDefault = true);

	/// * Bind another node to an external double source.
	bool tie(const std::string& relative_path, const GWRawValue<double> &rawValue, bool useDefault = true)
	{
		return tie(relative_path.c_str(), rawValue, useDefault);
	}

	/// * Bind another node to an external string source.
	bool tie(const char * relative_path, const GWRawValue<const char *> &rawValue, bool useDefault = true);

	///* Bind another node to an external string source.
	bool tie(const std::string& relative_path, const GWRawValue<const char*> &rawValue, bool useDefault = true)
	{
		return tie(relative_path.c_str(), rawValue, useDefault);
	}

	/// * Unbind another node from any external data source.
	bool untie(const char * relative_path);

	/// * Unbind another node from any external data source.
	bool untie(const std::string& relative_path)
	{
		return untie(relative_path.c_str());
	}

	/**
	 * Add a change listener to the property. If "initial" is set call the
	 * listener initially.
	 */
	void addChangeListener(GWPropertyChangeListener * listener, bool initial = false);

	/// * Remove a change listener from the property.
	void removeChangeListener(GWPropertyChangeListener * listener);

	/// * Get the number of listeners.
	int nListeners() const { return _listeners ? (int)_listeners->size() : 0; }

	/// * Fire a value change event to all listeners.
	void fireValueChanged();

	/// * Fire a child-added event to all listeners.
	void fireChildAdded(GWPropertyNode * child);

	/**
	 * Trigger a child-added and value-changed event for every child (Unlimited
	 * depth).
	 *
	 * @param fire_self   Whether to trigger the events also for the node itself.
	 *
	 * It can be used to simulating the creation of a property tree, eg. for
	 * (re)initializing a subsystem which is controlled through the property tree.
	 */
	void fireCreatedRecursive(bool fire_self = false);

	/// * Fire a child-removed event to all listeners.
	void fireChildRemoved(GWPropertyNode * child);

	/**
	 * Fire a child-removed event for every child of this node (Unlimited depth)
	 *
	 * Upon removal of a child node only for this single node a child-removed
	 * event is triggered. If eg. resource cleanup relies on receiving a
	 * child-removed event for every child this method can be used.
	 */
	void fireChildrenRemovedRecursive();

	/// * Clear any existing value and set the type to NONE.
	void clearValue();

	/**
	 * Compare two property trees. The property trees are equal if: 1)
	 * They have no children, and have the same type and the values are
	 * equal, or 2) have the same number of children, and the
	 * corresponding children in each tree are equal. "corresponding"
	 * means have the same name and index.
	 *
	 * Attributes, removed children, and aliases aren't considered.
	 */
	static bool compare(const GWPropertyNode& lhs, const GWPropertyNode& rhs);

protected:
	void fireValueChanged(GWPropertyNode * node);
	void fireChildAdded(GWPropertyNode * parent, GWPropertyNode * child);
	void fireChildRemoved(GWPropertyNode * parent, GWPropertyNode * child);

	GWPropertyNodePtr eraseChild(gwBase::PropertyList::iterator child);

	/// * Protected constructor for making new nodes on demand.
	GWPropertyNode(const std::string& name, int index, GWPropertyNode * parent);
	template<typename Itr>
	GWPropertyNode(Itr begin, Itr end, int index, GWPropertyNode * parent);

	//static gwBase::PropertyInterpolationMgr* _interpolation_mgr;

private:
	// Get the raw value
	bool get_bool() const;
	int get_int() const;
	long get_long() const;
	float get_float() const;
	double get_double() const;
	const char * get_string() const;

	// Set the raw value
	bool set_bool(bool value);
	bool set_int(int value);
	bool set_long(long value);
	bool set_float(float value);
	bool set_double(double value);
	bool set_string(const char * value);

	///* Get the value as a string.
	const char * make_string() const;

	// * Trace a read access.
	void trace_read() const;

	// * Trace a write access.
	void trace_write() const;

	int _index;
	std::string _name;
	/// To avoid cyclic reference counting loops this shall not be a reference
	/// counted pointer
	GWPropertyNode * _parent;
	gwBase::PropertyList _children;
	mutable std::string _buffer;
	gwBase::props::Type _type;
	bool _tied;
	int _attr;

	// The right kind of pointer...
	union
	{
		GWPropertyNode * alias;
		GWRaw* val;
	} _value;

	union
	{
		bool bool_val;
		int int_val;
		long long_val;
		float float_val;
		double double_val;
		char * string_val;
	} _local_val;

	std::vector<GWPropertyChangeListener *> * _listeners;

	// Pass name as a pair of iterators
	template<typename Itr>
	GWPropertyNode * getChildImpl(Itr begin, Itr end, int index = 0, bool create = false);

	// very internal method
	template<typename Itr>
	GWPropertyNode* getExistingChild(Itr begin, Itr end, int index);

	// very internal path parsing function
	template<typename SplitItr>
	friend GWPropertyNode* find_node_aux(GWPropertyNode * current, SplitItr& itr, bool create, int last_index);

	// For boost
	friend size_t hash_value(const GWPropertyNode& node);
};

// Convenience functions for use in templates
template<typename T>
#if PROPS_STANDALONE
T
#else
typename boost::disable_if<boost::is_enum<T>, T>::type
#endif
getValue(const GWPropertyNode*);

template<>
inline bool getValue<bool>(const GWPropertyNode* node) { return node->getBoolValue(); }

template<>
inline int getValue<int>(const GWPropertyNode* node) { return node->getIntValue(); }

template<>
inline long getValue<long>(const GWPropertyNode* node) { return node->getLongValue(); }

template<>
inline float getValue<float>(const GWPropertyNode* node)
{
	return node->getFloatValue();
}

template<>
inline double getValue<double>(const GWPropertyNode* node)
{
	return node->getDoubleValue();
}

template<>
inline const char * getValue<const char*>(const GWPropertyNode* node)
{
	return node->getStringValue();
}

template<>
inline std::string getValue<std::string>(const GWPropertyNode* node)
{
	return node->getStringValue();
}

namespace gwBase
{
	/**
	 * Default trait for extracting enum values from GWPropertyNode. Create your
	 * own specialization for specific enum types to enable validation of values.
	 */
	template<class T>
	struct enum_traits
	{
		/// * Typename of the enum
		static const char* name() { return typeid(T).name(); }

		/// * @return Default value (will be used if validation fails)
		static T defVal() { return T(); }

		/// * @return Whether the given integer value has an enum value defined
		static bool validate(int) { return true; }
	};
} // namespace gwBase

/** Extract enum from GWPropertyNode */
template<typename T>
#if PROPS_STANDALONE
inline T
#else
inline typename boost::enable_if<boost::is_enum<T>, T>::type
#endif
getValue(const GWPropertyNode* node)
{
	typedef gwBase::enum_traits<T> Traits;
	int val = node->getIntValue();
	if (!Traits::validate(val))
	{
		GW_LOG(GW_GENERAL, GW_WARN, "Invalid value for enum (" << Traits::name() << ", val = " << val << ")");
		return Traits::defVal();
	}
	return static_cast<T>(node->getIntValue());
}

inline bool setValue(GWPropertyNode* node, bool value)
{
	return node->setBoolValue(value);
}

inline bool setValue(GWPropertyNode* node, int value)
{
	return node->setIntValue(value);
}

inline bool setValue(GWPropertyNode* node, long value)
{
	return node->setLongValue(value);
}

inline bool setValue(GWPropertyNode* node, float value)
{
	return node->setFloatValue(value);
}

inline bool setValue(GWPropertyNode* node, double value)
{
	return node->setDoubleValue(value);
}

inline bool setValue(GWPropertyNode* node, const char* value)
{
	return node->setStringValue(value);
}

inline bool setValue(GWPropertyNode* node, const std::string& value)
{
	return node->setStringValue(value.c_str());
}

template<typename T>
bool GWPropertyNode::tie(const GWRawValue<T> &rawValue, bool useDefault)
{
	using namespace gwBase::props;
	if (_type == ALIAS || _tied)
	{
		return false;
	}

	useDefault = useDefault && hasValue();
	T old_val = GWRawValue<T>::DefaultValue();
	if (useDefault)
	{
		old_val = getValue<T>(this);
	}

	clearValue();
	if (PropertyTraits<T>::Internal)
	{
		_type = PropertyTraits<T>::type_tag;
	}
	else
	{
		_type = EXTENDED;
	}

	_tied = true;
	_value.val = rawValue.clone();
	if (useDefault)
	{
		int save_attributes = getAttributes();
		setAttribute(WRITE, true);
		setValue(old_val);
		setAttributes(save_attributes);
	}
	return true;
}

template<>
bool GWPropertyNode::tie(const GWRawValue<const char *> &rawValue, bool useDefault);

template<typename T>
T GWPropertyNode::getValue(typename boost::disable_if_c<gwBase::props::PropertyTraits<T>::Internal>::type* dummy) const
{
	using namespace gwBase::props;
	if (_attr == (READ | WRITE) && _type == EXTENDED
		&& _value.val->getType() == PropertyTraits<T>::type_tag)
	{
		return static_cast<GWRawValue<T>*>(_value.val)->getValue();
	}
	if (getAttribute(TRACE_READ))
	{
		trace_read();
	}
	if (!getAttribute(READ))
	{
		return GWRawValue<T>::DefaultValue();
	}
	switch (_type)
	{
	case EXTENDED:
		if (_value.val->getType() == PropertyTraits<T>::type_tag)
		{
			return static_cast<GWRawValue<T>*>(_value.val)->getValue();
		}
		break;
	case STRING:
	case UNSPECIFIED:
		return gwBase::parseString<T>(make_string());
		break;
	default: // avoid compiler warning
		break;
	}
	return GWRawValue<T>::DefaultValue();
}

template<typename T>
inline T GWPropertyNode::getValue(typename boost::enable_if_c<gwBase::props::PropertyTraits<T>::Internal>::type* dummy) const
{
	return ::getValue<T>(this);
}

template<typename T, typename T_get /* = T */> // TODO use C++11 or traits
std::vector<T> GWPropertyNode::getChildValues(const std::string& name) const
{
	const gwBase::PropertyList& props = getChildren(name);
	std::vector<T> values(props.size());

	for (size_t i = 0; i < props.size(); ++i)
	{
		values[i] = props[i]->getValue<T_get>();
	}

	return values;
}

template<typename T>
inline std::vector<T> GWPropertyNode::getChildValues(const std::string& name) const
{
	return getChildValues<T, T>(name);
}

template<typename T>
bool GWPropertyNode::setValue(const T& val, typename boost::disable_if_c<gwBase::props::PropertyTraits<T>::Internal>::type* dummy)
{
	using namespace gwBase::props;
	if (_attr == (READ | WRITE) && _type == EXTENDED && _value.val->getType() == PropertyTraits<T>::type_tag)
	{
		static_cast<GWRawValue<T>*>(_value.val)->setValue(val);
		return true;
	}
	if (getAttribute(WRITE) && ((_type == EXTENDED && _value.val->getType() == PropertyTraits<T>::type_tag) || _type == NONE || _type == UNSPECIFIED))
	{
		if (_type == NONE || _type == UNSPECIFIED)
		{
			clearValue();
			_type = EXTENDED;
			_value.val = new GWRawValueContainer<T>(val);
		}
		else
		{
			static_cast<GWRawValue<T>*>(_value.val)->setValue(val);
		}
		if (getAttribute(TRACE_WRITE))
		{
			trace_write();
		}
		return true;
	}
	return false;
}

template<typename T>
inline bool GWPropertyNode::setValue(const T& val, typename boost::enable_if_c<gwBase::props::PropertyTraits<T>::Internal>::type* dummy)
{
	return ::setValue(this, val);
}

/// * Utility function for creation of a child property node.
inline GWPropertyNode* makeChild(GWPropertyNode* parent, const char* name, int index = 0)
{
	return parent->getChild(name, index, true);
}

/**
 * Utility function for creation of a child property node using a
 * relative path.
 */
namespace gwBase
{
	template<typename StringType>
	inline GWPropertyNode* makeNode(GWPropertyNode* parent, const StringType& name)
	{
		return parent->getNode(name, true);
	}
}

// For boost::hash
size_t hash_value(const GWPropertyNode& node);

// Helper comparison and hash functions for common cases
namespace gwBase
{
	namespace props
	{
		struct Compare
		{
			bool operator()(const GWPropertyNode* lhs, const GWPropertyNode* rhs) const
			{
				return GWPropertyNode::compare(*lhs, *rhs);
			}
			bool operator()(GWPropertyNodePtr lhs, const GWPropertyNode* rhs) const
			{
				return GWPropertyNode::compare(*lhs, *rhs);
			}
			bool operator()(const GWPropertyNode* lhs, GWPropertyNodePtr rhs) const
			{
				return GWPropertyNode::compare(*lhs, *rhs);
			}
			bool operator()(GWPropertyNodePtr lhs, GWPropertyNodePtr rhs) const
			{
				return GWPropertyNode::compare(*lhs, *rhs);
			}
		};

		struct Hash
		{
			size_t operator()(const GWPropertyNode* node) const
			{
				return hash_value(*node);
			}
			size_t operator()(GWPropertyNodePtr node) const
			{
				return hash_value(*node);
			}
		};
	}
}

/** Convenience class for change listener callbacks without
 * creating a derived class implementing a "valueChanged" method.
 * Also removes listener on destruction automatically.
 */
template<class T>
class SGPropertyChangeCallback : public GWPropertyChangeListener
{
public:
	SGPropertyChangeCallback(T* obj, void (T::*method)(GWPropertyNode*), GWPropertyNodePtr property, bool initial = false)
		: _obj(obj), _callback(method), _property(property)
	{
		_property->addChangeListener(this, initial);
	}

	SGPropertyChangeCallback(const SGPropertyChangeCallback<T>& other)
		: _obj(other._obj)
		, _callback(other._callback)
		, _property(other._property)
	{
		_property->addChangeListener(this, false);
	}

	virtual ~SGPropertyChangeCallback()
	{
		_property->removeChangeListener(this);
	}
	void valueChanged(GWPropertyNode * node)
	{
		(_obj->*_callback)(node);
	}

private:
	T* _obj;
	void (T::*_callback)(GWPropertyNode*);
	GWPropertyNodePtr _property;
};

#endif // GWUTIL_PROPS_H

// end of Props.h
