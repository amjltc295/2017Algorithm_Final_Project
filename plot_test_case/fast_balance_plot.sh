#!/bin/bash
#this script should run outside plot_test_case folder

echo fast balance plot script

number=(1 2 3 4 5)

for num in ${number[@]};do
    echo "python plot_unbalanced_colored_picture.py ../outputs/case$num.out case${num}_balance.png"
    python plot_unbalanced.py ../outputs/case$num.out case${num}_balance.png
    echo ""
done
