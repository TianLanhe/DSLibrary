#ifndef OBJECT_H
#define OBJECT_H

#include "Exception.h"

#define DSLIB_BEGIN	namespace DSLib {
#define DSLIB_END }

DSLIB_BEGIN

#define CHECK_INDEX_OUT_OF_BOUNDS(cond) \
	if(!(cond))							\
		THROW_EXCEPTION(IndexOutOfBoundsException,"Index Out Of Bounds")

#define CHECK_NO_MEMORY_EXCEPTION(pointer)	\
	if((pointer) == nullptr)				\
		THROW_EXCEPTION(NoMemoryException,"No Enough Memory To Create Object")

#define CHECK_PARAMETER_EXCEPTION(cond)	\
	if(!(cond))							\
		THROW_EXCEPTION(InvalidParameterException,"Paramter Is Invalid")

#define CHECK_STATE_EXCEPTION(cond)	\
	if(!(cond))						\
		THROW_EXCEPTION(ErrorStateException,"Abnormal State Occurs")

// 数据结构基类
template<typename T>
class Object {

public:
	typedef T value_type;
	typedef const T const_value_type;
	typedef size_t size_type;
	typedef T& reference;
	typedef const T& const_reference;
	typedef T* iterator;
	typedef const T* const_iterator;

public:
	virtual ~Object() = 0 {}
};

DSLIB_END

#endif // !OBJECT_H
