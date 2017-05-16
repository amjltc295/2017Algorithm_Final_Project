#!/bin/bash
#this script should run outside plot_test_case folder

echo fast plot script

number=(1 2 3 4 5)

for num in ${number[@]};do
	python plot_test_case/plot_picture.py ProblemE_test_case/iccad2015_input.case$num plot_test_case/case$num.png
done