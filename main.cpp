#include <iostream>
#include <iostream>
#include <iomanip>
#include <limits>
#include <numbers>
#include <string>

#include "ok_json.hpp"
#include "lexer.hpp"

int main()
{
    constexpr auto max_precision{ std::numeric_limits<long double>::digits10 + 1 };

    //while (true) {
    //    std::cout << "Enter number: ";
    //    std::string str;
    //    std::cin >> str;
    //    auto number = parse_number(str);
    //    if (number.has_value()) {
    //        std::cout << std::setprecision(max_precision) << number.value() << "\n\n";
    //    }
    //    else {
    //        std::cout << "Invalid!\n\n";
    //    }
    //}
    //return 0;

    while (true) {
        std::cout << "Enter JSON string: ";
        std::string json_string;
        std::getline(std::cin, json_string);
        const char *str = OK::parse(json_string).has_value() ? "Valid" : "Invalid";
        std::cout << str << std::endl;

        //auto tokens = OK::tokenize(json_string);
        //if (tokens.has_value()) {
        //    for (const OK::json_token &token : tokens.value()) {
        //        char c;
        //        switch (token.type) {
        //        case OK::JSON_TOKEN_TYPE::LEFT_BRACE:
        //            c = '{';
        //            break;
        //        case OK::JSON_TOKEN_TYPE::RIGHT_BRACE:
        //            c = '}';
        //            break;
        //        case OK::JSON_TOKEN_TYPE::LEFT_BRACKET:
        //            c = '[';
        //            break;
        //        case OK::JSON_TOKEN_TYPE::RIGHT_BRACKET:
        //            c = ']';
        //            break;
        //        case OK::JSON_TOKEN_TYPE::COMMA:
        //            c = ',';
        //            break;
        //        case OK::JSON_TOKEN_TYPE::COLON:
        //            c = ':';
        //            break;
        //        case OK::JSON_TOKEN_TYPE::TRUE:
        //            std::cout << "true\n";
        //            continue;
        //        case OK::JSON_TOKEN_TYPE::FALSE:
        //            std::cout << "false\n";
        //            continue;
        //        case OK::JSON_TOKEN_TYPE::NULL_WORD:
        //            std::cout << "null\n";
        //            continue;
        //        case OK::JSON_TOKEN_TYPE::STRING:
        //            std::cout << '"' << token.string << "\"\n";
        //            continue;
        //        case OK::JSON_TOKEN_TYPE::NUMBER:
        //            std::cout << std::setprecision(max_precision) << token.number << '\n';
        //            continue;
        //        }
        //        std::cout << c << '\n';
        //    }
        //    //std::cout << std::endl;
        //}
        //else {
        //    std::cout << "error: invalid JSON\n\n";
        //}
    }
}

//#define _CRT_SECURE_NO_DEPRECATE
//#include <stdio.h>
//
//
//typedef enum testStatus { ERROR, PASS, FAIL } TestStatus;
//
//TestStatus parseData(char *data) {
//    return OK::parse(data).has_value() ? PASS : FAIL;
//}
//
//TestStatus testFile(const char *filename) {
//    FILE *f = fopen(filename, "rb");
//    if (f == NULL) { return ERROR; };
//    fseek(f, 0, SEEK_END);
//    long len = ftell(f);
//    fseek(f, 0, SEEK_SET);
//    char *data = (char *)malloc(len + 1);
//    fread(data, 1, len, f);
//    data[len] = '\0';
//    fclose(f);
//    TestStatus status = parseData(data);
//    free(data);
//    return status;
//}
//
//
//int main(int argc, const char *argv[]) {
//
//    try {
//        const char *path = argv[1];
//        TestStatus status = testFile(path);
//        return (status == PASS) ? 0 : 1;
//    }
//    catch (...) {
//        return 1;
//    }
//
//}