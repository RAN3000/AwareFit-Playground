import csv
import cv2
import os
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patches as patches
from joblib import Parallel, delayed

DATASET_FOLDER = "dataset/"
TRAIN_NAME = "full_rects.csv"
TEST_NAME = "full_rects_test.csv"
VALIDATION_NAME = "full_rects_val.csv"
NUM_PROCESSOR = 8

def aspectratio(img_file):
    img = cv2.imread(img_file)
    ar = img.shape[1] / img.shape[0] # w/h
    return ar

def drawaspectratios(aspectratios):
    plt.style.use('dark_background')
    fig = plt.figure()
    ax = fig.add_subplot(111, aspect='equal')

    for ar in aspectratios:
        x = 0
        y = 0
        w = 0
        h = 0
        color = "#E3302140" if ar > 1 else "#50B4B640"

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

    plt.show()
    return

def main():
    if not os.path.exists(DATASET_FOLDER):
        print("Dataset not found")
        return

    imgpaths = []

    with open(DATASET_FOLDER + VALIDATION_NAME, 'r') as csvfile:
        trainreader = csv.reader(csvfile)
        headers = next(trainreader, None)
        for row in trainreader:
            imgpaths.append(row[0])

    aspectratios = [] # w/h
    aspectratios.extend(Parallel(n_jobs=NUM_PROCESSOR)(delayed(aspectratio)(img_file) for img_file in imgpaths))
    drawaspectratios(aspectratios)

if __name__ == '__main__':
    main()