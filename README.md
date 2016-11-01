##*Introduction to mesh skinning*

-----
**CMake Build instructions**

-----
  - Out-of-source build :
 ```
git clone git@github.com:Modelisation5ETI/TP5_Skinning.git && \
mkdir TP5_Skinning-build && cd TP5_Skinning-build  && \
cmake ../TP5_Skinning  && \
make -j
 
 ```
  - QtCreator :
 ```
git clone git@github.com:Modelisation5ETI/TP5_Skinning.git  && \
mkdir TP5_Skinning-build && cd TP5_Skinning-build  && \
qtcreator ../TP5_Skinning
 
 ```
  *Then configure project using QtCreator API* : 
   - *Set the build directory to TP5_Skinning-build/*
   - *Run CMake and build project*


-----
**Implementation**

-----

 <img src="./Screenshots/SkinnedCylinder.png" alt="SkinnedCylinder" width="200" height="200" />

 ```
 ./Skinning
 ```
Skinning cylinder using weights
