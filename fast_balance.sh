#!/bin/bash
#this script should run outside plot_test_case folder

echo fast plot script

./bin/DPT_balance_color inputs/case0.in outputs/case0.out

number=(1 2 3 4 5)

for num in ${number[@]};do
	./bin/DPT_balance_color ProblemE_test_case/iccad2015_input.case$num outputs/case$num.out
done
