/* Meta-library with auxiliary tools for typical traits
*  Author of Rifat
*  GitHub Repository - https://github.com/Reifat
*  Latest changes on 28.08.2021.
*/

#ifndef  _TYPE_TRAIT_
#define _TYPE_TRAIT_

#include <cstdint>
#include <functional>

template<typename ...Ty>
class TypeList
{
public:
	static constexpr std::size_t _size = sizeof...(Ty);
};

template<typename List>
class FrontList;
template<typename Head, typename ...Tail>
class FrontList<TypeList<Head, Tail...>>
{
public:
	using Type = Head;
};
template<typename List>
using Front = typename FrontList<List>::Type;

template<typename List>
class PopFrontList;
template<typename Head, typename ...Tail>
class PopFrontList<TypeList<Head, Tail...>>
{
public:
	using Type = TypeList<Tail...>;
};
template<typename List>
using PopFront = typename PopFrontList<List>::Type;

template<typename List, typename NewElement>
class PushFrontList;
template<typename... Elements, typename NewElement>
class PushFrontList<TypeList<Elements...>, NewElement>
{
public:
	using Type = TypeList<NewElement, Elements...>;
};
template<typename List, typename NewElem>
using PushFront = typename PushFrontList<List, NewElem>::Type;

template<class Ty, std::uint8_t n>
struct CodeType_ {
	using BaseType = Ty;
	static constexpr std::uint8_t _type_code = n;
};

template<typename List, std::size_t inx, typename ...Elemens>
class GenerateCodeTypeList;
template<typename List, std::size_t inx, typename LastElem>
class GenerateCodeTypeList<List, inx, LastElem>
{
public:
	using Type = PushFront<List, CodeType_<LastElem, inx>>;
};
template<typename List, std::size_t inx, typename FirstElem, typename ...Elems>
class GenerateCodeTypeList<List, inx, FirstElem, Elems...>
{
	using NewList = PushFront<List, CodeType_<FirstElem, inx>>;
public:
	using Type = typename GenerateCodeTypeList<NewList, inx + 1, Elems...>::Type;
};
template<typename ...Ty>
using CodeTypeList = typename GenerateCodeTypeList<TypeList<>, 0, Ty...>::Type;


template<typename cTy, typename List>
class FindTypeCode;
template<typename cTy, typename Ty>
class FindTypeCode<cTy, TypeList<Ty>>
{
	static constexpr bool _type_test = std::is_same_v<cTy, typename Ty::BaseType>;

public:
	static constexpr std::uint8_t result = _type_test ? Ty::_type_code : std::numeric_limits<std::uint8_t>::max();
};
template<typename cTy, typename List>
class FindTypeCode : public FindTypeCode<cTy, PopFront<List>>
{
	static constexpr bool _type_test = std::is_same_v<cTy, typename Front<List>::BaseType>;
public:

	static constexpr std::uint8_t result = _type_test ? Front<List>::_type_code : FindTypeCode<cTy, PopFront<List>>::result;
};

template<typename cTy, typename ...Ty>
class CompareTypes;
template<typename cTy, typename Ty>
class CompareTypes<cTy, Ty>
{
public:

	static constexpr bool Execute(const std::function<bool(std::size_t left, std::size_t right)>& compare_func)
	{
		return compare_func(cTy::TypeCode, Ty::TypeCode);
	}
};
template<typename cTy, typename Ty0, typename ...Ty1>
class CompareTypes<cTy, Ty0, Ty1...> : public CompareTypes<cTy, Ty1...>
{
public:

	static constexpr bool Execute(const std::function<bool(std::size_t left, std::size_t right)>& compare_func)
	{
		if (compare_func(cTy::TypeCode, Ty0::TypeCode))
		{
			return true;
		}
		else
			return CompareTypes<cTy, Ty1...>::Execute(compare_func);
	}
};

template<typename ...Ty>
class DynamicMultyCast;
template<typename T>
class DynamicMultyCast<T>
{
public:
	template<typename InTy>
	static constexpr void* Execute(const InTy* in)
	{
		return (void*)dynamic_cast<const T*>(in);
	}
};
template<typename T, typename ...Ty>
class DynamicMultyCast<T, Ty...> : public DynamicMultyCast<Ty...>
{
	using DynamicMultyCast<Ty...>::DCast;
public:
	template<typename InTy>
	static constexpr void* DCast(const InTy* in)
	{
		if (void* ptr = (void*)dynamic_cast<const T*>(in))
			return ptr;
		else
			return (void*)DynamicMultyCast<Ty...>::Execute(in);
	}
};
template<typename RetTy>
class PointerCast
{
public:
	template<typename ...Ty0, typename Ty>
	static constexpr RetTy* MultyDynamic(Ty in)
	{
		return static_cast<RetTy*>(DynamicMultyCast<Ty0...>::Execute(in));
	}
};

#endif