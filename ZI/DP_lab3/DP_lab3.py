import os
import shutil
from lab2 import elgamal_encode

from lab3 import *
from lab3 import rsa_sign
from lab3 import gost_sign
from lab3 import elgamal_sign


def read_file(filename: str, ext: str) -> bytearray:
    with open(filename + '.' + ext, 'rb') as origin_file:
        return bytearray(origin_file.read())


def main() -> int:
    try:
        shutil.rmtree('signs')
    except OSError:
        pass
    os.mkdir('signs')

    filename = 'original'
    ext = 'png'
    m = read_file(filename, ext)

    print('Encoding with Elgamal...')
    elg_en = elgamal_encode(m)
    print(f'Sign with RSA is: {rsa_sign(m)}')
    print(f'Sign with Elgamal is: {elgamal_sign(m)}')
    print(f'Sign with GOST is: {gost_sign(m)}')
    return 0


if __name__ == '__main__':
    exit(main())
