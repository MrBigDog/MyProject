#ifndef GWBASE_TYPESDEF_H
#define GWBASE_TYPESDEF_H 1

#ifndef __cplusplus
# error This library requires C++
#endif

#include <compiler.h>

#include <string>
#include <vector>

/** STL vector list of ints */
typedef std::vector < int > IntList;
typedef IntList::iterator IntListIterator;// int_list_iterator;
typedef IntList::const_iterator ConstIntListIterator;// const_int_list_iterator;

/** STL vector list of doubles */
typedef std::vector < double > DoubleList;// double_list;
typedef DoubleList::iterator DoubleListIterator;// double_list_iterator;
typedef DoubleList::const_iterator ConstDoubleListIterator;// const_double_list_iterator;

/** STL vector list of strings */
typedef std::vector < std::string > StringList;// string_list;
typedef StringList::iterator StringListIterator;
typedef StringList::const_iterator ConstStringListIterator;

#endif // GWUTIL_TYPESDEF_H

