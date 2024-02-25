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
       OK::json& json_data = parsed_json.value();
       // Access parsed data here
   } else {
       // Handle parsing error
   }
}
```

**Accessing parsed data:**

```c++
if (json_data.type == OK::JSON_TYPE::STRING) {
    std::string name = json_data.string; // Get name string
} else if (json_data.type == OK::JSON_TYPE::NUMBER) {
    int age = json_data.number; // Get age number
} else if (json_data.type == OK::JSON_TYPE::OBJECT) {
    // Access object properties using json_data.map
} else if (json_data.type == OK::JSON_TYPE::ARRAY) {
    // Access array elements using json_data.children
}
```

**Constructing JSON data:**
```c++
OK::json example_json;
example_json.type = OK::JSON_TYPE::OBJECT;
example_json.map = {
    {"name", OK::json("John")},
    {"age", OK::json(25)},
    {"is_student", OK::json(true)}
};

std::string json_str = example_json.to_string(); // Serialize to JSON string
```

## Known Limitations
- It does not verify and convert Unicode control characters (e.g., \u1234).
- It may not cover all edge cases or conform to the JSON spec in every detail.

## More information
- See ok_json.hpp for documentation on functions and structures.
- GPL3 License.
- Contributions welcome!

