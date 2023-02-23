import time
import numpy as np
from copy import deepcopy
from threading import Thread
from random import randint

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
        self.AllowedSlicesReverse()
        self.allowedSlicesList = sorted(self.allowedSlicesList, key = prod, reverse=True)
        self.allowedSlicesList = self.allowedSlicesList[0:2]
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

    def RegArraySlice(self, arrayIngredients):
        tomato = np.count_nonzero(arrayIngredients == '0')
        mushroom = np.count_nonzero(arrayIngredients == '1')
        return (tomato >= int(self.ingrXSlice)) and (mushroom >= int(self.ingrXSlice))

    def CalculateScore(self):
        global CandidateSlicesListScore
        global CandidateSlicesList
        global start_time

        if self.slicesListScore > CandidateSlicesListScore:
            CandidateSlicesList = self.slicesList
            CandidateSlicesListScore = self.slicesListScore
            saveResult(nomeFile + '_arr.out')
            # print time.time() - start_time
            # print CandidateSlicesList
            # print CandidateSlicesListScore

    def Cutter(self):
        global CandidateSlicesListScore
        global CandidateSlicesList
        global start_time
        returnIdx = self.idx
        # partenzaFetta = FindStart(self.arrayIngredients, self.rows, self.cols)
        partenzaFetta = (randint(0, int(self.rows) - 1), randint(0, int(self.cols) - 1))
        tempFetta = ""
        cuttedSlice = False
        for fetta in self.allowedSlicesList[self.idx:]:
            returnIdx = returnIdx + 1

            if (int(fetta[0]) + int(partenzaFetta[0]) <= int(self.rows)) and (
                    int(fetta[1]) + int(partenzaFetta[1]) <= int(self.cols)):
                tempFetta = self.arrayIngredients[int(partenzaFetta[0]):int(partenzaFetta[0]) + fetta[0],
                            int(partenzaFetta[1]):int(partenzaFetta[1]) + fetta[1]]

                if (np.count_nonzero(tempFetta == '9') == 0) and (self.RegArraySlice(tempFetta) == True):
                    cuttedSlice = True
                    break

        if cuttedSlice == True:
            returnIdx = returnIdx - 1
            self.slicesList = self.slicesList + (partenzaFetta, fetta,)
            self.slicesListScore = self.slicesListScore + fetta[0] * fetta[1]
            if self.slicesListScore > CandidateSlicesListScore:
                CandidateSlicesList = self.slicesList
                CandidateSlicesListScore = self.slicesListScore
                saveResult(nomeFile + '_eu.out')
                # print time.time() - start_time
                # print CandidateSlicesList
                # print CandidateSlicesListScore
            self.arrayIngredients[int(partenzaFetta[0]):int(partenzaFetta[0]) + fetta[0],
            int(partenzaFetta[1]):int(partenzaFetta[1]) + fetta[1]] = 9 * np.ones(fetta)
        else:
            # self.CalculateScore()
            returnIdx = self.numAllowedSlices

        return returnIdx


def FindStart(pizzaRestante, rows, cols, ):
    for i in range(0, int(rows)):
        for j in range(0, int(cols)):
            if int(pizzaRestante[i][j]) != 9:
                break
        if int(pizzaRestante[i][j]) != 9:
            break
    if i < rows and j < cols:
        startPoint = (i, j)
    else:
        startPoint = (rows, cols)

    return startPoint


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


while True:
    nomeFile = 'big'
    pizza = Pizza(nomeFile + '.in')
    pizzotto = deepcopy(pizza)
    listaPizze = [pizzotto]

    while len(listaPizze) > 0:
        nextListaPizze = []
        for pizzotto in listaPizze:
            tmpPizzotto = deepcopy(pizzotto)
            newIdx = pizzotto.Cutter()
            if newIdx < pizzotto.numAllowedSlices:
                nextListaPizze.append(pizzotto)
            if newIdx < tmpPizzotto.numAllowedSlices - 1:
                tmpPizzotto.idx = newIdx + 1
                nextListaPizze.append(tmpPizzotto)

        listaPizze = deepcopy(nextListaPizze)

    print CandidateSlicesListScore

#print time.time() - start_time
# print CandidateSlicesList, CandidateSlicesListScore
