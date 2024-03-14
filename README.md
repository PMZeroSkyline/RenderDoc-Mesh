# RenderDoc-Mesh
Extraction of model(.obj) from RenderDoc .csv

example
```
rmesh POSITION.x POSITION.y POSITION.z in_file.csv out_file.obj
```
```
rmesh POSITION.x POSITION.y POSITION.z UV.x UV.y in_file.csv out_file.obj
```
```
rmesh POSITION.x POSITION.y POSITION.z NORMAL.x NORMAL.y NORMAL.z in_file.csv out_file.obj out_file.obj
```

compile
```
// Compiled with c++17

// MSVC
cl /EHsc /std:c++17 rmesh.cpp
// Clang++
clang++ -std=c++17 rmesh.cpp -o rmesh
```