#include "wrapping_integers.hh"

using namespace std;

Wrap32 Wrap32::wrap( uint64_t n, Wrap32 zero_point )
{
  // Your code here.
  uint32_t val = static_cast<uint32_t>(n + zero_point.raw_value_);
  return Wrap32(val);
}

uint64_t Wrap32::unwrap( Wrap32 zero_point, uint64_t checkpoint ) const
{
  // Your code here.
  auto check = Wrap32::wrap(checkpoint, zero_point);
  uint32_t gap = raw_value_ - check.raw_value_;
  uint64_t abs_seqno = gap + checkpoint;
  if(abs_seqno >= (1ULL << 32) && abs_seqno - checkpoint > (1ULL << 31)){
    return abs_seqno - (1ULL << 32);
  }
  return abs_seqno;
}
