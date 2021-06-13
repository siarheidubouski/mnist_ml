#pragma once

#include <fstream>
#include <cstdint>
#include <vector>
#include <string>
#include <map>
#include <unordered_set>
#include <memory>
#include <iostream>
#include <cassert>
#include "data.hpp"

/*
*  Read the data, count the number of unique classes, convert to little endian,
*  pass around of traned data and validation data.
*/

class data_handler {
  private:
    using data_ptr = std::shared_ptr<data>;
    using data_vector = std::vector<data_ptr>;

    std::shared_ptr<data_vector> data_array; // all the data(pre-split)
    std::shared_ptr<data_vector> training_data;
    std::shared_ptr<data_vector> test_data;
    std::shared_ptr<data_vector> validation_data;

    int num_classes;
    int feature_vector_size;
    std::map<std::uint8_t, int> class_map;
    
    static constexpr double K_TRAIN_SET_PERCENT  = 0.75;
    static constexpr double K_TEST_SET_PERCENT   = 0.20;
    static constexpr double K_VALIDATION_PERCENT = 0.05;
    
  public:

    data_handler();

    void read_feature_vector(const std::string& path);
    void read_feature_labels(const std::string& path);
    void split_data();
    void count_classes();

    std::uint32_t convert_to_little_indian(const unsigned char* bytes);
    std::shared_ptr<data_vector> get_training_data() const;
    std::shared_ptr<data_vector> get_test_data() const;
    std::shared_ptr<data_vector> get_validation_data() const;
};