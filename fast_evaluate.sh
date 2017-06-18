#!/bin/bash

echo fast evaluate

python evaluate.py outputs/case0.out

number=(1 2 3 4 5)

for num in ${number[@]};do
	python2 evaluate.py outputs/case$num.out
done
