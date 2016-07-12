#!/bin/bash
ctags *.?pp
g++ main.cpp -o a.main.out && ./a.main.out
