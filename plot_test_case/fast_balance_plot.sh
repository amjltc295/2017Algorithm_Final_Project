#!/bin/bash
#this script should run inside plot_test_case folder

echo fast balance plot script

number=(0 1 2 3 4 5)

for num in ${number[@]};do
    echo "python3 plot_balanced_colored_picture.py ../outputs/case$num.out case${num}_balanced.png"
    python3 plot_balanced.py ../outputs/case$num.out case${num}_balanced.png
    echo ""
done
