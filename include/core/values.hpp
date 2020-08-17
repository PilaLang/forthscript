#pragma once

#include <stdint.h>

#include <functional>
#include <string>
#include <vector>

struct Object {
    struct Object *next_to_scan, *next, *prev;
    size_t referenceCount : 63;
    bool marked : 1;

    virtual Object *addPointedToQueue(struct Object *head) = 0;
    virtual ~Object();
};

typedef struct ExecutionResult (*NativeWord)(struct Interpreter &);

enum class ValueType {
    Nil = 1,
    Numeric = 2,
    Boolean = 3,
    Word = 128,
    WordAssign = 129,
    WordDeclare = 130,
    String = 131,
    Array = 388,
    Placeholder = 389
};

inline bool isHeapType(ValueType type) { return (((int)(type)) & 128) != 0; }
inline bool isArrayType(ValueType type) { return (((int)(type)) & 256) != 0; }

struct Value {
    ValueType type = ValueType::Nil;
    union {
        bool booleanValue;
        int64_t numericValue;
        Object *object;
        struct String *str;
        struct Array *arr;
    };
};

struct String : Object {
    virtual ~String();
    virtual Object *addPointedToQueue(struct Object *head);
    inline const std::u32string &get() const { return str; }
    inline String(const std::u32string &str) { this->str = str; }
    inline String(std::u32string_view sv) { str = sv; }
    // this is used for symbol table
    // as strings are interned
    // there is only one object representing a given string
    // symbol table can just find this vector directly
    // instead of doing costly lookup in hash map
    std::vector<Value> *values = nullptr;

   private:
    std::u32string str;
};

struct Array : Object {
    std::vector<Value> values;
    virtual ~Array();
    virtual Object *addPointedToQueue(struct Object *head);
};