#include <stack>

#include "ok_json.hpp"
#include "lexer.hpp"

#define FAIL() return std::nullopt

namespace OK {
    inline bool is_value(json_token token) {
        return token.type == JSON_TOKEN_TYPE::TRUE
            || token.type == JSON_TOKEN_TYPE::FALSE
            || token.type == JSON_TOKEN_TYPE::NULL_WORD
            || token.type == JSON_TOKEN_TYPE::NUMBER
            || token.type == JSON_TOKEN_TYPE::STRING
            || token.type == JSON_TOKEN_TYPE::LEFT_BRACE
            || token.type == JSON_TOKEN_TYPE::LEFT_BRACKET;
    }

    enum State {
        DEFAULT = 1,
        OBJECT_START,
        OBJECT_END,
        OBJECT_KEY,
        OBJECT_COLON,
        OBJECT_VALUE,
        OBJECT_COMMA,
        ARRAY_START,
        ARRAY_END,
        ARRAY_VALUE,
        ARRAY_COMMA
    };

    void handle_json_return(json& current, std::stack<json>& json_stack, std::stack<std::string_view>& key_stack) {
        auto top = json_stack.top();
        json_stack.pop();
        if (top.type == JSON_TYPE::ARRAY) {
            top.children.push_back(current);
        } else if (top.type == JSON_TYPE::OBJECT) {
            auto key = key_stack.top();
            key_stack.pop();
            top.map[key] = current;
        }
        current = top;
    }

    void handle_value(json_token token, State return_state, State& state, std::stack<State>& state_stack,
                      json& current, std::stack<json>& json_stack) {
        if (token.type == JSON_TOKEN_TYPE::LEFT_BRACE) {
            state_stack.push(return_state);
            state = OBJECT_START;
            json_stack.push(current);
            current = {};
            current.type = JSON_TYPE::OBJECT;
        }
        else if (token.type == JSON_TOKEN_TYPE::LEFT_BRACKET) {
            state_stack.push(return_state);
            state = ARRAY_START;
            json_stack.push(current);
            current = {};
            current.type = JSON_TYPE::ARRAY;
        }
        else {
            switch (token.type) {
            case JSON_TOKEN_TYPE::STRING:
                current.children.push_back({token.string});
                break;
            case JSON_TOKEN_TYPE::NUMBER:
                current.children.push_back({token.number});
                break;
            default:
                current.children.push_back({static_cast<JSON_TYPE>(token.type)});
            }
            state = return_state;
        }
    }

