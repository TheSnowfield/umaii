#!/usr/bin/python3

import io
import os
import sys
import subprocess
from datetime import datetime

def main():

  # check for correct number of arguments
  if len(sys.argv) != 5:
    print(sys.argv)
    print("Usage: sign.py <build id> <img offset> <input file> <output file>")
    sys.exit(1)

  flen = os.stat(sys.argv[4])
  offset = int(sys.argv[3])
  hash = subprocess.check_output(["git", "describe", "--always"]).decode("utf-8").replace("\n","")

  # generate header
  fp = open(sys.argv[5], "wb")
  fp.seek(io.SEEK_SET, 0)
  fp.write(bytearray("UMFW".encode("utf-8")))
  fp.write(bytearray("v0.0.1_abcde".encode("utf-8")))
  fp.write(bytearray(flen.st_size.to_bytes(4, byteorder="little")))
  fp.write(bytearray(0xCCCCCCCC.to_bytes(4, byteorder="little")))
  fp.write(bytearray(int(sys.argv[3]).to_bytes(4, byteorder="little")))

  # image base
  fp.write(bytearray(offset.to_bytes(4, byteorder="little")))
  # image ivt
  fp.write(bytearray((offset + 1).to_bytes(4, byteorder="little")))
  # image stack pointer
  fp.write(bytearray(offset.to_bytes(4, byteorder="little")))

  fp.close()


if __name__ == "__main__":
  main()
