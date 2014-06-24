
//template<typename T>
//struct Iterator{
//	typedef T value_type;
//};

/** Type compution
 */
template<typename Iterator>
struct iterator_traits{
	typedef typename Iterator::value_type value_type;
};

/** Type compution(Partial specialition version)
 *
 * Used for '&ClassName' or 'PodType*' type pointer
 */
template<typename T>
struct iterator_traits<T*>{
	typedef T value_type;
};


template<typename ForwardIterator1, typename ForwardIterator2>
void iter_swap(ForwardIterator1 iter1, ForwardIterator2 iter2)
{
	typedef typename iterator_traits<ForwardIterator1>::value_type value_type;
	value_type temp = *iter1;
	*iter1 = * iter2;
	*iter2 = temp;
}

#include<vector>
//#include<boost/type_traits/is_same.hpp>
#include"chapter02.hpp"

using std::vector;

struct A { virtual ~A() {} };
struct B : A {};

// from csdn

#pragma once
#include <vector>
#include <iostream>
#include <string>

template<int orderNum>
class CTemplateA
{
public:
	CTemplateA();
	static int i;
	static std::vector<int> vec;
	static std::string str;
};

template<int orderNum>
int CTemplateA<orderNum>::i = 100;

template<int orderNum>
std::vector<int> CTemplateA<orderNum>::vec = std::vector<int>(5,2);

template<int orderNum>
std::string CTemplateA<orderNum>::str = "late bind?";

template<int orderNum>
CTemplateA<orderNum>::CTemplateA()
{
	using namespace std;
	cout<<"i = " <<i <<endl;
	cout<<"vec的size = " <<vec.size() <<endl;
	for (size_t i = 0; i < vec.size(); i++)
	{
		cout<<vec[i] <<" ";
	}
	cout << str;
}


CTemplateA<10> a;
//output:
//i = 100
//vec的size = 0
// str is output empty
//首先，调试器下“看”到的往往不是真实的。
//其次，C++标准必须保持对C的兼容，所以内建类型大多是优先初始化的。
//如果你不希望这一点，用一个struct把它包起来。
//其次，控制初始化顺序往往是灾难性的坑。所以，这个问题不要过于深入地讨论。
//csdn end.

int main()
{
	vector<int> v;
	v.push_back(2);
	v.push_back(3);

	::iter_swap(v.begin(), v.begin() + 1);
	
	vector<int> v1;
	v1.push_back(4);
	v1.push_back(5);
	::iter_swap(&v, &v1);

	bool res = is_same<int&, int>::value;
	res = is_same<int, int>::value;

	/* test cases for ex2-1 */
	typedef replace_type<void*, void, int>::type t1;
	res = std::is_same<t1, int*>::value;

	typedef replace_type<int const*[10], int const, long>::type t2;
	res = std::is_same<t2, long*[10]>::value;

	typedef replace_type<char& (*)(char&), char&, long&>::type t3;
	res = std::is_same<t3, long& (*)(long&)>::value;

	/* test cases for ex2-2 */
	B b;
	A* a_ptr = &b;
	B* b_ptr = ::polymorphic_downcast<B*>(a_ptr);

	A& a_ref = b;
	B& b_ref = ::polymorphic_downcast<B&>(a_ref);

	/* test cases for ex2-3 */
	std::string tt = type_desriptor<int>();
	tt = type_desriptor<int*>();
	tt = type_desriptor<long int*>();
	tt = type_desriptor<long int*[]>();
	tt = type_desriptor<unsigned int>(); // unknown

	std::cout << a.str;// output late bind?
	return 0;
}