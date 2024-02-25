#ifndef MLEXER_H
#define MLEXER_H

#include <optional>
#include <string_view>
#include <vector>

namespace OK {
    enum class JSON_TOKEN_TYPE {
        TRUE = 1,
        FALSE,
        NULL_WORD,
        STRING,
        NUMBER,
        LEFT_BRACE,
        RIGHT_BRACE,
        LEFT_BRACKET,
        RIGHT_BRACKET,
        COMMA,
        COLON
    };

    struct json_token {
        enum JSON_TOKEN_TYPE type;
        union {
            std::string_view string;
            long double number;
        };

        json_token() {}

        json_token(JSON_TOKEN_TYPE type) {
            this->type = type;
        }

        json_token(std::string_view string) {
            this->type = JSON_TOKEN_TYPE::STRING;
            this->string = string;
        }

        json_token(long double number) {
            this->type = JSON_TOKEN_TYPE::NUMBER;
            this->number = number;
        }
    };

    std::optional<std::vector<json_token>> tokenize(std::string_view str);
}
#endif