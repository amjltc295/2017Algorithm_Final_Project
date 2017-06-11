#!/bin/bash
#this script should run outside plot_test_case folder

echo fast dot -Tpng script

number=(0 1 2 3 4 5)

for num in ${number[@]};do
	dot -Tpng outputs/case$num.out -o outputs/case$num.png
done
