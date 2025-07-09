# vzip

This project implements Huffman coding in C++ for compressing and decompressing text files.

## 📦 Features

- Compress any text file using Huffman coding
- Decompress previously compressed `.vzip` files
- CLI interface for easy use
- Includes install and uninstall via `make`

## 🔧 Installation

```bash
mkdir build
make
sudo make install
```

This installs the CLI tool as `vzip` in `/usr/local/bin`.

## 🗑️ Uninstall

```bash
sudo make uninstall
```

## 🚀 Usage

### Compress a file

```bash
vzip compress <inputfile.txt>
```

This will create a file named `<inputfile>.vzip`.

### Decompress a file

```bash
vzip decompress <inputfile.vzip>
```

This will create a file named `<inputfile>.txt`.

## 📁 File Structure

- `src/` — source code
- `include/` — header files
- `build/` — output binary directory

## 🛠 Requirements

- C++11 compatible compiler
- Linux/macOS with `make`

## 🧹 Clean Build

```bash
make clear
```

## 📝 License

MIT License
