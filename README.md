# Thermo_ocl
A 3D FDM thermal calculation program powered by OpenCL

## Requirements
2 header files are required.
- [fast-cpp-csv-parser](https://github.com/ben-strasser/fast-cpp-csv-parser)
- [LeksysINI](https://github.com/Lek-sys/LeksysINI)

## Compilation
GCC (mingw-w64) is preferred.
```
OpenCL(Intel SDK)：g++.exe -I"C:/intel/OpenCL/sdk/include" -"C:/intel/OpenCL/sdk/lib/x64" -std=c++11  .\[.cppファイル] -v -lOpenCL -static -O2
OpenCL(CUDA Toolkit)：g++.exe -I"%CUDA_PATH%/include/" -L"%CUDA_PATH%/lib/x64" -std=c++11 .\[.cppファイル] -v -lOpenCL -static -O2
```

## Usage
There is no document for it, but you can read the codes easily because they are procedural and very simple.
