#include "ResponseStream.hpp"

#include <unistd.h>

ResponseStream::ResponseStream()
    : seq_(0), totalRead_(0), totalWrite_(0), isEOF_(false) {}
ResponseStream::~ResponseStream() {
  std::list<Chunk *>::iterator it;
  for (it = list_.begin(); it != list_.end(); it++) {
    delete *it;
  }
}
ResponseStream::Chunk::Chunk(int seq) : seq_(seq), size_(0), offset_(0) {}

int ResponseStream::readStr(const std::string &str) {
  Chunk *chunk = new Chunk(seq_++);
  return str.size();
}

int ResponseStream::readFile(int fd) {
  if (isEOF_) {
    return 0;
  }
  if (list_.size() == CHUNK_LIST_SIZE) {
    return DELAYED_FILE_READ;
  }
  Chunk *chunk = new Chunk(seq_++);
  int size = read(fd, chunk->buffer_, BODY_CHUNK_SIZE);
  isEOF_ = size == 0 || (size > 0 && size < BODY_CHUNK_SIZE);
  if (size <= 0) {
    delete chunk;
    return size;
  }
  totalRead_ += size;
  chunk->size_ = size;
  list_.push_back(chunk);
  return size;
}

int ResponseStream::writeToClient(int fd) {
  if (list_.size() == 0) {
    return 0;
  }
  Chunk *chunk = list_.front();
  int left = chunk->size_ - chunk->offset_;
  int size = write(fd, chunk->buffer_ + chunk->offset_, left);
  if (size <= 0) {
    return size;
  }
  totalWrite_ += size;
  chunk->offset_ += size;
  if (chunk->size_ - chunk->offset_ == 0) {
    list_.pop_front();
    delete chunk;
  }
  return size;
}

int ResponseStream::getTotalRead() const { return totalRead_; }

int ResponseStream::getTotalWrite() const { return totalWrite_; }

bool ResponseStream::isEOF() const { return list_.size() == 0 && isEOF_; }

void ResponseStream::setEof(bool eof) { isEOF_ = eof; }
