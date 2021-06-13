#include "../include/data.hpp"

using vec_of_uint8 = std::vector<uint8_t>;

data::data() {
  feature_vector = std::make_shared<vec_of_uint8>();
}

void data::set_feature_vector(std::shared_ptr<vec_of_uint8> feature_vector) {
  this->feature_vector = feature_vector;
}

void data::append_to_feature_vector(std::uint8_t val) {
  feature_vector->push_back(val);
}

void data::set_label(std::uint8_t label) {
  this->label = label;
}

void data::set_enumerated_label(int enum_label) {
  this->enum_label = enum_label;
}
    
int data::get_feature_vector_size() const {
  return static_cast<int>(feature_vector->size());
}

std::uint8_t data::get_label() const {
  return label;
}

std::uint8_t data::get_enumerated_label() const {
  return enum_label;
}

std::shared_ptr<vec_of_uint8> data::get_feature_vector() const {
  return feature_vector;
}
