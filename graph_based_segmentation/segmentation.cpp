#include <stdio.h>
#include <algorithm>
#include <cassert>
#include <random>
#include <vector>
#include <opencv2/opencv.hpp>
#define CVUI_IMPLEMENTATION
#include "cvui.h"

#include "Graph.hpp"

#define WINDOW_NAME_ORIGINAL "Original"
#define WINDOW_NAME "Segmentation"

using namespace cv;

/* SEGMENTATION FUNCTION */
Mat graphBasedSegmentationMono(Mat &input, const double &k, const vector<Vec3b> &random_colors)
{
    assert(k > 0);

    cout << "Segmenting image....." << endl;

    Mat smoothed;
    GaussianBlur(input, smoothed, Size(0, 0), 0.8, 0.8);

    // build the grid graph
    Graph gridGraph(smoothed);

    // 0. Sort all the edges in non-decreasing order of their weight.
    sort(gridGraph.edges.begin(), gridGraph.edges.end());

    // 1. Start a segmentation S0, where each vertex vi is in its own component.
    DisjointSets ds(gridGraph.V);

    // 2. Repeat step 3. for q = 1,...,#E
    vector<pair<int, iPair>>::iterator it;
    for (it = gridGraph.edges.begin(); it != gridGraph.edges.end(); it++)
    {

        // 3. Let Ci(q-1) be the component of S(q-1) containing vi and
        //    Cj(q-1) the component containing vj. If Ci(q-1) != Cj(q-1) and
        //    it->weight <= MInt(Ci(q-1), Cj(q-1)) then S(q) is obtained
        //    from S(q-1) by merging Ci(q-1) and Cj(q-1). Otherwise S(q) = S(q-1).
        int u = it->second.first;
        int v = it->second.second;

        int component_u = ds.find(u);
        int component_v = ds.find(v);

        double threshold_u = k / ds.size[component_u];
        double threshold_v = k / ds.size[component_v];
        // it->first is the edge weight
        bool D = (it->first > (ds.Int[component_u] + threshold_u) || it->first > (ds.Int[component_v] + threshold_v));

        if (component_u != component_v && !D)
        {
            // merge the two sets components
            ds.merge(component_u, component_v);
        }
    }

    // 4. Algorithm stops with segmentation S = S(#E), generate output image
    int nRows = smoothed.rows;
    int nCols = smoothed.cols;
    Mat output(nRows, nCols, CV_8UC3);
    for (int y = 0; y < nRows; ++y)
    {
        for (int x = 0; x < nCols; ++x)
        {
            int component = ds.find(x + y * nCols);
            output.at<Vec3b>(y, x)[0] = random_colors.at(component)[0];
            output.at<Vec3b>(y, x)[1] = random_colors.at(component)[1];
            output.at<Vec3b>(y, x)[2] = random_colors.at(component)[2];
        }
    }

    return output;
}

int main(int argc, char **argv)
{

    // cout << "Test passed: " << std::boolalpha << testMST() << endl;

    if (argc != 2)
    {
        printf("usage: segmentation <Image_Path>");
        return -1;
    }

    Mat colorImage, image, imageCopy;
    colorImage = imread(argv[1], 1);

    colorImage.convertTo(colorImage, CV_8U);
    cvtColor(colorImage, image, CV_BGR2GRAY);
    int resizedCols = 240;
    int resizedRows = image.rows / (float)image.cols * resizedCols;
    resize(image, imageCopy, Size(resizedCols, resizedRows));

    if (!image.data)
    {
        printf("No image data \n");
        return -1;
    }

    namedWindow(WINDOW_NAME_ORIGINAL, WINDOW_AUTOSIZE);
    namedWindow(WINDOW_NAME, WINDOW_AUTOSIZE);
    cvui::init(WINDOW_NAME_ORIGINAL);

    Mat segmented;
    int k = 30000;

    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> uni(0, 255);
    vector<Vec3b> random_colors;
    for (int m = 0; m < image.rows * image.cols; ++m)
    {
        Vec3b toPush;
        toPush[0] = uni(rng);
        toPush[1] = uni(rng);
        toPush[2] = uni(rng);
        random_colors.push_back(toPush);
    }

    while (true)
    {
        segmented = graphBasedSegmentationMono(imageCopy, k, random_colors);

        cvui::window(image, 10, 50, 180, 100, "Set k:");
        cvui::trackbar(image, 15, 90, 165, &k, 500, 50000);
        cvui::update();

        imshow(WINDOW_NAME_ORIGINAL, image);
        imshow(WINDOW_NAME, segmented);

        if (cv::waitKey(30) == 27)
        {
            break;
        }
    }

    return 0;
}