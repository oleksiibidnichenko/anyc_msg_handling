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

Message size limits:
- Control Message: 64 bytes,
- Control Message Acknowledgement: 16 bytes,
- Control Message Negative Acknowledgement: 12 bytes,
- Control Message Acknowledgement with Checksum: 16 bytes,
- Data Message: 64 kilobytes.


### Traffic generator
A tool that shoots with a ton of messages to test target applications.

Example:
```
./traffic_generator -m 1001:64:25 -m 1002:16:25 -m 8001:1500:50 -t10

```
t - time in seconds (default value is 10);
m - message type in the following format (MSG_ID:MSG_SIZE_BYTES:FRACTION).

# Resulting benchmarks
The measurement is done for three implementations: with a message handler
written on CPP17, CPP14, and with an empty handler (dummy).
The average result calculated in the following way:
1. Gather 5 samples for each version;
2. Exclude min and max extremums;
3. Take the average value of the remaining 3 samples.

Note: all measurements were done in the Release mode.

## CPU
11th Gen Intel(R) Core(TM) i5-1135G7 @ 2.40GHz   1.38 GHz

## Target PC
VirtualBox Ubuntu 20.04.3 LTS

## Result 1 (max package size 1200 bytes)
Traffic_generator command options
```
-m 1001:64:25 -m 1002:16:25 -m 8001:1200:50 -t10
```
Result (an average value of five samples in nanoseconds)
|CPP11|CPP17|Dummy|
|:----:|:----:|:----:|
|2014|1791|1711|

Total performance raises formula: 1 - (cpp17-dummy)/(cpp11-dummy).
Total performance raises: 74%.

## Result 2 (max package size 8000 bytes)
Traffic_generator command options
```
-m 1001:64:25 -m 1002:16:25 -m 8001:8000:50 -t10

```
Result (an average value of five samples in nanoseconds)
|CPP11|CPP17|Dummy|
|:----:|:----:|:----:|
|3408|2586|2322|

Total performance raises formula: 1 - (cpp17-dummy)/(cpp11-dummy).
Total performance raises: 76%.
