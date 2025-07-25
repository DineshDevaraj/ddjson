
/*
 *  Copyright [2025] <Dinesh Devaraj>
 */

#include <catch2/catch_test_macros.hpp>
#include <cstdint>
#include <string>

#include "header/doc.hpp"
#include "header/node.hpp"

using ddjson::Doc_t;
using ddjson::Node_t;
using ddjson::Valtype;
using std::string;

TEST_CASE("Validate JSON Parsing") {
  Doc_t jsonDoc;
  const char *jsonStr = R"({"int": 42})";
  Node_t &root = jsonDoc.parse_string(jsonStr);
  REQUIRE(root.child_count() == 1);
  REQUIRE(root.value_type() == Valtype::Object);
  REQUIRE(root.is_valid() == true);
}

TEST_CASE("Validate missing colon error") {
  Doc_t jsonDoc;
  const char *jsonStr = R"({"key" "value"})";
  string errMsg = "Expected colon `:`";
  Node_t &root = jsonDoc.parse_string(jsonStr);
  REQUIRE(root.is_valid() == false);
  REQUIRE(jsonDoc.error().desc == errMsg);
}

TEST_CASE("Validate missing closing brace error") {
  Doc_t jsonDoc;
  const char *jsonStr = R"({"key": "value")";
  string errMsg =
      "Expected colon `:`, comma `,` "
      "or close-symbol `}` but got `null` instead";
  Node_t &root = jsonDoc.parse_string(jsonStr);
  REQUIRE(root.is_valid() == false);
  REQUIRE(jsonDoc.error().desc == errMsg);
}

TEST_CASE("Validate int field") {
  Doc_t jsonDoc;
  const char *jsonStr = R"({"int": 42})";
  Node_t &root = jsonDoc.parse_string(jsonStr);
  REQUIRE(root["int"].is_valid() == true);
  REQUIRE(root["int"].value_type() == Valtype::Int);
  REQUIRE(static_cast<int>(root["int"]) == 42);
}

TEST_CASE("Validate float field") {
  Doc_t jsonDoc;
  const char *jsonStr = R"({"float": 3.14})";
  Node_t &root = jsonDoc.parse_string(jsonStr);
  REQUIRE(root["float"].is_valid() == true);
  REQUIRE(root["float"].value_type() == Valtype::Float);
  REQUIRE(static_cast<float>(root["float"]) == 3.14f);
}

TEST_CASE("Validate string field") {
  Doc_t jsonDoc;
  const char *jsonStr = R"({"string": "Hello, World!"})";
  Node_t &root = jsonDoc.parse_string(jsonStr);
  REQUIRE(root["string"].is_valid() == true);
  REQUIRE(root["string"].value_type() == Valtype::String);
  REQUIRE(static_cast<string>(root["string"]) == "Hello, World!");
}

TEST_CASE("Validate array field") {
  Doc_t jsonDoc;
  const char *jsonStr = R"({"array": [1, 2, 3]})";
  Node_t &root = jsonDoc.parse_string(jsonStr);
  REQUIRE(root["array"].is_valid() == true);
  REQUIRE(root["array"].value_type() == Valtype::Array);
  REQUIRE(root["array"].child_count() == 3);
  REQUIRE(static_cast<int>(root["array"][0]) == 1);
  REQUIRE(static_cast<int>(root["array"][1]) == 2);
  REQUIRE(static_cast<int>(root["array"][2]) == 3);
}

TEST_CASE("Validate object field") {
  Doc_t jsonDoc;
  const char *jsonStr = R"({"object": {"key": "value"}})";
  Node_t &root = jsonDoc.parse_string(jsonStr);
  REQUIRE(root["object"].is_valid() == true);
  REQUIRE(root["object"].value_type() == Valtype::Object);
  REQUIRE(root["object"]["key"].is_valid() == true);
  REQUIRE(root["object"]["key"].value_type() == Valtype::String);
  REQUIRE(static_cast<string>(root["object"]["key"]) == "value");
}

