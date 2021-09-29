#ifndef _MULTY_DYNAMIC_CAST_
#define _MULTY_DYNAMIC_CAST_

#include <type_traits>
#include <variant>


template<typename ...Ty>
class MultiCast
{
public:
	using ReturnTy = std::variant<Ty...>;

private:
	template<typename ...T>
	struct Cast;
	template<typename T>
	struct Cast<T>
	{
		template<typename InTy>
		static constexpr ReturnTy Execute(InTy in)
		{
			return dynamic_cast<T>(in);
		}
	};
	template<typename T0, typename ...T1>
	class Cast<T0, T1...> : public Cast<T1...>
	{
		using Cast<T1...>::Execute;
	public:
		template<typename InTy>
		static constexpr ReturnTy Execute(InTy in)
		{
			if (T0 ptr = dynamic_cast<T0>(in))
				return ptr;
			else
				return Cast<T1...>::Execute(in);
		}
	};

public:
	template<typename InTy>
	static constexpr ReturnTy Dynamic(InTy in)
	{
		return Cast<Ty...>::Execute(in);
	}
};


//using DynMultiTy = MultiCast< B*, C*, D*, E*>;
//using Var = DynMultiTy::ReturnTy;

#endif