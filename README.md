# Asynchronous message handling
This project aims to prove the power of the modern C++ (17) capabilities in
writing high-performance networking code to be used later in 5G gNB networking
services.

## Dependencies
1. cmake;
2. boost.asio.

## Solution architecture

                    ┌─────────────────────────────────────────────┐
                    │                                             │
┌──────────┐        │                ┌────────────────┐           │
│Traffic   │        │                │          perf  │           │
│record    ├──────┐ │             ┌──┴───────────┐    │           │
│database 1│      │ │     TCP     │              │    │           │
└──────────┘      │ │   ┌─────────* C++11 based  ├────┘           │
                  ▼ │   │         │  async app   │            ┌───▼────┐
                 ┌──┴───┴──────┐  └──────────────┘            │        │
┌──────────┐     │             │                              │ report │
│Traffic   │     │ TCP Traffic │                              │        │
│record    ├─────►  Generator  │     ┌────────────────┐       └───▲────┘
│database 2│     │             │     │          perf  │           │
└──────────┘     └──┬───┬──────┘  ┌──┴───────────┐    │           │
                  ▲ │   │         │              │    │           │
                  │ │   └─────────* c++17 based  ├────┘           │
┌──────────┐      │ │     TCP     │  async app   │                │
│Traffic   ├──────┘ │             └──────────────┘                │
│record    │        │                                             │
│database 3│        └─────────────────────────────────────────────┘
└──────────┘


### Syntetic net protocol layout

 ┌───────────┬─────────────┬──────────────────────┐
 │  Message  │ Payload msg.│ Rundomized payload   │
 │   Type    │   length    │       data           │
 │  2 bytes  │  4 bytes    │                      │
 └───────────┴─────────────┴──────────────────────┘
Note: byte order is the big endian.


# Resulting benchmarks
TBD
