#!/bin/bash


for i in $(seq 1 100); do
  echo "### TEST $i"
  ./dpll-solver "benchmarks/example-$i.cnf"
  head -n $i reference.txt | tail -n 1
done
