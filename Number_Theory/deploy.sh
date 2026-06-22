#!/bin/bash

# Number Theory Explorer - Deployment Script
# This script builds and packages the application for distribution

set -e  # Exit on any error

echo "🚀 Number Theory Explorer - Deployment Script"
echo "=============================================="

# Configuration
PROJECT_NAME="NumberTheoryGUI"
VERSION="1.0.0"
BUILD_TYPE="Release"
QT_VERSION="6"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Function to print colored output
print_status() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARN]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

print_header() {
    echo -e "${BLUE}==== $1 ====${NC}"
}

# Check prerequisites
check_prerequisites() {
    print_header "Checking Prerequisites"

    # Check for Qt6
    if ! command -v qmake6 &> /dev/null && ! command -v qmake &> /dev/null; then
        print_error "Qt6 development tools not found. Please install Qt6."
        exit 1
    fi
    print_status "Qt6 found"

    # Check for CMake
    if ! command -v cmake &> /dev/null; then
        print_error "CMake not found. Please install CMake 3.16 or higher."
        exit 1
    fi
    print_status "CMake found"

    # Check for C++ compiler
    if ! command -v g++ &> /dev/null && ! command -v clang++ &> /dev/null; then
        print_error "C++ compiler not found. Please install GCC or Clang."
        exit 1
    fi
    print_status "C++ compiler found"
}

# Clean previous build
clean_build() {
    print_header "Cleaning Previous Build"
    if [ -d "build" ]; then
        rm -rf build
        print_status "Removed old build directory"
    fi
}

# Create build directory and configure
configure_build() {
    print_header "Configuring Build"
    mkdir -p build
    cd build

    # Configure with CMake
    if command -v qmake6 &> /dev/null; then
        QMAKE_CMD="qmake6"
    else
        QMAKE_CMD="qmake"
    fi

    cmake .. \
        -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
        -DCMAKE_PREFIX_PATH="/opt/Qt/${QT_VERSION}/gcc_64" \
        -DCMAKE_INSTALL_PREFIX="/usr/local" \
        -DQT_QMAKE_EXECUTABLE=$(which ${QMAKE_CMD})

    print_status "CMake configuration completed"
    cd ..
}

# Build the application
build_application() {
    print_header "Building Application"
    cd build

    # Build with all available cores
    make -j$(nproc)
    print_status "Application built successfully"

    cd ..
}

# Run tests
run_tests() {
    print_header "Running Tests"
    cd build

    if [ -f "${PROJECT_NAME}Test" ]; then
        ./${PROJECT_NAME}Test
        print_status "Tests completed successfully"
    else
        print_warning "Test executable not found, skipping tests"
    fi

    cd ..
}

# Create distribution package
create_package() {
    print_header "Creating Distribution Package"

    # Create package directory
    PACKAGE_DIR="${PROJECT_NAME}-${VERSION}"
    rm -rf ${PACKAGE_DIR}
    mkdir -p ${PACKAGE_DIR}

    # Copy executable
    cp build/${PROJECT_NAME} ${PACKAGE_DIR}/

    # Copy resources
    cp -r resources ${PACKAGE_DIR}/

    # Create desktop file for Linux
    cat > ${PACKAGE_DIR}/${PROJECT_NAME}.desktop << EOF
[Desktop Entry]
Version=${VERSION}
Name=Number Theory Explorer
Comment=Educational number theory application
Exec=${PROJECT_NAME}
Icon=${PROJECT_NAME}
Terminal=false
Type=Application
Categories=Education;Science;Math;
EOF

    # Create run script
    cat > ${PACKAGE_DIR}/run.sh << EOF
#!/bin/bash
cd "\$(dirname "\$0")"
export LD_LIBRARY_PATH="\$PWD/lib:\$LD_LIBRARY_PATH"
./${PROJECT_NAME}
EOF
    chmod +x ${PACKAGE_DIR}/run.sh

    # Create tarball
    tar -czf ${PACKAGE_DIR}.tar.gz ${PACKAGE_DIR}
    print_status "Distribution package created: ${PACKAGE_DIR}.tar.gz"

    # Cleanup
    rm -rf ${PACKAGE_DIR}
}

