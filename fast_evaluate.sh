#!/bin/bash

echo fast evaluate the output files

number=(0 1 2 3 4 5)

for num in ${number[@]};do
	echo
	echo evaluating case$num
	python2 evaluate.py outputs/case$num.out
done
