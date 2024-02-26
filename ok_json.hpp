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
        TRUE,
        FALSE,
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

        explicit json(JSON_TYPE type) {
            this->type = type;
            if (type == JSON_TYPE::OBJECT) {
                this->value = std::unordered_map<std::string_view, json>{};
            } else if (type == JSON_TYPE::ARRAY) {
                this->value = std::vector<json>{};
            }
        }

        json(json_object mp) {
            this->type = JSON_TYPE::OBJECT;
            this->value = mp;
        }

        json(json_array arr) {
            this->type = JSON_TYPE::ARRAY;
            this->value = arr;
        }

        json(std::string_view string) {
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

        std::string serialize() const;

        json& operator [](std::string key) {
            return get<json_object>(this->value)[key];
        }

        json& operator [](int index) {
            return get<json_array>(this->value)[index];
        }

        json_number number() {
            return get<json_number>(this->value);
        }

        json_string string() {
            return get<json_string>(this->value);
        }

        size_t size() {
            if (type == JSON_TYPE::OBJECT) {
                return get<json_object>(this->value).size();
            } else if (type == JSON_TYPE::ARRAY) {
                return get<json_array>(this->value).size();
            }
            return static_cast<size_t>(0);
        }
    };

    std::optional<json> parse(std::string_view string);
    std::optional<json> parse(std::istream& stream);
}