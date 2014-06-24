#pragma once
#include<cassert>
/** ex2-0
 * if T is reference, return T
 * other return T const&
 */
template<typename T>
struct add_const_ref{
	typedef T const& type;
};

template<typename T>
struct add_const_ref<T&>{
	typedef T type;
};

/** ex2-0 test tools
 */
template<typename T1, typename T2>
struct is_same	: std::false_type
{
	// inherited from false type, same to code below:
	// static const bool value = false;
};

template<typename T1>
struct is_same<T1, T1>{
	static const bool value = true;
};

/** ex2-0 Test case
 */
// bool res = is_same<int&, int>::value;
// output: false
// bool res = is_same<int&, int&>::value;
// output: true

// End ex2-0

/** ex2-1 Replace a part of c
 * Can't fit to any kind of c type.
 * TODO: ...
 */
template<typename TC, typename TX, typename TY>
struct replace_type{
	typedef TC type;
};

// replace form like <void*, void, int>
template<typename TX, typename TY>
struct replace_type<TX*, TX, TY>{
	typedef TY* type;
};

// replace form like <int const*[10], int const, long>
template<typename TX, typename TY>
struct replace_type<TX*[10], TX, TY>{
	typedef TY* type[10];							// NOT "typename TY const*[] type" and [10] is needed.
};

// replace form like <char& (*)(char&), char&, long&>
template<typename TX, typename TY>
struct replace_type<TX(*)(TX), TX, TY>{
	typedef TY(*type)(TY);							// NOT "typedef TY(*)(TY) type" or "typedef TY(type*)(TY)"
};

/** ex2-2 safe static_cast
 */
template<typename Target, typename Source>
inline Target polymorphic_downcast(Source* x)
{
	assert(dynamic_cast<Target>(x) == x);			// check logic error, not so helpful~!
	return static_cast<Target>(x);
}
template<typename Target, typename Source>
inline Target polymorphic_downcast(Source& x)
{
	assert(&dynamic_cast<Target>(x) == &x);
	return static_cast<Target>(x);
}

/** ex2-3 type descriptor
 */
#if 0
#include<boost/typeof/typeof.hpp>
template<typename T>
inline std::string type_descriptor()
{
	return typeid(T).name();
}
#endif

/** ex2-4 type decriptor
 */
// basic type can be identified
#if 0
template<typename T>
struct basic_type{
	static const std::string value;
};
template<typename T>
const std::string basic_type<T>::value = "Unknown";

template<>
struct basic_type<int>{
	static const std::string value;
};
//template<>
const std::string basic_type<int>::value = "int";

template<>
struct basic_type<char>{
	static const std::string value;
};
const std::string basic_type<char>::value  = "char";

template<>
struct basic_type<short int>{
	static const std::string value;
};
const std::string basic_type<short int>::value = "short int";

template<>
struct basic_type<long int>{
	static const std::string value;
};
const std::string basic_type<long int>::value = "long int";
// End basic type

//template<typename T>
//inline std::string type_descriptor()
//{
//	return "Unknown type!";
//}
//template<typename T>
//inline std::string type_descriptor<T&>()
//{
//	return basic_type<T> + '&';
//}
//template<typename T>
//inline std::string type_descriptor<T*>()
//{
//	return basic_type<T> + '*';
//}
//template<typename T>
//inline std::string type_descriptor<T const>()
//{
//	return basic_type<T> + "const";
//}
template<typename T>
struct type_desriptor{
	operator std::string(){
		return basic_type<T>::value;
	}
};

template<typename T>
struct type_desriptor<T*>{
	operator std::string(){
		std::string str =  type_desriptor<T>();
		str += " *";
		return str;
	}
};

template<typename T>
struct type_desriptor<T&>{
	operator std::string(){
		std::string str = type_desriptor<T>();
		str += "&";
		return str;
	}
};

template<typename T>
struct type_desriptor<T[]>{
	operator std::string(){
		std::string str = type_desriptor<T>();
		str += "[]";
		return str;
	}
};

// Match to normal function pointer(parameter is empty)
// Result is the return type of the function
// T is the function pointer type
// eg: typedef int (*func[10])(void)
// Result : int, T: *[]
#if 0
template<typename Result, typename T>
struct type_desriptor<Result, T>{
	typedef Result (*FuncPtr)();
	operator std::string(){
		std::string str = type_desriptor<FuncPtr>();
		str += type_desriptor<T>();
		str += "()";
		return str;
	}
};
#endif
#endif

/**ex2-5 return type with description
 */
template<typename T>
struct basic_type{
	static const std::string value;
};
template<typename T>
const std::string basic_type<T>::value = "Unknown";

template<>
struct basic_type<int>{
	static const std::string value;
};
//template<>
const std::string basic_type<int>::value = "int";

template<>
struct basic_type<char>{
	static const std::string value;
};
const std::string basic_type<char>::value  = "char";

template<>
struct basic_type<short int>{
	static const std::string value;
};
const std::string basic_type<short int>::value = "short int";

template<>
struct basic_type<long int>{
	static const std::string value;
};
const std::string basic_type<long int>::value = "long int";
// End basic type

/** Main template
 */
template<typename T>
struct type_desriptor{
	operator std::string(){
		return basic_type<T>::value;
	}
};

template<typename T>
struct type_desriptor<T*>{
	operator std::string(){
		std::string str =  type_desriptor<T>();
		str = "pointer to " + str;
		return str;
	}
};

template<typename T>
struct type_desriptor<T&>{
	operator std::string(){
		std::string str = type_desriptor<T>();
		str = "reference to " + str;
		return str;
	}
};

template<typename T>
struct type_desriptor<T[]>{
	operator std::string(){
		std::string str = type_desriptor<T>();
		str = "array of " + str;
		return str;
	}
};
// match function with return type T
template<typename T>
struct type_desriptor<T()>{
	operator std::string(){
		std::string str = type_desriptor<T>();
		str = "function returning " + str;
		return str;
	}
};

// match basic function pointer(with parameter empty)
template<typename T>
struct type_desriptor<T(*)()>{
	operator std::string(){
		std::string str = type_desriptor<T>();
		str = "pointer to function returning " + str;
		return str;
	}
};
// Match to complicated function pointer(with parameter empty)
// Result is the return type of the function
// T is the function pointer type
// eg: typedef int (*func[10])(void)
// Result : int, T: *[]
#if 0
template<typename Result, typename T>
struct type_desriptor<Result(T*)()>{
	operator std::string(){
		std::string str = type_desriptor<FuncPtr>();
		str += type_desriptor<T>();
		str += "()";
		return str;
	}
};
#endif

/** ex2-6 type category
 */

// match signed char
template<typename T>
struct is_char
	: std::false_type
{
};
template<>
struct is_char<char>
	: std::true_type
{
};
template<>
struct is_char<signed char>
	: std::true_type
{
};

// match unsigned char
template<typename T>
// match char
struct is_unsigned_char
	: std::false_type
{
};
template<>
struct is_unsigned_char<unsigned char>
	: std::true_type
{
};
// signed int, unsigned int, long, long int, long long....etc.
/** ex2-7
 */