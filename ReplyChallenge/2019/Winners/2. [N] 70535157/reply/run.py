#!/usr/bin/python3
import os
import sys
from multiprocessing import Pool

fn = sys.argv[1]
os.system('./make.sh score {}'.format(fn))
os.system('mkdir -p outputs')

names = [
    'a_solar',
    'b_dream',
    'c_soup',
    'd_maelstrom',
    'e_igloos',
    'f_glitch',
]

total = 0
results = []


def solve1(name):
    old_score = 0
    try:
        old_score = int(open('outputs/score_%s.txt' % (name)).read())
    except:
        pass
    global total
    global results
    os.system('./build/%s outputs2/%s.ans < tests/%s.txt > outputs/%s_tmp.ans' % (fn, name, name, name))
    os.system('./build/score tests/%s.txt outputs/%s_tmp.ans > outputs/score_%s_tmp.txt' % (name, name, name))

    cur = int(open('outputs/score_%s_tmp.txt' % (name)).read())
    print('Test %s: %d' % (name, cur))
    #os.system('rm outputs/score_%s.txt' % (name))
    if old_score < cur:
        print('increased {} {}!'.format(name, cur - old_score))
        os.system('cp outputs/{0}_tmp.ans outputs/{0}.ans'.format(name))
        os.system('cp outputs/score_{0}_tmp.txt outputs/score_{0}.txt'.format(name))
    os.system('rm outputs/score_{}_tmp.txt'.format(name))
    os.system('rm outputs/{}_tmp.ans'.format(name))
    result.append(cur)
    total += cur


def main():
    pool = Pool()

    total = 0
    waits = []
    for name in names:
        waits.append(pool.apply_async(solve1, (name, )))

    for wait in waits:
        wait.wait()

    os.system('./archive.sh > /dev/null 2> /dev/null')

    print('===================')
    print('Total: {} {}'.format(total, sum(results)))

main()
