#/bin/bash
echo -e  "compiling snake game"
clang -o snake snake.c `sdl2-config --cflags --libs`
echo -e "\033[32mSuccessfully compiled snake game\033[0m"