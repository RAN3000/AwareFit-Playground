# Graph Based Image Segmentation

Implementation of the paper: _Efficient Graph-Based Image Segmentation_ by Felzenszwalb and Huttenlocher.
The paper addresses the problem of image segmentation, the proposed algorithm is very fast. I found the paper a pleasure to read, I decided to try and implement the algorithm to improve my understanding of the paper's concepts.

Here's an example of the program:

![Original Image](demoPics/graph-based-segmentation-1-original.png?raw=true "Original Image")
![Segmented Image](demoPics/graph-based-segmentation-1.png?raw=true "Segmented Image")

I recommend reading the paper, and secondarily the commends on the code.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine.

### Prerequisites

You need to have OpenCV installed on your system, this project was developed with OpenCV 3.4.1.

OpenCV setup instruction on:

- [Linux](https://docs.opencv.org/3.4.1/d7/d9f/tutorial_linux_install.html)
- [Mac](https://www.learnopencv.com/install-opencv3-on-macos/)
- [Windows](https://docs.opencv.org/3.4.1/d3/d52/tutorial_windows_install.html)

### Installing

First clone or download this project in a folder, let's call it `<dir>`.
To compile the program, execute this commands:

```
cd <dir>
mkdir build
cd build
cmake ..
make
```

To run the program:

```
cd <dir>/build
./Segmentation <image of your choice>
```

## Built With

- [OpenCV](https://opencv.org/about.html) - open source computer vision and machine learning software library
- [cvui](https://dovyski.github.io/cvui/) - simple UI lib built on top of OpenCV drawing primitives

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details.

## Acknowledgments

- Paper reference: P. F. Felzenszwalb and D. P. Huttenlocher. Efficient Graph-Based Image Segmentation. IJCV, 59:167–181, 2004.
- Tutorial on Kruskal's minimum spanning tree of a graph algorithm: [GeeksforGeeks](https://www.geeksforgeeks.org/kruskals-minimum-spanning-tree-using-stl-in-c/).
- Tutorial on disjoint-set forest with union by rank and path compression: [GeeksforGeeks](https://www.geeksforgeeks.org/union-find-algorithm-set-2-union-by-rank/).
