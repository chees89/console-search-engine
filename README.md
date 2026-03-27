# Search Engine

Command-line search engine for text files.

## Features
- Document indexing
- Keyword search
- Result ranking
- JSON support

## Technologies
- C++
- CMake
- nlohmann/json
- Google Test

## Requirements
- gcc / g++
- CMake (>= 3.17)

## Input files

Before running the program, configure:

- `config.json` — paths to text files and settings
- `requests.json` — search queries

## Build (Linux)

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

## Build (Windows)

Open the project in your IDE (CLion / Visual Studio) and build it with CMake.

## Run

./search_engine

## Tests

ctest
