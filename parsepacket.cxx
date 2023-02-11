#include <cstdint>
#include <iomanip>
#include <iostream>
#include <ostream>

using std::cout, std::endl;
using byte = uint8_t;   // Chose not to use std::byte here.
                        // To init a buffer from literals requires ugly cast of each value :-(


// TODO:  Move to packet.hxx
typedef struct Packet {
    enum class Flag_bits {
        ENCRYPTED = 0b0000'0001,
        // reserved 0b0000'0010,
        SIGNED    = 0b0000'0100,
        // reserved 0b0000'1000,
        // reserved 0b0001'0000,
        // reserved 0b0010'0000,
        // reserved 0b0100'0000,
        // reserved 0b1000'0000,
        
        // If you define one of the reserved bits, change the operator<< code below to display it.
        // Convention is uppercase char for set, lowercase for unset.
        // e.g. "[-----S-e]" means Signed but not encrypted
    };
    
    uint16_t msg_len;      // length of payload below
    byte flags;            // see Flag_bits enum defined flag bits
    uint8_t type;
    const byte* payload;   // variable length, specified in msg_len above
    
    Packet() = default;
    Packet(const byte* buff);

    bool isEncrypted() { return flags & (byte) Flag_bits::ENCRYPTED; }
    bool isSigned()    { return flags & (byte) Flag_bits::SIGNED; }
    
    friend std::ostream& operator<<(std::ostream&, Packet&);
} Packet;

Packet::Packet(const byte* buff) {
    // TODO: add sanity checking. e.g. buff != nullptr; type < max_defined_type; reserved flag bits maybe should be 0, etc
    auto p = buff;
    msg_len = (uint16_t) *p;    p += sizeof(msg_len);
    flags =   (byte) *p;        p += sizeof(flags);
    type =    (byte) *p;        p += sizeof(type);
    payload = (byte*) p;
}

std::ostream& operator<<(std::ostream& out, Packet& p) {
    // Length
    out << " Length: 0x" << std::hex << std::setw(4) << std::setfill('0') << std::uppercase << p.msg_len << endl;
    // Flags
    out << "  Flags:   0x" << std::hex << std::setw(2) << +p.flags;
    out << "\t[" << "-----" << (p.isSigned() ? "S" : "s") << "-" << (p.isEncrypted() ? "E" : "e")  << "]";
    out << endl;
    // Type
    // TODO: output a readable string for the type value.. e.g. 1 = "ACK",  etc.
    out << "   Type:   " << std::dec << std::setw(4) << std::setfill(' ') << +p.type << endl;   // + to convert to int for output (since uint8_t is typedef unsigned char)
    
    // Payload
    // TODO: wrap lines if payload is too long
    out << "Payload: " << (p.msg_len > 0 ? "0x" : "<None>");
    for (auto i=0; i<p.msg_len; i++)
        out << std::hex << std::setw(2) << std::setfill('0') << std::uppercase << +p.payload[i] ;
    out << endl;
    
    return out;
}
// TODO: define an alternate, compact (1 line?) packet dump format for logging


int main(int argc, char** argv) {

    byte buff[14] {
        0x0A, 0x00,
        0x05,
        0x02,
        0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x80
    };
    
    auto p = Packet(buff);
    cout << p << endl;
    
    /* output =
     
     Length: 0x000A
      Flags:   0x05    [-----S-E]
       Type:      2
    Payload: 0x71727374757677787980

     */
}
