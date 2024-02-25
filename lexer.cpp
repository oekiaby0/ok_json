#include <string>

#include "lexer.hpp"

#define HANDLE_WORD(wordstr, value) {\
    if (c == wordstr[++word_index]) {\
        if (word_index == std::char_traits<char>::length(wordstr) - 1) {\
            tokens.push_back({ value });\
            word_index = 0;\
            state = DEFAULT;\
        }\
    }\
    else {\
        return std::nullopt;\
    }\
}

namespace OK {

    constexpr inline bool is_number_end(char c) {
        return c == ',' || c == '}' || c == ']' || c == ' ' || c == '\n' || c == '\r' || c == '\t';
    }

    struct Number {
        long double value;
        size_t length;
    };

    Number parse_number(std::string_view str) {
        enum {
            START,
            MINUS_SIGN,
            DIGIT_NON_ZERO,
            DIGIT_REMAINING,
            DIGIT_ZERO,
            DECIMAL_POINT,
            DIGIT_DECIMAL,
            EXPONENT_SYMBOL,
            EXPONENT_SIGN,
            DIGIT_EXPONENT,
            END
        } number_state = START;
        size_t i;
        for (i = 0; i < str.size(); i++) {
            char c = str.at(i);
            switch (number_state) {
                case START:
                    if (c == '-') {
                        number_state = MINUS_SIGN;
                    } else if (c == '0') {
                        number_state = DIGIT_ZERO;
                    } else if (c >= '0' && c <= '9') {
                        number_state = DIGIT_NON_ZERO;
                    } else {
                        return {0, 0};
                    }
                    break;
                case MINUS_SIGN:
                    if (c == '0') {
                        number_state = DIGIT_ZERO;
                    } else if (c >= '1' && c <= '9') {
                        number_state = DIGIT_NON_ZERO;
                    } else {
                        return {0, 0};
                    }
                    break;
                case DIGIT_ZERO:
                    if (c == '.') {
                        number_state = DECIMAL_POINT;
                    } else if (c == 'e' || c == 'E') {
                        number_state = EXPONENT_SYMBOL;
                    } else {
                        if (!is_number_end(c)) return {0, 0};
                        number_state = END;
                    }
                    break;
                case DIGIT_NON_ZERO:
                    if (c >= '0' && c <= '9') {
                        number_state = DIGIT_REMAINING;
                    } else if (c == '.') {
                        number_state = DECIMAL_POINT;
                    } else if (c == 'e' || c == 'E') {
                        number_state = EXPONENT_SYMBOL;
                    } else {
                        if (!is_number_end(c)) return {0, 0};
                        number_state = END;
                    }
                    break;
                case DIGIT_REMAINING:
                    if (c >= '0' && c <= '9') {
                        number_state = DIGIT_REMAINING;
                    } else if (c == '.') {
                        number_state = DECIMAL_POINT;
                    } else if (c == 'e' || c == 'E') {
                        number_state = EXPONENT_SYMBOL;
                    } else {
                        if (!is_number_end(c)) return {0, 0};
                        number_state = END;
                    }
                    break;
                case DECIMAL_POINT:
                    if (c >= '0' && c <= '9') {
                        number_state = DIGIT_DECIMAL;
                    } else {
                        return {0, 0};
                    }
                    break;
                case DIGIT_DECIMAL:
                    if (c >= '0' && c <= '9') {
                        number_state = DIGIT_DECIMAL;
                    } else if (c == 'e' || c == 'E') {
                        number_state = EXPONENT_SYMBOL;
                    } else {
                        if (!is_number_end(c)) return {0, 0};
                        number_state = END;
                    }
                    break;
                case EXPONENT_SYMBOL:
                    if (c == '-' || c == '+') {
                        number_state = EXPONENT_SIGN;
                    } else if (c >= '0' && c <= '9') {
                        number_state = DIGIT_EXPONENT;
                    } else {
                        return {0, 0};
                    }
                    break;
                case EXPONENT_SIGN:
                    if (c >= '0' && c <= '9') {
                        number_state = DIGIT_EXPONENT;
                    } else {
                        return {0, 0};
                    }
                    break;
                case DIGIT_EXPONENT:
                    if (c >= '0' && c <= '9') {
                        number_state = DIGIT_EXPONENT;
                    } else {
                        if (!is_number_end(c)) return {0, 0};
                        number_state = END;
                    }
                    break;
            }
            if (number_state == END) {
                break;
            }
        }

        // TODO: parse the number while verifying its grammar for speed?
//		long double value;
//		auto result = std::from_chars(str.data(), str.data() + i, value);
//		if (result.ec == std::errc::invalid_argument || result.ec == std::errc::result_out_of_range) {
//			return { 0, 0 };
//		}
//		return { value, i };

        size_t pos;
        long double value = std::stold(str.data(), &pos);
        if (pos == 0 || pos == i) {
            return {0, 0};
        }
        return {value, pos};
    }

