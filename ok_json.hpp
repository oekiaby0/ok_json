#include <vector>
#include <string>
#include <string_view>
#include <optional>
#include <map>

namespace OK {
    enum class JSON_TYPE {
        OBJECT,
        ARRAY,
        STRING,
        NUMBER,
        TRUE_WORD,
        FALSE_WORD,
        NULL_WORD,
    };

    struct json;
    typedef std::unordered_map<std::string_view, json> json_object;
    typedef std::vector<json> json_array;
    typedef long double json_number;
    typedef std::string_view json_string;
    typedef std::variant<json_object, json_array, json_string, json_number> json_value;

    struct json {
        JSON_TYPE type;
        json_value value;

        ~json() {}

        json() {}

        json(JSON_TYPE type) {
            this->type = type;
            if (type == JSON_TYPE::OBJECT) {
                this->value = std::unordered_map<std::string_view, json>{};
            } else if (type == JSON_TYPE::ARRAY) {
                this->value = std::vector<json>{};
            }
        }

        json(std::string_view &string) {
            this->type = JSON_TYPE::STRING;
            this->value = string;
        }

        json(long double number) {
            this->type = JSON_TYPE::NUMBER;
            this->value = number;
        }

        json(const json &other) {
            type = other.type;
            value = other.value;
        }

        std::string to_string() const;
    };

    std::optional<json> parse(std::string_view string);
}