# CLI Assistant Search Query

A command-line interface (CLI) tool that combines YouTube search, Google search, and basic calculations in one application.

## Features

- **YouTube Search**: Search for videos and open them directly in your browser
- **Google Search**: Perform web searches and open results in your browser
- **Basic Calculator**: Perform arithmetic calculations with proper operator precedence

## Prerequisites

- C++ compiler (g++ or clang++)
- libcurl development library
- nlohmann/json library

### Installing Dependencies

#### macOS
```bash
brew install curl
```

#### Ubuntu/Debian
```bash
sudo apt-get install libcurl4-openssl-dev
```

## Building the Project

1. Clone the repository:
```bash
git clone https://github.com/yourusername/CLI-Assistant-Search-Query.git
cd CLI-Assistant-Search-Query
```

2. Compile the program:
```bash
g++ -o cli-assistant code.cpp -lcurl

g++ -std=c++17 -I/opt/homebrew/include SQ.cpp -o MacEXE -L/opt/homebrew/lib -lcurl
```

## Usage

The program supports three main operations:

### YouTube Search
```bash
./cli-assistant -yt "search query"
```
This will:
1. Search YouTube for the query
2. Display the top 5 results
3. Let you choose a video to open in your browser

### Google Search
```bash
./cli-assistant -gg "search query"
```
This will open the Google search results for your query in your default browser.

### Calculator
```bash
./cli-assistant -calc "expression"
```
Supports basic arithmetic operations with proper operator precedence:
- Addition (+)
- Subtraction (-)
- Multiplication (*)
- Division (/)

Example:
```bash
./cli-assistant -calc "3-3+2-6-7+90/3"  # Result: 19
```

## API Key

The program requires a YouTube API key to function. You need to:
1. Get a YouTube Data API key from the [Google Cloud Console](https://console.cloud.google.com/)
2. Replace the placeholder in the code with your API key

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.