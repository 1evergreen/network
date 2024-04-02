#include "reassembler.hh"
#include <numeric>
using namespace std;
void Reassembler::push_assembled(){
  string s = "";
  while(!mask.empty() && mask.front()){
    mask.pop_front();
    ++_first_unassem;
    s += assembler.front();
    assembler.pop_front();
  }
  output_.write(s);
}
void Reassembler::reset_assembler(){
  assembler.resize(writer().available_capacity());
  mask.resize(writer().available_capacity());
}

void Reassembler::assembly(uint64_t first_index, string& data){
  LEN_T start = max(first_index, _first_unassem);
  LEN_T end = min(first_index + data.size(), first_unaccess());
  if(start >=  end){return;}
  
  for(LEN_T i=start; i<end; ++i){
    mask[i - _first_unassem] = true;
    assembler[i - _first_unassem] = data[i - first_index];
  }
}

void Reassembler::insert( uint64_t first_index, string data, bool is_last_substring )
{
  // Your code here.
  if(!not_connected() ){
    reset_assembler();
    assembly(first_index, data);
    push_assembled();
    if(is_last_substring){
      FIN = true;
      _last_byte = first_index + data.size();
    }
    if(finished()){
      output_.close_write();
    }
  }

}

uint64_t Reassembler::bytes_pending() const
{
  // Your code here.
  return std::accumulate(mask.begin(), mask.end(), 0ULL);
}
