#include <functional>
#include <iostream>
#include <vector>

// Data User
enum UserType { LISTENER, ADMIN };
struct User {
  size_t id;
  std::string username, password;
  UserType type;
};

// Data Lagu
struct Song {
  static size_t id_counter;
  size_t id;
  std::string title, artist, genre;
  size_t duration, playCount;

  Song() = default;
  Song(const std::string& title, const std::string& artist,
       const std::string& genre, int duration)
      : title(title), artist(artist), genre(genre), duration(duration) {}

  bool operator==(const Song& rhs) const {
    return title == rhs.title && artist == rhs.artist;
  }

  bool operator!=(const Song& rhs) const { return !(*this == rhs); }
};

template <typename T>
struct Node {
  T data;
  Node<T>* previous;
  Node<T>* next;

  Node() = default;
  Node(const T& data) : data(data), next(nullptr), previous(nullptr) {}
};

template <typename T>
class LinkedList {
 public:
  LinkedList() : head_(nullptr), tail_(nullptr), node_counter_(0) {}

  int count() { return node_counter_; }

  void push(const T& data) {
    Node<T>* new_node = new Node<T>(data);
    if (isEmpty()) {
      node_counter_++;
      head_ = tail_ = new_node;
      return;
    }

    tail_->next = new_node;
    new_node->previous = tail_;
    tail_ = new_node;

    node_counter_++;
  }

  void insert(const T& data, int position = 0) {
    Node<T>* new_node = new Node<T>(data);
    node_counter_++;

    if (isEmpty()) {
      head_ = tail_ = new_node;
      return;
    }

    if (position <= 0) {
      new_node->next = head_;
      head_->previous = new_node;
      head_ = new_node;
      return;
    }

    Node<T>* iterator = head_;

    for (size_t i = 0; i < position - 1 && isNotNull(iterator); i++) {
      iterator = iterator->next;
    }

    if (isNull(iterator)) {
      push(data);
      delete new_node;
      return;
    }

    new_node->next = iterator->next;
    new_node->previous = iterator;

    if (isNotNull(new_node->next)) {
      new_node->next->previous = new_node;
    } else {
      tail_ = new_node;
    }

    iterator->next = new_node;
  }

  void deleteNode(const T& data) {
    if (isEmpty()) return;

    Node<T>* targe_node = head_;
    while (isNotNull(target_node) && targe_node->data != data) {
      targe_node = targe_node->next;
    }

    if (isNull(targe_node)) return;

    if (isNotNull(targe_node->previous)) {
      targe_node->previous->next = targe_node->next;
    } else {
      head_ = targe_node->next;
    }

    if (targe_node->next) {
      targe_node->next->previous = targe_node->previous;
    } else {
      tail_ = targe_node->previous;
    }

    delete targe_node;

    node_counter_--;
  }

  void reverse() {
    if (isEmpty()) return;

    Node<T>* previous_node = nullptr;
    Node<T>* current_node = head_;
    Node<T>* next_node = nullptr;

    tail_ = head_;

    while (isNotNull(current)) {
      next_node = current->next;
      current_node->next = previous_node;
      current_node->previous = next_node;

      previous_node = current_node;
      current_node = next_node;
    }

    head_ = previous_node;
  }

  void clear() {
    Node<T>* current{head_};

    while (isNotNull(current)) {
      Node<T>* next_node = current->next;
      delete current;
      current = next_node;
    }

    head_ = tail_ = nullptr;
    node_counter_ = 0;
  }

 private:
  Node<T>* head_;
  Node<T>* tail_;
  size_t node_counter_;

  bool isNull(Node<T>* node) { return !node; }
  bool isNotNull(Node<T>* node) { return node; }
  bool isEmpty() { return !head_; }
};

class SongLibrary {
 public:
  const std::vector<Song>& library() { return library_; }

  void addToLibrary(const Song& song) {
    library_.push_back(song);
    SongSorter::ensureSortedByID(library_);
  }

  void removeSongById(int target_id) {
    int target_index{-1};
    if (SongSearcher::binarySearch(target_id, library_, &target_index)) {
      library_.erase(std::remove_if(library_.begin(), library_.end(),
                                    [target_id](const Song& song) {
                                      return song.id == target_id;
                                    }),
                     library_.end());
    }
  }

