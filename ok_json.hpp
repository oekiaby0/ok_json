#include <vector>
#include <string>
#include <string_view>
#include <optional>
#include <map>

namespace OK {
    enum class JSON_TYPE {
        TRUE_WORD = 1,
        FALSE_WORD,
        NULL_WORD,
        STRING,
        NUMBER,
        OBJECT,
        ARRAY,
    };

    struct json {
        JSON_TYPE type;
        std::unordered_map<std::string_view, json> map;
        std::vector<json> children;
        std::string_view string;
        long double number;

        ~json() {}

        json() {}

        json(JSON_TYPE type) {
            this->type = type;
            if (type == JSON_TYPE::OBJECT) {
                this->map = {};
            } else if (type == JSON_TYPE::ARRAY) {
                this->children = {};
            }
        }

        json(std::string_view &string) {
            this->type = JSON_TYPE::STRING;
            this->string = string;
        }

        json(long double number) {
            this->type = JSON_TYPE::NUMBER;
            this->number = number;
        }

        json(const json &other) {
            type = other.type;
            switch (type) {
                case JSON_TYPE::OBJECT:
                    map = other.map;
                    break;
                case JSON_TYPE::ARRAY:
                    children = other.children;
                    break;
                case JSON_TYPE::STRING:
                    string = other.string;
                    break;
                case JSON_TYPE::NUMBER:
                    number = other.number;
                    break;
                default:
                    break;
            }
        }

        std::string to_string() const;
    };

    std::optional<json> parse(std::string_view string);
}