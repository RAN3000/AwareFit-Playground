import csv
import imagesize
import os
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patches as patches
from joblib import Parallel, delayed
import randomcolor
import random

DATASET_FOLDER = "dataset/"
TRAIN_NAME = "full_rects.csv"
TEST_NAME = "full_rects_test.csv"
VALIDATION_NAME = "full_rects_val.csv"
NUM_PROCESSOR = 8


def aspectratio(img_file, cl, colors_dict):
    width, height = imagesize.get(img_file)
    ar = width / height  # w/h
    color = colors_dict[cl]
    return (ar, color)


def drawaspectratios(aspectratios, colors_dict):
    plt.style.use('dark_background')
    fig = plt.figure()
    ax = fig.add_subplot(111, aspect='equal')

    for (ar, color) in aspectratios:
        x = 0
        y = 0
        w = 0
        h = 0
#        color = "#E3302140" if ar > 1 else "#50B4B640"

        if ar > 1:
            y = 1.5
            h = 1
            w = ar * h
            x = 3 - w/2
        else:
            x = 2.5
            w = 1
            h = w / ar
            y = 2 - h/2

        ax.add_patch(
            patches.Rectangle((x, y), w, h, fill=False, color=color, linewidth=0.7))

    plt.ylim((0, 4))
    plt.xlim((0, 6))

    legend_patches = []
    for cl, color in colors_dict.items():
        legend_patches.append(patches.Patch(color=color, label=cl))
    plt.legend(handles=legend_patches)

    plt.show()
    return


def main():
    if not os.path.exists(DATASET_FOLDER):
        print("Dataset not found")
        return

    imgpaths = []
    classes = []
    classes_set = []

    with open(DATASET_FOLDER + VALIDATION_NAME, 'r') as csvfile:
        trainreader = csv.reader(csvfile)
        headers = next(trainreader, None)
        for row in trainreader:
            imgpaths.append(row[0])
            classes.append(row[1])
            if row[1] not in classes_set:
                classes_set.append(row[1])

    if len(imgpaths) != len(classes):
        print("Wrong sizes between imgpaths and classes.")

    rand_color = randomcolor.RandomColor()
    colors = rand_color.generate(luminosity='bright', count=len(classes_set))

    colors_dict = {cl: color for (cl, color) in zip(classes_set, colors)}

    aspectratios = []  # (w/h, color)
    aspectratios.extend(Parallel(n_jobs=NUM_PROCESSOR)(delayed(aspectratio)(
        img_file, cl, colors_dict) for (img_file, cl) in zip(imgpaths, classes)))
    random.shuffle(aspectratios)
    drawaspectratios(aspectratios, colors_dict)


if __name__ == '__main__':
    main()
