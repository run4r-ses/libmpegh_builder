# libmpegh builder

[![Build](https://github.com/run4r-ses/libmpegh_builder/actions/workflows/build.yml/badge.svg)](https://github.com/run4r-ses/libmpegh_builder/actions/workflows/build.yml)

A multi-platform GitHub Actions builder for Ittiam's MPEG-H 3D decoder.

## Downloads
You can download the latest pre-compiled builds [here](https://run4r-ses.github.io/libmpegh_builder/).

## Supported platforms
- Windows *(32-bit builds are not available)*
- Linux
- macOS
- Android
- WebAssembly

## WebAssembly usage
For instructions on using the customized WASM build, see [WASM.md](WASM.md).

## License
This project is licensed under the MIT License.

libmpegh is licensed under BSD-3-Clause-Clear License,
and a copy is included with all builds.

The patch files located in the `patches/` directory contain source code fragments from Ittiam.
As derivative works, these files are subject to the same BSD-3-Clause-Clear License.

For more information, see [ittiam/LICENSE](ittiam/LICENSE), [ittiam/LICENSE2](ittiam/LICENSE2), and [ittiam/README.ittiam](ittiam/README.ittiam.md).
