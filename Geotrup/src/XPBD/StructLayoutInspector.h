#ifndef STRUCT_LAYOUT_INSPECTOR_HPP
#define STRUCT_LAYOUT_INSPECTOR_HPP

#include <cstddef>
#include <iostream>
#include <iomanip>
#include <type_traits>
#include <string>

/// Usage : 
/// INSPECT_STRUCT(JAttach, "JAttach", pt1, pt2, m_DRest, m_Alpha, pos1, pos2);
/// INSPECT_STRUCT(JRestrictAxis, "JRestrictAxis", axis, pt1, pos1, pt2, pos2, pos_min, pos_max, m_Alpha);
/// /!\ the order of the parameters are important ! (should be the same as in the struct)
namespace struct_layout {

// Context (no globals)
struct Context {
    std::size_t total = 0;
    std::size_t actual = 0;
};

// Header
template<typename T>
void print_header(const std::string& name, Context& ctx) {
    static_assert(std::is_standard_layout_v<T>,
                  "INSPECT_STRUCT requires standard-layout types");

    ctx.total = 0;
    ctx.actual = sizeof(T);

    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "Struct: " << name << "\n";
    std::cout << std::string(60, '=') << "\n";
    std::cout << std::left << std::setw(20) << "Member"
              << " | Offset | Size\n";
    std::cout << std::string(60, '-') << "\n";
}

// Member printer
template<typename T, typename Member>
void print_member(const char* name, Member T::*ptr, Context& ctx) {
    std::size_t offset =
        reinterpret_cast<std::size_t>(
            &(reinterpret_cast<T const volatile*>(0)->*ptr));

    std::size_t size =
        sizeof((reinterpret_cast<T const volatile*>(0)->*ptr));

    ctx.total += size;

    std::cout << std::left << std::setw(20) << name
              << " | " << std::setw(6) << offset
              << " | " << std::setw(4) << size << " bytes\n";
}

// Footer
template<typename T>
void print_footer(Context& ctx) {
    std::size_t wasted = ctx.actual - ctx.total;
    double efficiency = (ctx.total * 100.0) / ctx.actual;

    std::cout << std::string(60, '-') << "\n";
    std::cout << std::left << std::setw(28) << "Actual Size:"
              << std::right << ctx.actual << " bytes\n";
    std::cout << std::left << std::setw(28) << "Theoretical Min Size:"
              << std::right << ctx.total << " bytes\n";
    std::cout << std::left << std::setw(28) << "Alignment:"
              << std::right << alignof(T) << " bytes\n";
    std::cout << std::left << std::setw(28) << "Padding (Wasted):"
              << std::right << wasted << " bytes\n";
    std::cout << std::left << std::setw(28) << "Efficiency:"
              << std::right << std::fixed << std::setprecision(1)
              << efficiency << "%\n";
    std::cout << std::string(60, '=') << "\n";
}

} // namespace struct_layout


// ---------------- MACRO LAYER (clean + unlimited args) ----------------

// Expand one field
#define SL_FIELD(TYPE, MEMBER, CTX) \
    struct_layout::print_member<TYPE>(#MEMBER, &TYPE::MEMBER, CTX);

// Variadic expansion (C++17 fold via initializer list trick)
#define SL_EXPAND_FIELDS(TYPE, CTX, ...) \
    (void)std::initializer_list<int>{ \
        (SL_FIELD(TYPE, __VA_ARGS__, CTX), 0) \
    }

// Trick to map each argument individually
#define SL_FE_1(WHAT, TYPE, CTX, X) WHAT(TYPE, X, CTX)
#define SL_FE_2(WHAT, TYPE, CTX, X, ...) WHAT(TYPE, X, CTX); SL_FE_1(WHAT, TYPE, CTX, __VA_ARGS__)
#define SL_FE_3(WHAT, TYPE, CTX, X, ...) WHAT(TYPE, X, CTX); SL_FE_2(WHAT, TYPE, CTX, __VA_ARGS__)
#define SL_FE_4(WHAT, TYPE, CTX, X, ...) WHAT(TYPE, X, CTX); SL_FE_3(WHAT, TYPE, CTX, __VA_ARGS__)
#define SL_FE_5(WHAT, TYPE, CTX, X, ...) WHAT(TYPE, X, CTX); SL_FE_4(WHAT, TYPE, CTX, __VA_ARGS__)
#define SL_FE_6(WHAT, TYPE, CTX, X, ...) WHAT(TYPE, X, CTX); SL_FE_5(WHAT, TYPE, CTX, __VA_ARGS__)
#define SL_FE_7(WHAT, TYPE, CTX, X, ...) WHAT(TYPE, X, CTX); SL_FE_6(WHAT, TYPE, CTX, __VA_ARGS__)
#define SL_FE_8(WHAT, TYPE, CTX, X, ...) WHAT(TYPE, X, CTX); SL_FE_7(WHAT, TYPE, CTX, __VA_ARGS__)
#define SL_FE_9(WHAT, TYPE, CTX, X, ...) WHAT(TYPE, X, CTX); SL_FE_8(WHAT, TYPE, CTX, __VA_ARGS__)
#define SL_FE_10(WHAT, TYPE, CTX, X, ...) WHAT(TYPE, X, CTX); SL_FE_9(WHAT, TYPE, CTX, __VA_ARGS__)

// Selector
#define SL_GET_FE(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,NAME,...) NAME

#define SL_FOR_EACH(TYPE, CTX, ...) \
    SL_GET_FE(__VA_ARGS__, \
        SL_FE_10, SL_FE_9, SL_FE_8, SL_FE_7, SL_FE_6, \
        SL_FE_5, SL_FE_4, SL_FE_3, SL_FE_2, SL_FE_1) \
        (SL_FIELD, TYPE, CTX, __VA_ARGS__)

// Main macro
#define INSPECT_STRUCT(TYPE, NAME, ...) \
    do { \
        struct_layout::Context ctx; \
        struct_layout::print_header<TYPE>(NAME, ctx); \
        SL_FOR_EACH(TYPE, ctx, __VA_ARGS__) \
        struct_layout::print_footer<TYPE>(ctx); \
    } while(0)
#endif