    std::optional<std::vector<OK::json_token>> tokenize(std::string_view str) {
        enum {
            DEFAULT,
            STRING,
            NUMBER,
            WORD_TRUE,
            WORD_FALSE,
            WORD_NULL,
        } state = DEFAULT;
        size_t word_index = 0;
        size_t string_start = 0;
        bool is_escaped = false;
        std::vector<OK::json_token> tokens;
        for (size_t index = 0; index < str.size(); index++) {
            char c = str.at(index);
            if (state == DEFAULT) {
                switch (c) {
                    case ' ':
                    case '\n':
                    case '\r':
                    case '\t':
                        continue;
                    case '{':
                        tokens.push_back({OK::JSON_TOKEN_TYPE::LEFT_BRACE});
                        continue;
                    case '}':
                        tokens.push_back({OK::JSON_TOKEN_TYPE::RIGHT_BRACE});
                        continue;
                    case '[':
                        tokens.push_back({OK::JSON_TOKEN_TYPE::LEFT_BRACKET});
                        continue;
                    case ']':
                        tokens.push_back({OK::JSON_TOKEN_TYPE::RIGHT_BRACKET});
                        continue;
                    case ',':
                        tokens.push_back({OK::JSON_TOKEN_TYPE::COMMA});
                        continue;
                    case ':':
                        tokens.push_back({OK::JSON_TOKEN_TYPE::COLON});
                        continue;
                    case '"':
                        state = STRING;
                        string_start = index + 1;
                        continue;
                    case '-':
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9':
                    case '0':
                        state = NUMBER;
                        break;
                    case 't':
                        state = WORD_TRUE;
                        continue;
                    case 'f':
                        state = WORD_FALSE;
                        continue;
                    case 'n':
                        state = WORD_NULL;
                        continue;
                    default:
                        return std::nullopt;
                }
            } else if (state == STRING) {
                if (c == '"' && !is_escaped) {
                    // TODO: unescape strings
                    tokens.push_back({str.substr(string_start, index - string_start)});
                    state = DEFAULT;
                } else {
                    if (c == '\\' || is_escaped) {
                        is_escaped = !is_escaped;
                    }
                }
            } else if (state == WORD_TRUE) {
                HANDLE_WORD("true", OK::JSON_TOKEN_TYPE::TRUE);
            } else if (state == WORD_FALSE) {
                HANDLE_WORD("false", OK::JSON_TOKEN_TYPE::FALSE);
            } else if (state == WORD_NULL) {
                HANDLE_WORD("null", OK::JSON_TOKEN_TYPE::NULL_WORD);
            }

            if (state == NUMBER) {
                Number number = parse_number(str.substr(index));
                if (!number.length) {
                    return std::nullopt;
                }
                tokens.push_back({number.value});
                index += number.length - 1;
                state = DEFAULT;
            }
        }

        if (state != DEFAULT) {
            return std::nullopt;
        }
        return tokens;
    }
}