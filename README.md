superparsing
============
Scalable Nonparametric Image Parsing with superpixels based on a semplification of the algorithm proposed in the paper 'Superparsing: Scalable Nonparametric Image Parsing with Superpixels' by Joseph Tighe and Svetlana Lazebnick.

The project is developed in C++ using QtCreator

Requirements
------------
The libraries required by this project are:
- opencv
- matio
- libDAI
- jsoncpp

3RD-PARTY
---------
The files in the directory `OpenCVGraphSegmentation` are based on the project `OpenCVGraphSegmentation` released under the GPL v2

It also require the library GC_Optimization which you can take from here `http://vision.csd.uwo.ca/code/` (gco-v3.0.zip) and put the cpp and h files in a subfolder named GCO.

Usage
-----
The program need the folder containing the Sift Flow Dataset to be located in the parent folder of the project folder.