 private:
  std::vector<Song> library_;
};

class SongSearcher {
 public:
  static std::vector<Song> linearSearch(
      std::vector<Song>& data, std::function<bool(const Song&)> condition) {}

  static bool binarySearch(size_t target_id, std::vector<Song>& data,
                           int* result_index) {
    SongSorter::ensureSortedByID(data);

    size_t start{0}, mid{0}, end{data.size() - 1};
    bool found = false;

    while (!found && start <= end) {
      mid = (start + end) / 2;

      if (target_id = data[mid].id) {
        *result_index = mid;
        found = true;
        break;
      }
    }
  }
};

class SongSorter {
 public:
  static inline auto by_title = [](const Song& a, const Song& b) {
    return a.title < b.title;
  };

  static inline auto by_id = [](const Song& a, const Song& b) {
    return a.id < b.id;
  };

  static inline auto by_play_count = [](const Song& a, const Song& b) {
    return a.playCount < b.playCount;
  };

  static void ensureSortedByID(std::vector<Song>& data) {
    if (data.size() > 30) {
      quickSort(data, 0, data.size() - 1, by_id);
    } else {
      bubbleSort(data, by_id);
    }
  }

  static void bubbleSort(
      std::vector<Song>& data,
      std::function<bool(const Song&, const Song&)> comparator) {
    if (data.empty()) return;

    for (size_t i = 0; i < data.size(); ++i) {
      for (size_t j = 0; j < data.size() - i - 1; ++j) {
        if (comparator(data[j + 1], data[j])) {
          swap(&data[j], &data[j + 1]);
        }
      }
    }
  }

  static void quickSort(
      std::vector<Song>& data, size_t awal, size_t akhir,
      std::function<bool(const Song&, const Song&)> comparator) {
    if (awal < akhir) {
      int pivot = partition(data, awal, akhir, comparator);
      if (pivot > awal) quickSort(data, awal, pivot - 1, comparator);
      if (pivot < akhir) quickSort(data, pivot + 1, akhir, comparator);
    }
  }

 private:
  static size_t partition(
      std::vector<Song>& data, size_t index_awal, size_t index_akhir,
      std::function<bool(const Song&, const Song&)> comparator) {
    Song pivot = data[index_akhir];
    size_t i = index_awal - 1;

    for (size_t j = index_awal; j < index_akhir; j++) {
      if (comparator(data[j], pivot)) {
        i++;
        swap(&data[j], &data[i]);
      }
    }

    swap(&data[i + 1], &data[index_akhir]);

    return (i + 1);
  }

  static void swap(Song* data1, Song* data2) {
    Song temporary = *data1;
    *data1 = *data2;
    *data2 = temporary;
  }
};

class Playlist {};

// bool binarySearch(size_t id_dicari, std::vector<Song>& data,
//   size_t* result_index) {
// if (!is_sorted) {
// if (data.size() > 30) {
// quickSort(data, 0, data.size() - 1);
// } else {
// insertionSort(data);
// }
// is_sorted = !is_sorted;
// }

// size_t start{0}, mid{0}, end{data.size() - 1};
// bool found = false;
// while (!found && start <= end) {
// mid = (start + end) / 2;

// if (id_dicari == data[mid].id) {
// *result_index = mid;
// found = true;
// break;
// }

// if (id_dicari > data[mid].id) {
// start = mid + 1;
// } else if (id_dicari < data[mid].id) {
// end = mid - 1;
// }
// }

// return found;
// }

// void insertionSort(std::vector<Song>& data) {
// for (int i = 1; i < data.size(); i++) {
// int j{i};
// while (j > 0 && data[j].id < data[j - 1].id) {
// swap(&data[j], &data[j - 1]);
// j--;
// }
// }
// }

class ConsoleUI {
 public:
  static void clearScreen() {
#ifdef __WIN32__
    system("cls");
#else
    system("clear");
#endif
  }
};

size_t Song::id_counter = 1;
int main() {
  ConsoleUI::clearScreen();
  return 0;
}