# Create AppImage (Linux only)
create_appimage() {
    if [[ "$OSTYPE" == "linux-gnu"* ]]; then
        print_header "Creating AppImage"

        if command -v appimagetool &> /dev/null; then
            # Create AppDir structure
            APPDIR="AppDir"
            rm -rf ${APPDIR}
            mkdir -p ${APPDIR}/usr/bin
            mkdir -p ${APPDIR}/usr/lib
            mkdir -p ${APPDIR}/usr/share/applications
            mkdir -p ${APPDIR}/usr/share/icons/hicolor/256x256/apps

            # Copy files
            cp build/${PROJECT_NAME} ${APPDIR}/usr/bin/
            cp -r resources ${APPDIR}/usr/bin/
            cp ${PROJECT_NAME}.desktop ${APPDIR}/usr/share/applications/
            # cp icon.png ${APPDIR}/usr/share/icons/hicolor/256x256/apps/${PROJECT_NAME}.png

            # Create AppRun script
            cat > ${APPDIR}/AppRun << EOF
#!/bin/bash
cd "\$(dirname "\$0")"
export LD_LIBRARY_PATH="\$PWD/usr/lib:\$LD_LIBRARY_PATH"
exec "\$PWD/usr/bin/${PROJECT_NAME}" "\$@"
EOF
            chmod +x ${APPDIR}/AppRun

            # Create AppImage
            appimagetool ${APPDIR} ${PROJECT_NAME}-${VERSION}.AppImage
            print_status "AppImage created: ${PROJECT_NAME}-${VERSION}.AppImage"

            # Cleanup
            rm -rf ${APPDIR}
        else
            print_warning "appimagetool not found, skipping AppImage creation"
        fi
    fi
}

# Generate documentation
generate_docs() {
    print_header "Generating Documentation"

    # Create README for distribution
    cat > README.md << EOF
# Number Theory Explorer

A comprehensive educational GUI application for exploring number theory algorithms.

## Features

- **Interactive Algorithm Exploration**: 11+ number theory algorithms with real-time computation
- **Visual Learning Tools**: Animated visualizations for prime sieves and factorization trees
- **Educational Content**: Step-by-step tutorials and mathematical explanations
- **Performance Analysis**: Benchmarking tools for algorithm comparison
- **Accessibility**: Full keyboard navigation and screen reader support
- **Multiple Themes**: Light, dark, and high-contrast themes

## System Requirements

- **Operating System**: Linux, Windows, or macOS
- **Qt Version**: Qt6.2 or higher
- **Memory**: 512MB RAM minimum, 2GB recommended
- **Storage**: 50MB free space

## Installation

### Linux
\`\`\`bash
# Extract the archive
tar -xzf NumberTheoryGUI-${VERSION}.tar.gz
cd NumberTheoryGUI-${VERSION}

# Run the application
./run.sh
\`\`\`

### Windows
Run the installer executable and follow the setup wizard.

### macOS
Open the .dmg file and drag the application to your Applications folder.

## Usage

1. Launch the Number Theory Explorer
2. Select an algorithm category from the tabs
3. Enter your parameters in the input fields
4. Click the algorithm button to compute results
5. View detailed results with mathematical formatting
6. Use the visualization tools to understand algorithms visually

## Algorithm Categories

- **Basic Operations**: GCD, Extended GCD, modular arithmetic
- **Prime Theory**: Primality testing, sieve algorithms
- **Advanced Functions**: Euler's totient, matrix exponentiation
- **Cryptography**: Modular inverse, fast exponentiation

## Educational Features

- **Interactive Tutorials**: Step-by-step learning guides
- **Algorithm Explanations**: Detailed mathematical descriptions
- **Visual Demonstrations**: Animated algorithm visualizations
- **Quiz System**: Test your understanding

## Support

For issues, questions, or contributions:
- GitHub Repository: [Repository URL]
- Documentation: Built-in help system
- Community Forums: [Forum URL]

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments

Built with Qt6 - a powerful C++ GUI framework.
Special thanks to the number theory community for algorithm insights.
EOF

    print_status "Documentation generated"
}

# Main deployment process
main() {
    print_header "Starting Deployment Process"

    check_prerequisites
    clean_build
    configure_build
    build_application
    run_tests
    create_package
    create_appimage
    generate_docs

    print_header "Deployment Completed Successfully!"
    echo ""
    echo "Generated files:"
    echo "  - ${PROJECT_NAME}-${VERSION}.tar.gz (Linux)"
    echo "  - ${PROJECT_NAME}-${VERSION}.AppImage (Linux AppImage)"
    echo "  - README.md (Documentation)"
    echo ""
    echo "To run the application:"
    echo "  cd ${PROJECT_NAME}-${VERSION}"
    echo "  ./run.sh"
}

# Run main function
main "$@"
