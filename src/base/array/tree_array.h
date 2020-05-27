// About tree array strcut: https://www.cnblogs.com/xl2432/p/12974749.html

#ifndef SRC_BASE_ARRAY_TREE_ARRAY_H_
#define SRC_BASE_ARRAY_TREE_ARRAY_H_

#include <vector>

namespace base {

// Note: Index of array is from 1 to size()
template <typename T>
class TreeArray {
 public:
  TreeArray(const int size)
      : origin_data_(size + 1, 0), tree_data_(size + 1, 0), size_(size) {}

  ~TreeArray() = default;

  // Note: Start from 1
  T sum(int index) {
    int res = 0;

    while (index > 0) {
      res += tree_data_[index];
      index -= lowbit(index);
    }

    return res;
  }

  void add(int index, T data) {
    origin_data_[index] += data;

    while (index <= size_) {
      tree_data_[index] += data;
      index += lowbit(index);
    }
  }

  void set(int index, T data) { add(index, data - origin_data_[index]); }

  T get(int index) { return origin_data_[index]; }

  int size() { return size_; }

 private:
  int lowbit(const int num) { return num & (-num); }

  std::vector<T> origin_data_;
  std::vector<T> tree_data_;
  int size_;
};

}  // namespace base

#endif  // SRC_BASE_ARRAY_TREE_ARRAY_H_