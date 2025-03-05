#!/bin/bash
g++ $1 -o out -I/opt/homebrew/Cellar/sfml/3.0.0_1/include -L/opt/homebrew/Cellar/sfml/3.0.0_1/lib -std=c++20 -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
./out
