/* ConstExprString
 * Original author - Scott Schurr's
 * Source https://github.com/boostcon/cppnow_presentations_2012
 * The author of the changes - Reifat
 * GitHub Repository - https://github.com/Reifat
 * Last change 28.08.2021
 */

/*
* * * * * * * * * * * * * * *
* Tested on the compiler: "x86-64 clang 12.0.1, x64 msvs v19"
* * * * * * * * * * * * * * * *
*/

#ifndef  _CONST_EXPR_STRING
#define  _CONST_EXPR_STRING

#include <type_traits>

class ConstExprString
{
public:
    const char* const _p_str;
    const std::size_t _size;
public:
    template<std::size_t N>
    constexpr ConstExprString(const char(&a)[N]) : _p_str(a), _size(N - 1) {}

    template<std::size_t N>
    constexpr char at()const {
        return N < _size ? _p_str[N] : '\0';
    }
    constexpr std::size_t size() const { return _size; }
};

#endif // end _CONST_EXPR_STRING



