#!/usr/bin/env python3

def main():
    # write your code

    # server: 手元実行 or クラウド
    # opt: -O2 (C++) / --release (Cargo)
    # dlocal: -DLOCAL (C++ only)
    hashcode = Hashcode(server=True, opt=True, dlocal=True)

    # checker(=得点計算): ./checker testcase solution で標準出力にスコア(整数)を吐き出す
    hashcode.register_checker('calcscore.cpp')

#    hashcode.register_solution('../in/e.txt', 'optex/e_opt.txt')
    hashcode.register_solution('../in/f.txt', 'optex/f_wata.txt')

    while True:
#       result = hashcode.run_with_best('../in/f.txt', './greedy_opt.cpp')
       result = hashcode.run_with_best('../in/f.txt', './assing_opt2.cpp')
       if not result.update:
           break

#    print(hashcode.calculate_score('../in/d.in', 'd_12468835.out'))
#    for i in range(1, 151):
        # hashcode.run(testcase, source, solution, arg = [])
        # hashcode.run_with_best(testcase, source, arg = [])
        # source : ./a.out solution < testcase で標準出力に新しい解を吐き出す
        # run_with_bestだと今の最高値のファイルが自動で入力になる
        # 返り値.update: スコアが更新されたか
        # 返り値.output: 出力ファイルのパス

import platform
from logging import Logger, basicConfig, getLogger
from os import getenv
from pathlib import Path
import signal
from subprocess import check_call, check_output, Popen, PIPE
from typing import List, Union, Dict, Tuple
import random, string
import tempfile
from functools import lru_cache
import time

logger = getLogger(__name__) # type: Logger

# Dropbox/Hashcode2020/ の絶対パス
@lru_cache(maxsize=None)
def dropbox_root() -> Path:
    path = Path().cwd()
    while True:
        if (path / 'hashcode2020.key').exists():
            return path
        if path == Path('/'):
            logger.error('please run under Dropbox/Hashcode2020')
            exit(1)
        path = path.parent


ssh_ip = 'hashcode2020@34.84.206.38' # type: str
key_path = dropbox_root() / 'hashcode2020.key' # type: Path
cwd_on_cloud = Path('Dropbox') / 'Hashcode2020' / \
    Path.cwd().resolve().relative_to(dropbox_root()) # type: Path

# len 文字のランダムな文字列(英小文字)を返す
def random_str(len: int = 12) -> str:
    return ''.join([random.choice(string.ascii_letters) for i in range(len)])

# ランダムな読みやすい(?)文字列を返す
def random_readable_str() -> str:
    random_dict = [
        'tourist',
        'petr',
        'umnik',
        'rng58',
        'iwiwi',
        'kyuri',
        'mikami',
        'wafrelka',
        'kuma',
        'zaki',
        'poteti',
        'koike',
        'google',
        'amazon',
        'facebook',
        'apple',
        'pfn',
        'atcoder',
    ]
    return '{}-{}'.format(random.choice(random_dict), random_str())

# OS Xで実行するかどうかを返す
def run_on_osx(server: bool) -> bool:
    return not server and platform.system() == 'Darwin'

# sourceの言語を調べて返す(cpp / cargo)
def detect_lang(source: Path) -> str:
    if source.suffix == ".cpp":
        return "cpp"
    if (source / "Cargo.toml").exists():
        return "cargo"
    return "unknown"

# cmdを実行する
def execute(cmd: List[str], server: bool, change_cwd: bool = True, input_path: Path = None, output_path: Path = None):
    logger.debug('({}): {}'.format(
        'cloud' if server else 'local',
        ' '.join(cmd)))
    if not Path(cmd[0]).is_absolute and not cmd[0].startswith('.'):
        cmd[0] = './' + cmd[0]
    if server:
        ssh_command = ['ssh', '-i', str(key_path), ssh_ip]
        if change_cwd:
            ssh_command += ['cd', str(cwd_on_cloud), '&&']
        cmd = ssh_command + cmd
        if input_path:
            cmd += ['<', str(input_path)]
        if output_path:
            cmd += ['>', str(output_path)]
        check_call(cmd)        
    else:
        input_file = open(input_path, 'r') if input_path else None
        output_file = open(output_path, 'w') if output_path else None
        check_call(cmd, stdin=input_file, stdout=output_file)

# sourceの中身をbytesとして返す
def read_file(source: Path, server: bool) -> bytes:
    cmd = ['cat', str(source)]
    if server:
        ssh_command = ['ssh', '-i', str(key_path), ssh_ip]
        ssh_command += ['cd', str(cwd_on_cloud), '&&']
        cmd = ssh_command + cmd
    return check_output(cmd)

class ExecResult:
    def __init__(self, output: str, update: bool):
        self.output = output # type: str
        self.update = update # type: bool

