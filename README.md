# Image Processing

A terminal-based image editor written in C++ supporting a wide range of filters and image manipulations.

## Features

- 15 filters: Grayscale, Black & White, Invert, Merge, Flip, Rotate, Lighten/Darken, Crop, Frame, Detect Edges, Resize, Blur, Sunlight, Oil Painting, Old TV
- Undo/Redo support per editing session
- Input validation with helpful error messages
- Progress bar for heavy filters
- Supports JPG, JPEG, PNG, BMP, TGA formats

## Project Structure
```
image-processing/
├── include/          # stb_image headers
├── src/
│   ├── core/         # Image and Photo classes
│   ├── filters/      # Filter implementations and dispatcher
│   ├── ui/           # Menu, constants, and UI logic
│   └── utils/        # Input validation and utilities
├── samples/          # Sample input images
├── tests/            # Test script
├── CMakeLists.txt
└── main.cpp
```

## Build

### Requirements

- CMake 3.16+
- GCC with C++17 support

### Steps
```bash
cmake -B build
cmake --build build
```

## Usage
```bash
./build/image-processing
```

The application presents a menu-driven interface:

1. Select **Apply Filter** from the main menu
2. Choose a filter from the list
3. Enter the input image path
4. Enter any filter-specific parameters when prompted
5. Enter the output image path
6. Select **Back to Main Menu** to apply another filter or **Quit** to exit

## Testing

Runs all 15 filters against every image in a given directory and reports pass/fail for each output.

**Requirements:**
```bash
sudo apt install imagemagick
```

**Run:**
```bash
chmod +x test_filters.sh
./test_filters.sh  [binary_path]
```

| Argument | Description |
|----------|-------------|
| `samples_dir` | Directory containing input images |
| `binary_path` | Optional, defaults to `./build/image-processing` |

**Example:**
```bash
./test_filters.sh samples/
./test_filters.sh samples/ ./build/image-processing
```

Results are saved to `test_results.log` and filter outputs to `test_outputs/`.

## Output Samples

Sample filter outputs are available in the [`output_samples/`](output_samples/) directory.

## License

This project is licensed under the [GNU General Public License v3.0](LICENSE).

