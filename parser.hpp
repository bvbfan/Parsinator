/*
 * MIT License

 Copyright (c) 2025 ashwith2427

 Permission is hereby granted, free of charge, to any person obtaining
 a copy of this software and associated documentation files (the
 "Software"), to deal in the Software without restriction, including
 without limitation the rights to use, copy, modify, merge, publish,
 distribute, sublicense, and/or sell copies of the Software, and to
 permit persons to whom the Software is furnished to do so, subject to
 the following conditions:

 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#include <iostream>
#include <string_view>
#include <variant>

class Error {
    const char *err;
public:
    constexpr Error(const char* err) : err(err) {}
    constexpr const char* getError() const { return err; }
};

using parsedResult = std::variant<std::monostate, Error, char, std::string_view>;

template<class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };

class Parser {
    parsedResult result;
    std::string_view input;
    constexpr Parser(parsedResult res, std::string_view v) : result(std::move(res)), input{v} {}
public:
    constexpr Parser(std::string_view v) : input{v} {}
    constexpr Parser parse(char c) const {
        if (input.empty()) {
            return {Error("Empty input"), ""};
        }
        if (input[0] == c) {
            return {c, input.substr(1)};
        }
        return {Error("No match"), input};
    }
    constexpr Parser parse(std::string_view expected) const {
        if (input.empty()) {
            return {Error("Empty input"), ""};
        }
        if (expected.size() <= input.size()) {
            auto s = input.substr(0, expected.size());
            if (s == expected) {
                return {expected, input.substr(expected.size())};
            }
            return {Error("String cannot be matched!"), input};
        }
        return {Error("Expected string should be less than or equal to input string"), input};
    }
    friend inline std::ostream& operator<<(std::ostream& os, const Parser& parser)
    {
        std::visit(overloaded{
            [&](std::monostate) {
                os << "uninitialized monostate\n";
            },
            [&](const Error& e) {
                os << "Error: " << e.getError() << '\n';
            },
            [&](char c) {
                os << "value (char): " << c << '\n';
            },
            [&](std::string_view s) {
                os << "value (string view): " << s << '\n';
            }}, parser.result);
        return os;
    }
};
