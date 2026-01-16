#pragma once

#ifndef COMPILE_ASSERT
#define COMPILE_ASSERT(EXPRESSION) typedef char __static_assert_t[(EXPRESSION) ? 1 : -1]
//#define COMPILE_ASSERT(EXPR) static_assert((EXPR), #EXPR)
#endif // COMPILE_ASSERT

#ifndef OFFSETOF
#define OFFSETOF(STRUCT, MEMBER) __builtin_offsetof(STRUCT, MEMBER)
#endif // OFFSETOF

#ifndef NUMBEROF
#define NUMBEROF(ARRAY) (sizeof(ARRAY) / sizeof(ARRAY[0]))
#endif // NUMBEROF

