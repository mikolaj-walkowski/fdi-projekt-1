#ifndef EVIL_MACROS_HPP
#define EVIL_MACROS_HPP

#include <functional>
#include <cassert>
#include <cstdio>

#ifdef NDEBUG
#define assertf(condition, message, ...)
#else 
#define assertf(condition, message, ...)\
    if(!(condition)) {\
        fprintf(stderr, "Assertion failed: %s\n", #condition);\
        fprintf(stderr, message, __VA_ARGS__);\
        fprintf(stderr, "\n");\
        assert(0);\
    }
#endif

/// Rzuca error wtedy i tylko wtedy gdy condition == false.
#define enforce(condition, error) if(!(condition)) throw error

class OnExit final 
{
    private:
    std::function<void()> op;

    public:
    OnExit(std::function<void()> op) : op(op) {}
    ~OnExit() 
    {
        op();
    }
    OnExit(const OnExit& other) = delete;
};

#define JOIN_TOKENS(a, b) JOIN_TOKENS_2(a, b)
#define JOIN_TOKENS_2(a, b) a##b

/** Wykonuje op pod koniec obecnego bloku.
 *  Jeśli użyjemy kilku makr onExit w tym samym bloku,
 *  to zostaną wykonane w odwrotnej kolejności niż zostały umieszczone.
 */
#define onExit(op) OnExit JOIN_TOKENS(_onExit_, __LINE__) ([&](){op;})

#endif