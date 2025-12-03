#!/usr/bin/env bash
set -euo pipefail

# === CONFIG ===

# Create bin dir
BIN_DIR="bin"
mkdir -p "${BIN_DIR}"
cd "${BIN_DIR}"

SRC_DIR="../source"
MAYORANA="${SRC_DIR}/MayoranaFramework/source"

CFLAGS="-std=c17 -g -O0"     # debug-friendly; change -O0 to -O2 for release
# If you want position-independent code for dynamic libs:
PIC_FLAGS="-fPIC"

LDFLAGS='-L. -lSDL2 -lSDL2main -arch arm64
  -framework Cocoa
  -framework CoreAudio
  -framework AudioToolbox
  -framework AudioUnit
  -framework CoreFoundation
  -framework CoreServices
  -framework IOKit
  -framework CoreVideo
  -framework CoreGraphics
  -framework GameController
  -framework Metal
  -framework CoreHaptics
  -framework Carbon
  -framework ForceFeedback'

EXPORTED_SYMBOLS=(AppInit AppEnd AppRender AppUpdate AppInput)

# Sources (adjust the filenames if different)
APP_SRC="${SRC_DIR}/rendering_app.c"
MAC_APP_SRC="${SRC_DIR}/win32_rendering_app.c"  # you may want to rename this source to mac-specific

# Output names
APP_DYLIB="app.dylib"
MAC_APP="apple_engine"

# === END CONFIG ===

echo "Build start: $(date)"

echo "CFLAGS: ${CFLAGS}"
echo "LDFLAGS: ${LDFLAGS}"

# Build dynamic library (equivalent to /LD on Windows)
# macOS exports: use -Wl,-exported_symbol,_Name
EXPORT_FLAGS=""
for sym in "${EXPORTED_SYMBOLS[@]}"; do
  EXPORT_FLAGS+=" -Wl,-exported_symbol,_${sym}"
done

echo "Compiling shared library -> ${APP_DYLIB}"
echo "current dir" $(pwd)
echo "mayorana" ${MAYORANA}
clang ${CFLAGS} -I "${MAYORANA}" -I "${SRC_DIR}" -c "${APP_SRC}" -o rendering_app.o

clang -dynamiclib rendering_app.o -o "${APP_DYLIB}" ${EXPORT_FLAGS} -undefined dynamic_lookup
echo "Created ${APP_DYLIB}"

# Build console app (equivalent to win32_app). On mac, we produce a console executable named mac_app.
echo "Compiling console app -> ${MAC_APP}"

clang ${CFLAGS} "${MAC_APP_SRC}" -I "${MAYORANA}" -I "${SRC_DIR}" -o "${MAC_APP}" ${LDFLAGS}

echo "Created ${MAC_APP}"

# Clean intermediate objects
rm -f "${BIN_DIR}/rendering_app.o"

echo "Build finished: $(date)"
echo "Outputs:"
ls -l