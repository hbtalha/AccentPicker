
# Accent Picker

Accent Picker is a lightweight tool for quickly inserting accented characters on Linux, inspired by [PowerToys Quick Accent](https://learn.microsoft.com/en-us/windows/powertoys/quick-accent) for Windows. Hold a letter key to display a popup and select the desired accent, mimicking the Windows experience.

## Features

- Popup for accented characters when holding letter keys
- Closely mimics PowerToys Quick Accent behavior
- Fast and unobtrusive
- **X11 support only** (requires Qt X11 Extras)

## Installation

### Requirements
- Linux (X11 display server)
- Qt 6.7+ (with Qt X11 Extras)
- CMake

### Build
Clone the repository and build:

```bash
git clone https://github.com/yourusername/accentpicker.git
cd accentpicker
cmake -B build .
cmake --build build
```

### Install
To install Accent Picker system-wide (optional):

```bash
sudo cmake --install build
```

### Run
Launch the built binary:

```bash
./build/accentpicker
```

## Usage

Activate by holding the key for the character you want to add an accent to, then (while held down) press the activation key (Space key). If you continue to hold, an overlay to choose the accented character will appear.

For example: If you want "à", press and hold A and press Space.

With the dialog enabled, keep pressing your activation key.

## Supported Language Sets
Accent Picker supports accented characters for the following language sets:

- Catalan
- Currency
- Croatian
- Czech
- Danish
- Gaeilge
- Gàidhlig
- Dutch
- Greek
- Estonian
- Finnish
- French
- German
- Hebrew
- Hungarian
- Icelandic
- Italian
- Kurdish
- Lithuanian
- Macedonian
- Māori
- Norwegian
- Pinyin
- Polish
- Portuguese
- Romanian
- Slovak
- Slovenian
- Spanish
- Serbian
- Serbian Cyrillic
- Swedish
- Turkish
- Vietnamese
- Welsh

## Supported Environments

- Linux (X11 only)
- Not compatible with Wayland

## Credits

Accent Picker borrows the **X11PlatformWindow** class from [CopyQ](https://github.com/hluk/CopyQ/tree/master).

## License

See [LICENSE](LICENSE) for details.

## Contributing

Contributions are welcome! Help improve feature parity, usability, or documentation.

- Report bugs
- Suggest features
- Submit pull requests
- Improve documentation
