#pragma once

#include <core/callstack.hpp>
#include <core/evalstack.hpp>
#include <core/heap.hpp>
#include <core/symtable.hpp>
#include <core/values.hpp>
#include <functional>
#include <string>
#include <unordered_map>

enum class ExecutionResultType { Error, Success };

struct ExecutionResult {
    ExecutionResultType result;
    std::u32string error;
};

struct Interpreter {
    CallStack callStack;
    EvaluationStack evalStack;
    Heap heap;
    SymbolTable symTable;
    String *breakString, *returnString, *callString, *commaString;

    Interpreter(size_t maxRecursionDepth);
    void defineNativeWord(const std::u32string& str, NativeWord word);
    ExecutionResult callInterpreter(Array* code, bool newScope);
};