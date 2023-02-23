import time
from copy import deepcopy

CandidateSlicesList = ()
CandidateSlicesListScore = 0
start_time = time.time()

class Pizza:

    def __init__(self, filename):
        F = open(filename, "r")
        self.rows, self.cols, self.ingrXSlice, self.maxSliceSize = F.readline().split(' ')
        self.ingredients = [list(line.strip()) for line in F.readlines()]

        self.allowedSlicesList = ()
        self.AllowedSlices()
        self.numAllowedSlices = len(self.allowedSlicesList)

        self.slicesList = ()

        self.idx = 0

        F.close()

    def AllowedSlices(self):
        for i in range(1, int(self.rows) + 1):
            for j in range(1, int(self.cols) + 1):
                if ((i * j > 1) and (i * j <= int(self.maxSliceSize)) and (i * j >= int(self.ingrXSlice) * 2)):
                    self.allowedSlicesList = self.allowedSlicesList + ((i, j),)

    def RegSlice(self, ingredients):
        return (ingredients.count('M') >= int(self.ingrXSlice)) and (ingredients.count('T') >= int(self.ingrXSlice))

    def Cutter(self):
        returnIdx = self.idx
        partenzaFetta = FindStart(self.ingredients, self.rows, self.cols)
        tempFetta = ""
        cuttedSlice = False
        regSlice = False
        for fetta in self.allowedSlicesList[self.idx:]:
            returnIdx = returnIdx + 1
            illegalSlice = False
            for i in range(0, fetta[0]):
                for j in range(0, fetta[1]):
                    if (i + partenzaFetta[0] < int(self.rows)) and (j + partenzaFetta[1] < int(self.cols)):
                        if self.ingredients[partenzaFetta[0] + i][partenzaFetta[1] + j] != ' ':
                            tempFetta = tempFetta + self.ingredients[partenzaFetta[0] + i][partenzaFetta[1] + j]
                        else:
                            illegalSlice = True
                    else:
                        illegalSlice = True
            if illegalSlice == False:
                regSlice = self.RegSlice(tempFetta)
                if regSlice == True:
                    cuttedSlice = True
                    break

        if (cuttedSlice == True) and (regSlice == True):
            returnIdx = returnIdx - 1
            self.slicesList = self.slicesList + (partenzaFetta, fetta,)
            for i in range(0, fetta[0]):
                for j in range(0, fetta[1]):
                    self.ingredients[partenzaFetta[0] + i][partenzaFetta[1] + j] = ' '
        else:
            CalculateScore(self.slicesList)
            returnIdx = self.numAllowedSlices

        return returnIdx


def CalculateScore(Lista):
    global CandidateSlicesListScore
    global CandidateSlicesList
    global start_time
    score = 0
    size = False
    for tuplan in Lista:
        if size == False:
            size = True
        else:
            size = False
            score = score + int(tuplan[0]) * int(tuplan[1])

    if score > CandidateSlicesListScore:
        CandidateSlicesList = Lista
        CandidateSlicesListScore = score
        saveResult(nomeFile + '.out')
#       print CandidateSlicesList
        print time.time() - start_time
        print CandidateSlicesListScore
    return score

def FindStart(pizza, rows, cols,):
    for i in range(0,int(rows)):
        for j in range(0,int(cols)):
            if pizza[i][j] != ' ':
                break
        if pizza[i][j] != ' ':
            break
    if i < rows and j < cols:
        startPoint = (i, j)
    else:
        startPoint = (rows, cols)

    return startPoint

def saveResult(filename):
    sliceStart = ()
    size = False
    F = open(filename, "w")

    F.write("{0}\n".format(len(CandidateSlicesList)/2))
    for slice in CandidateSlicesList:
        if size == False:
            sliceStart = slice
            size = True
        else:
            F.write("{0} {1} {2} {3}\n".format(sliceStart[0], sliceStart[1], sliceStart[0] + int(slice[0]) - 1, sliceStart[1] + int(slice[1]) - 1))
            sliceStart = ()
            size = False

    F.close()

nomeFile = 'small'
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

print time.time() - start_time
print CandidateSlicesList, CandidateSlicesListScore
