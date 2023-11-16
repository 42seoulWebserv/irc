#ifndef BodyStream_HPP_
#define BodyStream_HPP_

#include <list>
#include <string>

#define BODY_CHUNK_SIZE 65536
#define CHUNK_LIST_SIZE 4
#define DELAYED_FILE_READ -2

class ResponseStream {
 public:
  ResponseStream();
  ~ResponseStream();
  int readStr(const std::string &str);
  int readFile(int fd);
  int writeToClient(int fd);
  int getTotalRead() const;
  int getTotalWrite() const;
  bool isEOF() const;
  void setEof(bool eof);

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
  int totalRead_;
  int totalWrite_;
  bool isEOF_;
  std::list<Chunk *> list_;
};

#endif