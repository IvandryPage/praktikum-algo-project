#include <iostream>

// Data User
enum UserType { ADMIN, LISTENER };
struct User {
  size_t id;
  std::string username, password;
  UserType type;
};

// Data Lagu
struct Song {
  size_t id;
  std::string title, artist, genre;
  size_t duration, playCount;
  float rating;

  Song() = default;
  Song(const std::string& title, const std::string& artist,
       const std::string& genre, int duration)
      : title(title), artist(artist), genre(genre), duration(duration) {}

  // Operator overloading => biar bisa bandingin Object Song secara langsung
  // TODO: == sama < atau >

  bool operator==(const Song& rhs) const {
    return title == rhs.title && artist == rhs.artist;
  }

  bool operator!=(const Song& rhs) const { return !(*this == rhs); }
};

// Implementasi Double Linked List
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
  LinkedList() : head(nullptr), tail(nullptr), node_counter(0) {}

  int count() { return node_counter; }

  // Sisip Belakang
  void push(const T& data) {
    Node<T>* new_node = new Node<T>(data);
    if (isEmpty()) {
      node_counter++;
      head = tail = new_node;
      return;
    }

    // Constructor Node : previous and next default is nullptr
    tail->next = new_node;
    new_node->previous = tail;
    tail = new_node;

    node_counter++;
  }

  void insert(const T& data, int position = 0) {
    node_counter++;
    if (isEmpty()) {
      Node<T>* new_node = new Node<T>(data);
      head = tail = new_node;
      return;
    }
    // Error Handling position < 0 => bakal nambahin di depan
    if (position <= 0) {
      Node<T>* new_node = new Node<T>(data);
      new_node->next = head;
      head->previous = new_node;
      head = new_node;
      return;
    }

    Node<T>* iterator = head;

    for (size_t i = 0; i < position - 1 && iterator; i++) {
      iterator = iterator->next;
    }

    // jika posisi yang diberikan lebih dari data yang ada, maka node ditambah
    // di belakang
    if (!iterator) {
      push(data);
      return;
    }

    Node<T>* new_node = new Node<T>(data);
    new_node->next = iterator->next;
    new_node->previous = iterator;
    if (new_node->next) new_node->next->previous = new_node;
    iterator->next = new_node;
    if (!new_node->next) tail = new_node;
  }

  void deleteNode(const T& data) {
    if (isEmpty()) return;  // data kosong
    Node<T>* delete_node = head;

    while (delete_node && delete_node->data != data) {
      delete_node = delete_node->next;
    }

    if (!delete_node) return;  // data tidak ditemukan

    if (delete_node->previous)
      delete_node->previous->next = delete_node->next;
    else
      head = delete_node->next;

    if (delete_node->next)
      delete_node->next->previous = delete_node->previous;
    else
      tail = delete_node->previous;

    delete delete_node;

    node_counter--;
  }

  // TODO: Reverse the linked list (make sure the program isn't crash)
  void reverse() {}

 private:
  Node<T>* head;
  Node<T>* tail;
  size_t node_counter;

  bool isEmpty() { return !head; }
};

// TODO: make the file name dynamic based on user input or playlist name
class File {
 public:
  const char* kFileName = "songs.txt";  //
  File() : file_ptr(nullptr) {};

  void save(Song* data, size_t& data_size) {
    file_ptr = fopen(kFileName, "w");

    if (file_ptr == NULL) {
      std::cerr << "Can't find file!\n";
      exit(-1);
    }

    fwrite(data, sizeof(Song), data_size, file_ptr);
    fclose(file_ptr);
  }

  void load(Song* data, size_t* data_size) {
    file_ptr = fopen(kFileName, "r");

    if (file_ptr == NULL) {
      file_ptr = fopen(kFileName, "w");
      fclose(file_ptr);
      file_ptr = fopen(kFileName, "r");
    }

    fseek(file_ptr, 0, SEEK_END);
    size_t file_size = ftell(file_ptr);
    rewind(file_ptr);

    *data_size = file_size / sizeof(Song);

    fread(data, sizeof(Song), *data_size, file_ptr);
    fclose(file_ptr);
  }

  void delete_file() {
#ifdef __WIN32__
    system("del database-SI.txt");
#else
    system("rm -f database-SI.txt");
#endif
  }

 private:
  FILE* file_ptr;
};

int main() {
  Song s1 = Song("judul", "pengarang", "pop", 180);
  Song s2 = Song("judul", "pengarang", "pop", 180);

  if (s1 != s2)
    std::cout << "Beda lagu";
  else
    std::cout << "Sama bro";
  return 0;
}