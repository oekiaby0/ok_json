# ok_json: A Simple JSON Parser in C++

## Features

* No external dependencies.
* Easy-to-use API.

## Usage

**Parsing a JSON string:**

```c++
#include "ok_json.hpp"

int main() {
   std::string_view json_string = "{\"name\": \"John\", \"age\": 25, \"is_student\": true}";
   std::optional<OK::json> parsed_json = OK::parse(json_string);

   if (parsed_json.has_value()) {
       OK::json& json = parsed_json.value();
       // Access parsed data here
   } else {
       // Handle parsing error
   }
}
```

**Accessing parsed data:**

```c++
if (json.type == OK::JSON_TYPE::STRING) {
    std::string name = json.string();
} else if (json.type == OK::JSON_TYPE::NUMBER) {
    int age = json.number();
} else if (json.type == OK::JSON_TYPE::OBJECT) {
    OK::json obj2 = json["obj2"];
} else if (json.type == OK::JSON_TYPE::ARRAY) {
    int value = json[2].number();
}
```

**Constructing JSON data:**
```c++
OK::json example_json {{
    {"name", {"John"}},
    {"age", {25}},
    {"is_student", OK::json{OK::JSON_TYPE::TRUE}}
}};

std::string json_str = example_json.serialize(); // Serialize to JSON string
```

## Known Limitations
- It does not verify, escape and unescape both unicode and control characters (e.g., \u1234, \t).
- It may not cover all edge cases or conform to the JSON spec in every detail.

## More information
- See ok_json.hpp for documentation on functions and structures.
- GPL3 License.
- Contributions welcome!