TEST_CASE("Validate boolean true field") {
  Doc_t jsonDoc;
  const char *jsonStr = R"({"bool": true})";
  Node_t &root = jsonDoc.parse_string(jsonStr);
  REQUIRE(root["bool"].is_valid() == true);
  REQUIRE(root["bool"].value_type() == Valtype::Bool);
  REQUIRE(static_cast<bool>(root["bool"]) == true);
}

TEST_CASE("Validate boolean false field") {
  Doc_t jsonDoc;
  const char *jsonStr = R"({"bool": false})";
  Node_t &root = jsonDoc.parse_string(jsonStr);
  REQUIRE(root["bool"].is_valid() == true);
  REQUIRE(root["bool"].value_type() == Valtype::Bool);
  REQUIRE(static_cast<bool>(root["bool"]) == false);
}

TEST_CASE("Validate null field") {
  Doc_t jsonDoc;
  const char *jsonStr = R"({"null": null})";
  Node_t &root = jsonDoc.parse_string(jsonStr);
  REQUIRE(root["null"].is_valid() == true);
  REQUIRE(root["null"].value_type() == Valtype::Null);
}

TEST_CASE("Validate empty object") {
  Doc_t jsonDoc;
  const char *jsonStr = R"({})";
  Node_t &root = jsonDoc.parse_string(jsonStr);
  REQUIRE(root.is_valid() == true);
  REQUIRE(root.value_type() == Valtype::Object);
  REQUIRE(root.child_count() == 0);
}

TEST_CASE("Validate empty array") {
  Doc_t jsonDoc;
  const char *jsonStr = R"({"empty_array": []})";
  Node_t &root = jsonDoc.parse_string(jsonStr);
  REQUIRE(root["empty_array"].is_valid() == true);
  REQUIRE(root["empty_array"].value_type() == Valtype::Array);
  REQUIRE(root["empty_array"].child_count() == 0);
}

TEST_CASE("Validate nested objects") {
  Doc_t jsonDoc;
  const char *jsonStr = R"({"outer": {"inner": {"key": "value"}}})";
  Node_t &root = jsonDoc.parse_string(jsonStr);
  REQUIRE(root["outer"].is_valid() == true);
  REQUIRE(root["outer"].value_type() == Valtype::Object);
  REQUIRE(root["outer"]["inner"].is_valid() == true);
  REQUIRE(root["outer"]["inner"].value_type() == Valtype::Object);
  REQUIRE(root["outer"]["inner"]["key"].is_valid() == true);
  REQUIRE(static_cast<string>(root["outer"]["inner"]["key"]) == "value");
}

TEST_CASE("Validate object inside array") {
  Doc_t jsonDoc;
  const char *jsonStr = R"({"array": [{"key": "value"}]})";
  Node_t &root = jsonDoc.parse_string(jsonStr);
  REQUIRE(root["array"].is_valid() == true);
  REQUIRE(root["array"].value_type() == Valtype::Array);
  REQUIRE(root["array"].child_count() == 1);
  REQUIRE(root["array"][0].is_valid() == true);
  REQUIRE(root["array"][0].value_type() == Valtype::Object);
  REQUIRE(root["array"][0]["key"].is_valid() == true);
  REQUIRE(static_cast<string>(root["array"][0]["key"]) == "value");
}

TEST_CASE("Validate array inside object") {
  Doc_t jsonDoc;
  const char *jsonStr = R"({"object": {"array": [1, 2, 3]}})";
  Node_t &root = jsonDoc.parse_string(jsonStr);
  REQUIRE(root["object"].is_valid() == true);
  REQUIRE(root["object"].value_type() == Valtype::Object);
  REQUIRE(root["object"]["array"].is_valid() == true);
  REQUIRE(root["object"]["array"].value_type() == Valtype::Array);
  REQUIRE(root["object"]["array"].child_count() == 3);
  REQUIRE(static_cast<int>(root["object"]["array"][0]) == 1);
  REQUIRE(static_cast<int>(root["object"]["array"][1]) == 2);
  REQUIRE(static_cast<int>(root["object"]["array"][2]) == 3);
}
