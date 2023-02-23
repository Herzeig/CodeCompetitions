import time
import numpy as np
from copy import deepcopy
from threading import Thread
#from random import randint
import random as rnd
import itertools

CandidateSlicesList = ()
CandidateSlicesListScore = 0
start_time = time.time()

def prod(elem):
    return int(elem[0]) * int(elem[1])

class Pizza:

    def __init__(self, filename):
        F = open(filename, "r")
        self.rows, self.cols, self.ingrXSlice, self.maxSliceSize = F.readline().split(' ')
        self.arrayIngredients = np.array([list(line.strip()) for line in F.readlines()])
        np.place(self.arrayIngredients, self.arrayIngredients == 'M', 1)
        np.place(self.arrayIngredients, self.arrayIngredients == 'T', 0)
        self.allowedSlicesList = ()
        self.AllowedSlices()
        self.allowedSlicesList = sorted(self.allowedSlicesList, key = prod, reverse=True)
        self.allowedSlicesList = self.allowedSlicesList
        self.numAllowedSlices = len(self.allowedSlicesList)
        self.slicesList = ()
        self.slicesListScore = 0
        self.idx = 0
        F.close()

    def AllowedSlices(self):
        for i in range(1, int(self.rows) + 1):
            for j in range(1, int(self.cols) + 1):
                if ((i * j > 1) and (i * j <= int(self.maxSliceSize)) and (i * j >= int(self.ingrXSlice) * 2)):
                    self.allowedSlicesList = self.allowedSlicesList + ((i, j),)

    def AllowedSlicesReverse(self):
        for i in range(int(self.rows) + 1, 0, -1):
            for j in range(int(self.cols) + 1, 0, -1):
                if ((i * j > 1) and (i * j <= int(self.maxSliceSize)) and (i * j >= int(self.ingrXSlice) * 2)):
                    self.allowedSlicesList = self.allowedSlicesList + ((i, j),)

    def CutAndValuate(self, idxSlice):
        global CandidateSlicesListScore
        global CandidateSlicesList
        #global start_time
        i = 0
        j = 0

        h, w = int(self.allowedSlicesList[idxSlice][0]), int(self.allowedSlicesList[idxSlice][1])

        while i < int(self.rows):
            while j < int(self.cols):
                fetta = self.arrayIngredients[i:i + h, j:j + w]
                if (fetta[fetta == '9'].size == 0) and (fetta[fetta == '0'].size >= int(self.ingrXSlice)) and (fetta[fetta == '1'].size >= int(self.ingrXSlice)):
                    self.arrayIngredients[i:i + h, j:j + w] = '9'
                    CandidateSlicesList = CandidateSlicesList + ((i, j), (h, w),)
                    CandidateSlicesListScore = CandidateSlicesListScore + h * w
                j = j + int(self.allowedSlicesList[0][1])
            j = 0
            i = i + int(self.allowedSlicesList[0][0])

        saveResult(nomeFile + '_sc.out')
        print CandidateSlicesListScore, time.time() - start_time
        #print CandidateSlicesList
        #print CandidateSlicesListScore

def saveResult(filename):
    sliceStart = ()
    size = False

    F = open(filename, "r")
    score = F.readline()
    F.close()

    if (CandidateSlicesListScore > int(score)):
        F = open(filename, "w")
        F.write("{0}\n".format(CandidateSlicesListScore))
        F.write("{0}\n".format(len(CandidateSlicesList) / 2))
        for slice in CandidateSlicesList:
            if size == False:
                sliceStart = slice
                size = True
            else:
                F.write("{0} {1} {2} {3}\n".format(sliceStart[0], sliceStart[1], sliceStart[0] + int(slice[0]) - 1,
                                                   sliceStart[1] + int(slice[1]) - 1))
                sliceStart = ()
                size = False

        F.close()

nomeFile = 'big'
pizza = Pizza(nomeFile + '.in')
permutazioni = itertools.permutations(pizza.allowedSlicesList)
for permuta in permutazioni:
    #rnd.shuffle(pizza.allowedSlicesList)
    pizza.allowedSlicesList = permuta
    for i in range(0, pizza.numAllowedSlices):
        pizza.CutAndValuate(i)
