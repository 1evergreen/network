#include "tcp_receiver.hh"
#include <optional>
#include <cstdint>

using namespace std;

void TCPReceiver::receive( TCPSenderMessage message )
{
  // Your code here.
  (void)message;

  // if(message.RST){

  // }
  if(message.SYN){
    SYN = true;
    isn_ = message.seqno;
  }
  if(SYN){
    if(message.FIN){
      // FIN = true;
      reassembler_.set_FIN();
    }

    if(reassembler_.finished()){
      reassembler_.close();
    }
  }

  // if(reassembler_.connected()){

  // }


}

TCPReceiverMessage TCPReceiver::send() const
{
  // Your code here.
  std::optional<Wrap32> ackno = SYN ? std::optional<Wrap32>(isn_ + reassembler_.ackno()) :std::nullopt; 
  LEN_T w_size = reassembler_.window_size();
  uint16_t window_size = w_size > UINT16_MAX? UINT16_MAX : w_size;
  bool RST = reassembler_.has_error();
  return TCPReceiverMessage{ackno, window_size, RST};
}
