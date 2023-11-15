#ifndef BodyStream_HPP_
#define BodyStream_HPP_

#include <list>
#include <string>

#define BODY_CHUNK_SIZE 65536
#define CHUNK_LIST_SIZE 4
#define DELAYED_FILE_READ -2

class BodyStream {
 public:
  BodyStream();
  ~BodyStream();
  int readFrom(int fd);
  int writeTo(int fd);
  int getTotalFileRead() const;
  int getTotalFileWrite() const;
  bool isEOF() const;

 private:
  class Chunk {
   public:
    Chunk(int seq);
    int seq_;
    int size_;
    int offset_;
    char buffer_[BODY_CHUNK_SIZE];
  };
  int seq_;
  int totalFileRead_;
  int totalFileWrite_;
  bool isReadEOF_;
  std::list<Chunk *> list_;
};

#endif