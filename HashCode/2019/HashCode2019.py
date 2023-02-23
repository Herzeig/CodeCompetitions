import time
#import numpy as np
from copy import deepcopy
from threading import Thread

#start_time = time.time()

class SlideShow:

    def __init__(self, filename):
        F = open(filename, "r")
        self.slides = []
        self.orizzontali = []
        self.verticali = []
        self.numFoto = int(F.readline())
        self.numH = 0
        self.numV = 0
        for i in range(0, self.numFoto):
            tmp = F.readline()
            self.foto = [e for e in tmp[:-1].split(' ')]
            self.prova = [self.trasformaTag(e) for e in self.foto[2:]]
            self.prova.insert(0, int(self.foto[1]))
            self.prova.insert(0, i)
            if self.foto[0]=='H':
                self.orizzontali.append(self.prova)
                self.numH = self.numH + 1
            else:
                self.verticali.append(self.prova)
                self.numV = self.numV + 1
        F.close()

    def trasformaTag(self, tag):
        temp = 0
        if len(tag) >= 2:
            temp = ord(tag[1])
            for i in range(2, len(tag)):
                temp = temp * ord(tag[i])
        return temp

    def unisciVerticali(self):
        self.verticali.sort(key=lambda x: x[1], reverse=True)

        riferimento = self.verticali[0]
        rif_Si = self.verticali[0][2:]
        self.verticali.pop(0)

        while len(self.verticali) > 0:
            Si1 = 0
            candidate_Si1 = 0
            score = 100000
            while Si1 < 1000:
                score2 = self.numComuni(rif_Si, self.verticali[Si1][2:])
                if score2 < score:
                    score = score2
                    candidate_Si1 = Si1
                Si1 = Si1 + 1
                if Si1 >= len(self.verticali) or (score2 == 0):
                    break

            a = riferimento
            b = self.verticali[candidate_Si1]
            num = a[1] + b[1] - self.numComuni(a[2:], b[2:])
            c = list(set(a[2:] + b[2:]))
            c.insert(0, num)
            c.insert(0, [a[0], b[0]])
            self.orizzontali.append(c)
            self.verticali.pop(candidate_Si1)

            if len(self.verticali) > 0:
                rif_Si = self.verticali[0][2:]
                riferimento = self.verticali[0]
                self.verticali.pop(0)


        # for i in range(0, self.numV, 2):
        #     a = self.verticali[i]
        #     b = self.verticali[i+1]
        #     #for b in self.verticali[self.numV // 2 + 1:]:
        #     num = a[1] + b[1] - self.numComuni(a[2:], b[2:])
        #     c = list(set(a[2:] + b[2:]))
        #     c.insert(0, num)
        #     c.insert(0, [a[0], b[0]])
        #     self.orizzontali.append(c)



    def creaShow(self):
        self.orizzontali.sort(key = lambda x: x[1], reverse=True)

        self.slides.append(self.orizzontali[0])
        rif_Si = self.orizzontali[0][2:]
        self.orizzontali.pop(0)

        while len(self.orizzontali) > 0:
            Si1 = 0
            candidate_Si1 = 0
            score = 0
            while (self.orizzontali[Si1][1] // 2 > score) and (Si1 < 1000):
                score2 = self.numComuni(rif_Si, self.orizzontali[Si1][2:])
                if score2 > score:
                    score = score2
                    candidate_Si1 = Si1
                Si1 = Si1 + 1
                if Si1 >= len(self.orizzontali):
                    break

            self.slides.append(self.orizzontali[candidate_Si1])
            rif_Si = self.orizzontali[candidate_Si1][2:]
            self.orizzontali.pop(candidate_Si1)
            print(len(self.orizzontali))

    def numComuni(self, list1, list2):
        return len(list(set(list1) & set(list2)))

    def convertMe(self, slide):
        if isinstance(slide, list):
            return str(slide[0]) + ' ' + str(slide[1])
        else:
            return slide

    def saveResult(self, filename):
        F = open(filename, "w")
        F.write("{0}\n".format(len(self.slides)))
        for slide in self.slides:
            F.write("{0}\n".format(self.convertMe(slide[0])))
        F.close()

#nomeFile = 'b_lovely_landscapes'
#nomeFile = 'c_memorable_moments'
#nomeFile = 'd_pet_pictures'
nomeFile = 'e_shiny_selfies'
slide = SlideShow(nomeFile + '.txt')
if(slide.numV > 0):
    slide.unisciVerticali()
slide.creaShow()
slide.saveResult(nomeFile + '_out.txt')
print('Letto')