class Hashcode:
    def __init__(self, server: bool, opt: bool, dlocal: bool):
        self.server = server # type: bool
        self.opt = opt # type: bool
        self.dlocal = dlocal # type: bool

    _temp_dir = dict() # type: Dict[bool, Path]
    def temp_file(self) -> Path:
        if self.server not in self._temp_dir:
            if not self.server:
                self._temp_dir[False] = Path(tempfile.mkdtemp())
                logger.info('temp dir on local: {}'.format(_temp_dir[False]))
            else:
                temp_dir = Path('/tmp/hashcode-' + random_str())
                logger.info('temp dir on cloud: {}'.format(temp_dir))
                self._temp_dir[True] = temp_dir
                execute(['mkdir', '-p', str(temp_dir)], server=True)
        return self._temp_dir[self.server] / random_str()
    
    compiled_bins = dict() # type: Dict[str, str]
    def compile(self, source: Path) -> Path:
        if str(source) in self.compiled_bins:
            logger.info('{} is already compiled, skipped'.format(source))
            return self.compiled_bins[str(source)]
        logger.info('compile {}'.format(source))
        lang = detect_lang(source)
        command = [] # type: List[str]
        if lang == "cpp":
            command += [getenv('CXX', 'g++')]
            command += ['-std=c++17', '-march=native',
                        '-Wall', '-Wextra', '-Wshadow']
            if run_on_osx(self.server):
                # expand stack for OS X (256MB)
                command += ['-Wl,-stack_size,0x10000000']
            if self.opt:
                command += ['-O2']
            else:
                command += ['-g', '-fsanitize=undefined,address']
            if self.dlocal:
                command += ['-DLOCAL']
            command += [str(source)]
            bin_path = self.temp_file()
            command += ['-o', str(bin_path)]
            execute(command, self.server)
            self.compiled_bins[str(source)] = str(bin_path)
            return bin_path
        elif lang == "cargo":
            command += ['cargo', 'build']
            if self.opt:
                command += ['--release']
            command += ['--manifest-path={}'.format(str(source / 'Cargo.toml'))]
            dir_path = self.temp_file()
            command += ['--target-dir={}'.format(dir_path)]
            execute(command, self.server)
            bin_path = dir_path
            if self.opt:
                bin_path = dir_path / 'release' / 'wata'
            else:
                bin_path = dir_path / 'debug' / 'wata'
            self.compiled_bins[str(source)] = str(bin_path)
            return bin_path
        else:
            logger.error("cannot compile : {}".format(source))
            exit(1)
        
    checker = None # type: Union[Path, None]
    def register_checker(self, source: str):
        if not self.checker:
            logger.info('register checker {}'.format(source))
            self.checker = self.compile(Path(source))
        else:
            logger.info('already registered checker, skipped')

    def calculate_score(self, testcase: str, solution: str) -> int:
        if not self.checker:
            logger.warning('checker is not registered')
            return -1
        output = self.temp_file()
        execute([str(self.checker), testcase, solution], self.server, output_path=output)
        point = read_file(output, self.server)
        try:
            point_int = int(point)
        except ValueError:
            logger.warning('output of checker is invalid {}'.format(point))
            exit(1)
        return point_int

    solutions = dict() # type: Dict[str, List[Tuple[str, int]]]
    def best_solution(self, testcase: str) -> str:
        if testcase not in self.solutions or len(self.solutions[testcase]) == 0:
            logger.error('any solution of {} is not registered'.format(testcase))
            exit(1)
        return max(self.solutions[testcase], key=lambda x: x[1])[0]

    # スコアが更新されたかを返す
    def register_solution(self, testcase: str, solution: str) -> bool:
        score = self.calculate_score(testcase, solution)
        if testcase not in self.solutions:
            self.solutions[testcase] = []
        prev = max(self.solutions[testcase], default=("default", -1), key=lambda x: x[1])[1]
        self.solutions[testcase].append((solution, score))
        if prev < score:
            logger.info('BEST! register solution of {} : {} ({} point)'.format(testcase, solution, score))
        else:
            logger.info('register solution of {} : {} ({} point)'.format(testcase, solution, score))
        return prev < score

    # スコアが更新されたかを返す
    def run(self, testcase: str, source: str, solution: str, arg: [str] = []) -> bool:
        binary = self.compile(Path(source))
        output = random_readable_str() + '.out'
        logger.info('running solution {} for {}, base = {}, output = {}'.format(source, testcase, solution, output))
        execute([str(binary), solution] + arg, server=self.server, input_path=testcase, output_path=output)
        return ExecResult(
            output=output,
            update=self.register_solution(testcase, output))

    # スコアが更新されたかを返す
    def run_with_best(self, testcase: str, source: str, arg: [str] = []) -> bool:
        sol = self.best_solution(testcase)
        return self.run(testcase, source, sol, arg)


if __name__ == '__main__':
    basicConfig(
        level=getenv('LOG_LEVEL', 'INFO'),
        format="[%(levelname)s] %(message)s"
    )
    main()
