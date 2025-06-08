
# How to Compile and Run

This project uses **libcurl** and **nlohmann-json** libraries. Follow the instructions below for your operating system.

---

## macOS

1. **Install dependencies via Homebrew:**

```bash
brew install curl
brew install nlohmann-json
```

2. **Compile your program:**

For Apple Silicon Macs (M1/M2) where Homebrew installs to `/opt/homebrew`:

```bash
g++ -std=c++17 -I/opt/homebrew/include code.cpp -o compiledCode -L/opt/homebrew/lib -lcurl
```

For Intel Macs where Homebrew is installed under `/usr/local`:

```bash
g++ -std=c++17 -I/usr/local/include code.cpp -o compiledCode -L/usr/local/lib -lcurl
```

Or, a portable way (detects Homebrew prefix):

```bash
g++ -std=c++17 -I$(brew --prefix)/include code.cpp -o compiledCode -L$(brew --prefix)/lib -lcurl
```

3. **Run:**

```bash
./compiledCode
```

---

## Linux (Ubuntu/Debian)

1. **Install dependencies:**

```bash
sudo apt update
sudo apt install build-essential libcurl4-openssl-dev nlohmann-json3-dev
```

2. **Compile:**

```bash
g++ -std=c++17 code.cpp -o compiledCode -lcurl
```

3. **Run:**

```bash
./compiledCode
```

---

## Windows (Using MSYS2 or MinGW)

1. **Install MSYS2** (https://www.msys2.org/) or MinGW.

2. **Open MSYS2 shell and update packages:**

```bash
pacman -Syu
```

3. **Install dependencies:**

```bash
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-curl mingw-w64-x86_64-nlohmann-json
```

4. **Compile:**

```bash
g++ -std=c++17 code.cpp -o compiledCode.exe -lcurl
```

5. **Run:**

```bash
./compiledCode.exe
```

---

## Notes

- Make sure `g++` is installed and accessible via your terminal.
- If you get errors about missing headers (`curl/curl.h` or `nlohmann/json.hpp`), check that the libraries are correctly installed and the include paths are correct.
- On macOS, Homebrew may install packages in different directories depending on your CPU architecture.