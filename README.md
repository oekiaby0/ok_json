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
    std::string name = get<json_string>(json.value); // Get name string
} else if (json.type == OK::JSON_TYPE::NUMBER) {
    int age = get<json_number>(json.value);; // Get age number
} else if (json.type == OK::JSON_TYPE::OBJECT) {
    // Access object properties using get<json_object>(json.value)
} else if (json.type == OK::JSON_TYPE::ARRAY) {
    // Access array elements using get<json_array>(json.value)
}
```

**Constructing JSON data:**
```c++
OK::json example_json {OK::JSON_TYPE::OBJECT};
example_json.value = {
    {"name", OK::json("John")},
    {"age", OK::json(25)},
    {"is_student", OK::json(true)}
};

std::string json_str = example_json.to_string(); // Serialize to JSON string
```

## Known Limitations
- It does not verify and unescape Unicode control characters (e.g., \u1234).
- It may not cover all edge cases or conform to the JSON spec in every detail.

## More information
- See ok_json.hpp for documentation on functions and structures.
- GPL3 License.
- Contributions welcome!

