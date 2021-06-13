#pragma once

#include <vector>
#include <cstdint>
#include <cstdio>
#include <memory>

class data {
  
  private:
    using vec_of_uint8 = std::vector<uint8_t>;
    std::shared_ptr<vec_of_uint8> feature_vector; // No class at end
    std::uint8_t label;
    int enum_label;  // A  -> 1, B -> 2

  public:
    data();
    void set_feature_vector(std::shared_ptr<vec_of_uint8>);
    void append_to_feature_vector(std::uint8_t);
    void set_label(std::uint8_t);
    void set_enumerated_label(int);
    
    int get_feature_vector_size() const;
    std::uint8_t get_label() const;
    std::uint8_t get_enumerated_label() const;
    std::shared_ptr<vec_of_uint8> get_feature_vector() const;
};