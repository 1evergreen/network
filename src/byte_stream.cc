#include "byte_stream.hh"

using namespace std;

ByteStream::ByteStream( uint64_t capacity ) : capacity_( capacity ) ,closed_(false),
total_pushed_(0), total_poped_(0), buffer(){}

bool Writer::is_closed() const
{
  // Your code here.
  return closed_;
}

void Writer::push( string data )
{
  // Your code here.
  if(is_closed() || has_error() || data.empty()){
    return;
  }
  LEN_T l = min(data.size(), available_capacity());
  buffer.append(data.substr(0, l));
  total_pushed_ += l;
  return;
}

void Writer::close()
{
  // Your code here.
  closed_ = true;
}

uint64_t Writer::available_capacity() const
{
  // Your code here.
  return capacity_ - buffer.size();
}

uint64_t Writer::bytes_pushed() const
{
  // Your code here.
  return total_pushed_;
}

bool Reader::is_finished() const
{
  // Your code here.
  return closed_ && bytes_buffered() == 0;
}

uint64_t Reader::bytes_popped() const
{
  // Your code here.
  return total_poped_;
}

string_view Reader::peek() const
{
  // Your code here.
  return {buffer};
}

void Reader::pop( uint64_t len )
{
  // Your code here.
  LEN_T l = min(len, buffer.size());
  buffer.erase(0, l);
  total_poped_ += l;
}

uint64_t Reader::bytes_buffered() const
{
  // Your code here.
  return buffer.size();
}
