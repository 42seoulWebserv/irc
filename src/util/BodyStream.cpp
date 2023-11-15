#include "BodyStream.hpp"

#include <unistd.h>

BodyStream::BodyStream()
    : seq_(0), totalFileRead_(0), totalFileWrite_(0), isReadEOF_(false) {}
BodyStream::~BodyStream() {
  std::list<Chunk *>::iterator it;
  for (it = list_.begin(); it != list_.end(); it++) {
    delete *it;
  }
}
BodyStream::Chunk::Chunk(int seq) : seq_(seq), size_(0), offset_(0) {}

int BodyStream::readFrom(int fd) {
  if (list_.size() == CHUNK_LIST_SIZE) {
    return DELAYED_FILE_READ;
  }
  Chunk *chunk = new Chunk(seq_++);
  int size = read(fd, chunk->buffer_, BODY_CHUNK_SIZE);
  isReadEOF_ = size == 0 || (size > 0 && size < BODY_CHUNK_SIZE);
  if (size <= 0) {
    delete chunk;
    return size;
  }
  totalFileRead_ += size;
  chunk->size_ = size;
  list_.push_back(chunk);
  return size;
}

int BodyStream::writeTo(int fd) {
  if (list_.size() == 0) {
    return 0;
  }
  Chunk *chunk = list_.front();
  int left = chunk->size_ - chunk->offset_;
  int size = write(fd, chunk->buffer_ + chunk->offset_, left);
  if (size <= 0) {
    return size;
  }
  totalFileWrite_ += size;
  chunk->offset_ += size;
  if (chunk->size_ - chunk->offset_ == 0) {
    list_.pop_front();
    delete chunk;
  }
  return size;
}

int BodyStream::getTotalFileRead() const { return totalFileRead_; }

int BodyStream::getTotalFileWrite() const { return totalFileWrite_; }

bool BodyStream::isEOF() const { return list_.size() == 0 && isReadEOF_; }
