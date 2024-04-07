#include "tcp_receiver.hh"
#include <optional>

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
  if(message.FIN){
    FIN = true;
    reassembler_.set_FIN();
  }

  if(reassembler_.finished()){
    reassembler_.close();
  }
  // if(reassembler_.connected()){

  // }


}

TCPReceiverMessage TCPReceiver::send() const
{
  // Your code here.
  std::optional<Wrap32> ackno = SYN ? std::optional<Wrap32>(isn_ + (reassembler_.ackno() + FIN)) :std::nullopt; 
  uint16_t window_size = reassembler_.window_size();
  bool RST = reassembler_.has_error();
  return TCPReceiverMessage{ackno, window_size, RST};
}
