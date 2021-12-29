# Asynchronous message handling
This project aims to prove the power of the modern C++ (17) capabilities in
writing high-performance networking code to be used later in 5G gNB networking
services.

## Dependencies
1. cmake;
2. boost.asio.

## Solution architecture
![solution design](./doc/design.png)

### Synthetic net protocol layout
![message layout](./doc/message.png)

Note: byte order is the big endian.

Message types:
- Control Message: 0x1001,
- Control Message Acknowledgement: 0x1002,
- Control Message Negative Acknowledgement: 0x1003,
- Control Message Acknowledgement with Checksum: 0x1004,
- Data Message: 0x8001.

# Resulting benchmarks
TBD
