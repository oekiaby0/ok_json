#include <vector>
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
        union {
            std::map<std::string_view, json> map;
            std::vector<json> children;
            std::string_view string;
            long double number;
        };

        ~json() {}
        json() {}

        json(JSON_TYPE type) {
            this->type = type;
        }

        json(std::string_view string) {
            this->type = JSON_TYPE::STRING;
            this->string = string;
        }

        json(long double number) {
            this->type = JSON_TYPE::NUMBER;
            this->number = number;
        }

        json(const json &other) : type(other.type) {
          switch (this->type) {
          case JSON_TYPE::OBJECT:
            new(&this->map) std::map<std::string_view, json>(other.map);
            break;
          case JSON_TYPE::ARRAY:
            new(&this->children) std::vector<json>(other.children);
            break;
          case JSON_TYPE::STRING:
            this->string = other.string;
            break;
          case JSON_TYPE::NUMBER:
            this->number = other.number;
            break;
          default:
            break;
          }
        }
        
        
        void operator=(const json &other) {
            this->type = other.type;
            this->number = other.number;
        }
    };

    std::optional<json> parse(std::string_view string);
}