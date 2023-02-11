# parsepacket

A C++ class defining (and printing) a simple packet with a variable length payload. 

I had this question in a job interview yesterday and I didn't do it as well as I could have.
So just for my own peace of mind, here's a better version.

This code

```cpp
  byte buff[14] {
    0x0A, 0x00,        // payload length (little-endian)
    0x04,              // flags:  ENCRYPTED=bit 1, SIGNED=bit 3, others reserved
    0x02,              // packet type:  just a uint8
    0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x80    // payload. variable length.
  };
    
  auto p = Packet(buff);
  cout << p << endl;   
```

outputs this:
```text
 Length: 0x000A
  Flags:   0x04 [-----S-e]
   Type:      2
Payload: 0x71727374757677787980
```
