# VLA

![License](https://img.shields.io/badge/license-PolyForm%20Noncommercial%201.0.0-blue)
[![Docs](https://img.shields.io/badge/docs-gh--pages-blue)](https://sd.vatnar.dev/libraries/vla/VLA) 

# Quickstart
### Using CMake and git submodules (recommended)

Clone the repository into where you want to put it, I recommend in a vendor folder or similar
```
git clone https://github.com/Vatnar/VLA
```
In your main `CMakeLists.txt`:
```cmake
add_subdirectory(vendor/VLA)

target_link_libraries(your_target PUBLIC
	VLA
)
```
You should now be able to include for example
```cpp
#include "VLA/Matrix.hpp"
#include "VLA/Vector.hpp"
```
And use the VLA library.

### Using a distributed release
Currently VLA is only available as source code, but in the future prebuilt binaries will be released for major platforms.
