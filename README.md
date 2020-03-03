# Infomages Design Tool

This design tool allows users to create eye-catching visualizations that embed basic charts into an image. We call these visualizations Infomages.

For questions please contact dcoelho_at_stonybrook.edu (replace _at_ with @). 


## Dependencies

The code is written in C++ and this repository contains a Visual Studio 2017 solution with the code. 

The code requires two libraries to run 

* [OpenCV Version 3.1](https://opencv.org/opencv-3-1/)

* [Qt Version 5.12.4](https://www.qt.io/download)

Additionally, the tool makes use of the Google Custom Search API to retrieve images. To use this feature you will need an API key. You can get a key through your [Google Developer account](https://console.developers.google.com/apis/dashboard)


## Data Files Supported

MemeViz accepts data in the form of .csv files. 

The data must be a table containing one or two columns. If a one column table is used and the data is numeric then we use the indices as lablels but if the data is categorical we use the counts of the categories as the numerical data. For two column tables we expect that one columne contains the labels and the second the numerical values.

## Example Results

![alt text](https://raw.githubusercontent.com/darius-coelho/Infomages-Design-Tool/master/Infomage%20Examples/pie1.png)

![alt text](https://raw.githubusercontent.com/darius-coelho/Infomages-Design-Tool/master/Infomage%20Examples/pie2.png)

![alt text](https://raw.githubusercontent.com/darius-coelho/Infomages-Design-Tool/master/Infomage%20Examples/bar1.png)

![alt text](https://raw.githubusercontent.com/darius-coelho/Infomages-Design-Tool/master/Infomage%20Examples/bar2.png)
