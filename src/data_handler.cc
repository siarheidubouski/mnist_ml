#include "../include/data_handler.hpp"

using data_ptr = std::shared_ptr<data>;
using data_vector = std::vector<data_ptr>;

data_handler::data_handler() {
  data_array      = std::make_shared<data_vector>();
  test_data       = std::make_shared<data_vector>();
  training_data   = std::make_shared<data_vector>();
  validation_data = std::make_shared<data_vector>();
}

void data_handler::read_feature_vector(const std::string& path) {
  int length = 4;
  std::uint32_t header[length]; // |MAGIC|NUM IMAGES|ROWSIZE|COLSIZE
  unsigned char bytes[length];
  
  FILE* file = fopen(path.c_str(), "r");
  
  if (file) {
    for (int i = 0; i < 4; i++) {
      if (fread(bytes, length, 1, file)) {
        header[i] = convert_to_little_indian(bytes);
      }
    }

    std::cout << "Done getting Input File Header" << '\n';

    int image_size = header[2] * header[3];
    
    for (int i = 0; i < header[1]; i++) {
      std::shared_ptr<data> d = std::make_shared<data>();
      std::uint8_t element[1];

      for (int j = 0; j < image_size; j++) {
        if (fread(element, sizeof(element), 1, file)) {
          d->append_to_feature_vector(element[0]);
        } else {
          assert(("Error reading from file", false));
        }
      }

      data_array->push_back(d);
    }

    std::cout << "Successfully read and stored " << data_array->size()
              << " feature vectors\n";
  } else {
    assert(("Can not find a file", false));
  }

}

void data_handler::read_feature_labels(const std::string& path) {
  int length = 2;
  std::uint32_t header[length]; // |MAGIC|NUM IMAGES|
  unsigned char bytes[length * length];
  
  FILE* file = fopen(path.c_str(), "r");
  
  if (file) {
    for (int i = 0; i < length; i++) {
      if (fread(bytes, sizeof(bytes), 1, file)) {
        header[i] = convert_to_little_indian(bytes);
      }
    }
    
    std::cout << "Done getting Label File Header" << '\n';
 
    for (int i = 0; i < header[1]; i++) {
      std::uint8_t element[1];

      if (fread(element, sizeof(element), 1, file)) {
        data_array->at(i)->set_label(element[0]);
      } else {
        assert(("Error reading from file", false));
      }
    }

    std::cout << "Successfully read and stored label" << std::endl;
  
  } else {
    assert(("Can not find a file", false));
  }
}

void data_handler::split_data() {
  std::unordered_set<int> used_indexes;
  int train_size = data_array->size() * K_TRAIN_SET_PERCENT;
  int test_size  = data_array->size() * K_TEST_SET_PERCENT;
  int valid_size = data_array->size() * K_VALIDATION_PERCENT;

  // Training data
  int count = 0;
  while (count < train_size) {
    int rand_index = rand() % data_array->size(); // 0 and data_array->size() - 1
    if (used_indexes.find(rand_index) == used_indexes.end()) {
      training_data->push_back(data_array->at(rand_index));
      used_indexes.insert(rand_index);
      count++;
    }
  }

  // Test data
  count = 0;
  used_indexes.clear();
  while (count < test_size) {
    int rand_index = rand() % data_array->size(); // 0 and data_array->size() - 1
    if (used_indexes.find(rand_index) == used_indexes.end()) {
      test_data->push_back(data_array->at(rand_index));
      used_indexes.insert(rand_index);
      count++;
    }
  }

  // Validation data
  count = 0;
  used_indexes.clear();
  while (count < valid_size) {
    int rand_index = rand() % data_array->size(); // 0 and data_array->size() - 1
    if (used_indexes.find(rand_index) == used_indexes.end()) {
      validation_data->push_back(data_array->at(rand_index));
      used_indexes.insert(rand_index);
      count++;
    }
  } 
  
  std::cout << "Training data size   " << training_data->size() << '\n';
  std::cout << "Test data size       " << test_data->size() << '\n';
  std::cout << "Validation data size " << validation_data->size() << '\n';
}

void data_handler::count_classes() {
  int count = 0;

  for (unsigned i = 0; i < data_array->size(); i++) {
    if (class_map.find(data_array->at(i)->get_label()) == class_map.end()) {
      class_map[data_array->at(i)->get_label()] = count;
      data_array->at(i)->set_enumerated_label(count);
      count++;
    }
  }

  num_classes = count;
  std::cout << "Successfully extracted " << num_classes << " unique classes\n";
}

std::uint32_t data_handler::convert_to_little_indian(const unsigned char* bytes) {
  return (std::uint32_t) ((bytes[0] << 24) | 
                          (bytes[1] << 16) |
                          (bytes[2] << 8)  |
                          (bytes[3]));
}

std::shared_ptr<data_vector> data_handler::get_training_data() const {
  return training_data;
}

std::shared_ptr<data_vector> data_handler::get_test_data() const {
  return test_data;
}

std::shared_ptr<data_vector> data_handler::get_validation_data() const {
  return validation_data;
}
