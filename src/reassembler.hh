#pragma once

#include "byte_stream.hh"
#include <deque>

class Reassembler
{
public:
  // Construct Reassembler to write into given ByteStream.
  explicit Reassembler( ByteStream&& output ) : output_( std::move( output ) ),
   assembler(), mask(), _first_unassem(0), _last_byte(0){}
  using LEN_T = uint64_t;
  /*
   * Insert a new substring to be reassembled into a ByteStream.
   *   `first_index`: the index of the first byte of the substring
   *   `data`: the substring itself
   *   `is_last_substring`: this substring represents the end of the stream
   *   `output`: a mutable reference to the Writer
   *
   * The Reassembler's job is to reassemble the indexed substrings (possibly out-of-order
   * and possibly overlapping) back into the original ByteStream. As soon as the Reassembler
   * learns the next byte in the stream, it should write it to the output.
   *
   * If the Reassembler learns about bytes that fit within the stream's available capacity
   * but can't yet be written (because earlier bytes remain unknown), it should store them
   * internally until the gaps are filled in.
   *
   * The Reassembler should discard any bytes that lie beyond the stream's available capacity
   * (i.e., bytes that couldn't be written even if earlier gaps get filled in).
   *
   * The Reassembler should close the stream after writing the last byte.
   */
  void insert( uint64_t first_index, std::string data, bool is_last_substring );

  // How many bytes are stored in the Reassembler itself?
  uint64_t bytes_pending() const;

  // Access output stream reader
  Reader& reader() { return output_.reader(); }
  const Reader& reader() const { return output_.reader(); }

  // Access output stream writer, but const-only (can't write from outside)
  const Writer& writer() const { return output_.writer(); }

private:
  ByteStream output_; // the Reassembler writes to this ByteStream

  deque<char> assembler; // a assembler to assemble strings 
  deque<bool> mask; // a assembler to assemble strings 

  LEN_T _first_unassem; // The first index of unassembled byte
  LEN_T _last_byte; //最后一位的序列号


public:
  bool finished() const {return FIN && _last_byte == _first_unassem;}
  bool not_connected(){return writer().is_closed() || output_.has_error();}
  LEN_T first_unaccess(){return _first_unassem + writer().available_capacity();}

  void push_assembled();
  void reset_assembler();
  void assembly(uint64_t first_index, string& data);

  // interface for tcp_receiver
  bool has_error() const {return output_.has_error();}
  void set_error()  { output_.set_error();}
  LEN_T ackno() const {return _first_unassem + SYN + finished();}
  LEN_T window_size() const {return writer().available_capacity();}
  void close() {output_.close_write();}

  bool SYN {false};
  bool FIN {false};
};
