#!/bin/bash

# Exit immediately if a command exits with a non-zero status
set -e

# Define vcpkg directory
VCPKG_DIR="vcpkg"

# Parse command-line arguments
PRESET="with_gui"
GENERATOR=""
COMPILER=""

while [[ $# -gt 0 ]]; do
    case $1 in
        --preset)
            PRESET="$2"
            shift 2
            ;;
        --generator)
            GENERATOR="$2"
            shift 2
            ;;
        --compiler)
            COMPILER="$2"
            shift 2
            ;;
        *)
            echo "Unknown argument: $1"
            exit 1
            ;;
    esac
done

echo "Configuring for $PRESET"

# Clone vcpkg if not already cloned
if [ ! -d "$VCPKG_DIR" ]; then
    echo "Cloning vcpkg..."
    git clone https://github.com/microsoft/vcpkg.git $VCPKG_DIR
else
    echo "vcpkg is already cloned."
fi

# Bootstrap vcpkg
echo "Bootstrapping vcpkg..."
cd $VCPKG_DIR
./bootstrap-vcpkg.sh
cd ..

# Configure CMake command
CMAKE_CMD=("cmake" "-S" "." "-B" "build" "--preset" "$PRESET")

# Add generator if specified
if [ -n "$GENERATOR" ]; then
    CMAKE_CMD+=("-G" "$GENERATOR")
fi

# Add compiler if specified
if [ -n "$COMPILER" ]; then
    CMAKE_CMD+=("-DCMAKE_C_COMPILER=$COMPILER" "-DCMAKE_CXX_COMPILER=$COMPILER")
fi

# Run CMake with specified parameters
echo "Running CMake with the following command: ${CMAKE_CMD[*]}"
"${CMAKE_CMD[@]}"

echo "Configuration completed successfully."

