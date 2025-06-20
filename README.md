# CRT-Based OTP Verification System

This repository contains a cryptographic One-Time Password (OTP) verification system implemented in C. The system leverages number-theoretic techniques such as the Chinese Remainder Theorem (CRT) and modular inverse to enable secure and tamper-resistant OTP generation and validation.

The project was developed as part of a Discrete Mathematics coursework at Institut Teknologi Bandung (ITB) under the guidance of Dr. Rinaldi Munir.

---

## Folder Structure
```plaintext
otp-verification-crt/
├── src/ # Source code (.c) implementing the OTP system
│ ├── main.c # Main program for OTP generation & verification
│ └── benchmark.c # To measure performance
├── data/ # Sample input files for test cases
│ └── example_input.txt
├── doc/ # Final paper/documentation
│ └── 13524071-Kalyca-Nathania-Benedicta-Manullang-Makalah-Matematika-Diskrit.pdf
└── README.md # This file
```

---

## Features

- CRT-based OTP reconstruction using user-defined moduli.
- Secure hashing of timestamp and user ID using 64-bit operations.
- Rate limiting after 3 failed verification attempts.
- Input validation for modulus range and coprimality.
- File-based tracking of attempts (`ratelimit.log`).

---

## How to Compile and Run

### Prerequisites
- A C compiler (e.g. GCC)
- Unix-like OS (for `unistd.h`)
- Terminal environment

### Compile
```bash
cd src
gcc main.c -o otp_verifier
```
### Run
```bash
./otp_verifier
```
You will be prompted to enter:
1. Current timestamp (integer)
2. User ID (integer)
3. Number of moduli (n)
4. n pairwise coprime integers as moduli

## Sample Input
Example test cases are available in data/example_input.txt.
You can feed these values manually when running the program.
Example:
```bash
123456789 42 3 5 7 11
```
## Benchmark
To measure performance of the OTP generation (e.g., secure_hash()):

### Compile Benchmark
```bash
gcc benchmark.c -o benchmark
```
### Run Benchmark
```bash
./benchmark
```
This will measure average execution time over 100,000 iterations.
You can use this data to compare performance with industry-standard OTP methods.

## Test Scenarios
The system has been tested across a variety of scenarios including:
- Small moduli with insufficient keyspace
- Coprime moduli vs. non-coprime detection
- Rate limiting after multiple failures
- Large timestamp and user ID combinations
- Validity drift between similar timestamps

Full documentation is available in the doc/ folder.

## Paper
Read the full academic paper describing the theory, implementation, and results:
📎 doc/13524071-Kalyca-Nathania-Benedicta-Manullang-Makalah-Matematika-Diskrit.pdf

## Acknowledgement
This project was developed as part of the IF1220 Discrete Mathematics course at ITB, under the supervision of Dr. Rinaldi Munir. 
Special thanks to everyone who supported this work.