    std::optional<json> parse(std::string_view string) {
        auto tokens_optional = tokenize(string);
        if (!tokens_optional.has_value()) {
            return std::nullopt;
        }

        auto tokens = tokens_optional.value();
        State state = DEFAULT;
        std::stack<State> state_stack = {};
        std::stack<json> json_stack = {};
        std::stack<std::string_view> key_stack = {};
        json current{};
        for (size_t i = 0; i < tokens.size(); i++) {
            auto &token = tokens.at(i);
            switch (state) {
            case DEFAULT:
                switch (token.type) {
                case JSON_TOKEN_TYPE::LEFT_BRACE:
                    state = OBJECT_START;
                    current.type = JSON_TYPE::OBJECT;
                    break;
                case JSON_TOKEN_TYPE::LEFT_BRACKET:
                    state = ARRAY_START;
                    current.type = JSON_TYPE::ARRAY;
                    break;
                case JSON_TOKEN_TYPE::NUMBER:
                    if (tokens.size() != 1) {
                        FAIL();
                    }
                    else {
                        return { token.number };
                    }
                    break;
                case JSON_TOKEN_TYPE::STRING:
                    if (tokens.size() != 1) {
                        FAIL();
                    }
                    else {
                        return { token.string };
                    }
                    break;
                case JSON_TOKEN_TYPE::TRUE:
                case JSON_TOKEN_TYPE::FALSE:
                case JSON_TOKEN_TYPE::NULL_WORD:
                    if (tokens.size() != 1) {
                        FAIL();
                    }
                    else {
                        return { static_cast<JSON_TYPE>(token.type) };
                    }
                    break;
                default:
                    FAIL();
                }
                break;
            case OBJECT_START:
                if (token.type == JSON_TOKEN_TYPE::RIGHT_BRACE) {
                    state = OBJECT_END;
                }
                else if (token.type == JSON_TOKEN_TYPE::STRING) {
                    state = OBJECT_KEY;
                    key_stack.push(token.string);
                }
                else {
                    FAIL();
                }
                break;
            case OBJECT_END:
                i--;
                if (state_stack.empty()) {
                    if (i != tokens.size() - 1) {
                        FAIL();
                    }
                }
                else {
                    state = state_stack.top();
                    state_stack.pop();
                    handle_json_return(current, json_stack, key_stack);
                }
                break;
            case OBJECT_KEY:
                if (token.type == JSON_TOKEN_TYPE::COLON) {
                    state = OBJECT_COLON;
                }
                else {
                    FAIL();
                }
                break;
            case OBJECT_COLON:
                if (token.type == JSON_TOKEN_TYPE::LEFT_BRACE) {
                    state_stack.push(OBJECT_VALUE);
                    state = OBJECT_START;
                    json_stack.push(current);
                    current = {};
                    current.type = JSON_TYPE::OBJECT;
                }
                else if (token.type == JSON_TOKEN_TYPE::LEFT_BRACKET) {
                    state_stack.push(OBJECT_VALUE);
                    state = ARRAY_START;
                    json_stack.push(current);
                    current = {};
                    current.type = JSON_TYPE::ARRAY;
                }
                else {
                    state = OBJECT_VALUE;
                    auto key = key_stack.top();
                    key_stack.pop();
                    if (token.type == JSON_TOKEN_TYPE::STRING) {
                        current.map[key] = { token.string };
                    }
                    else if (token.type == JSON_TOKEN_TYPE::NUMBER) {
                        current.map[key] = { token.number };
                    }
                    else if (token.type == JSON_TOKEN_TYPE::TRUE
                        || token.type == JSON_TOKEN_TYPE::FALSE
                        || token.type == JSON_TOKEN_TYPE::NULL_WORD
                    ){
                        current.map[key] = { static_cast<JSON_TYPE>(token.type) };
                    }
                    else {
                        FAIL();
                    }
                }
                break;
            case OBJECT_VALUE:
                if (token.type == JSON_TOKEN_TYPE::RIGHT_BRACE) {
                    state = OBJECT_END;
                }
                else if (token.type == JSON_TOKEN_TYPE::COMMA) {
                    state = OBJECT_COMMA;
                }
                else {
                    FAIL();
                }
                break;
            case OBJECT_COMMA:
                if (token.type == JSON_TOKEN_TYPE::STRING) {
                    state = OBJECT_KEY;
                    key_stack.push(token.string);
                }
                else {
                    FAIL();
                }
                break;
            case ARRAY_START:
                if (token.type == JSON_TOKEN_TYPE::RIGHT_BRACKET) {
                    state = ARRAY_END;
                }
                else if (is_value(token)) {
                    handle_value(token, ARRAY_VALUE, state, state_stack, current, json_stack);
                }
                else {
                    FAIL();
                }
                break;
            case ARRAY_END:
                i--;
                if (state_stack.empty()) {
                    if (i != tokens.size() - 1) {
                        FAIL();
                    }
                }
                else {
                    state = state_stack.top();
                    state_stack.pop();
                    handle_json_return(current, json_stack, key_stack);
                }
                break;
            case ARRAY_VALUE:
                if (token.type == JSON_TOKEN_TYPE::RIGHT_BRACKET) {
                    state = ARRAY_END;
                }
                else if (token.type == JSON_TOKEN_TYPE::COMMA) {
                    state = ARRAY_COMMA;
                }
                else {
                    FAIL();
                }
                break;
            case ARRAY_COMMA:
                if (is_value(token)) {
                    handle_value(token, ARRAY_VALUE, state, state_stack, current, json_stack);
                }
                else {
                    FAIL();
                }
                break;
            }
        }
        if ((state != ARRAY_END && state != OBJECT_END) || !state_stack.empty()) {
            FAIL();
        }
        return current;
    }
}
