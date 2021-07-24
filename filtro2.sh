#!/bin/bash
echo "calculando Memory bandwidth"
cpu=3
# 64 100 128 1000 1024 2000 2048 3000 4096 5000
for i in 64 100 128 1000 1024 2000 2048 3000 4096 5000
do
    ./perfctr $cpu L3 ./matmult -n $i |grep "L3 bandwidth" | awk '{print  $6}' | awk -v d=" " '{s=(NR==1?s:s d)$0}END{print s}' | awk '{print "'$i'", $1, $2, "\n'$i'", $3, $4}' > output
    cat output | awk "NR==1" >> Resultados/bandwidth1
    cat output | awk "NR==2" >> Resultados/bandwidth2

done
echo "calculando L2 CACHE"
for i in 64 100 128 1000 1024 2000 2048 3000 4096 5000
do
    ./perfctr $cpu L2CACHE ./matmult -n $i | grep "L2 miss ratio" | awk '{print  $6}' | awk -v d=" " '{s=(NR==1?s:s d)$0}END{print s}' | awk '{print "'$i'", $1, $2, "\n'$i'", $3, $4}' > output
    cat output | awk "NR==1" >> Resultados/cache1
    cat output | awk "NR==2" >> Resultados/cache2

done
echo "calculando FLOPS"
for i in 64 100 128 1000 1024 2000 2048 3000 4096 5000 
do
    ./perfctr $cpu FLOPS_DP ./matmult -n $i | grep "DP" | sed '/^R/d' | sed 's/|//g' | awk '{print $NF}' |  awk -v d=" " '{s=(NR==1?s:s d)$0}END{print s}'| awk '{print "'$i'", $1, $2, $3, $4, "\n'$i'", $5, $6, $7, $8}' > output
    cat output | awk "NR==1" >> Resultados/flops1
    cat output | awk "NR==2" >> Resultados/flops2

done
echo "calculando runtime"
for i in 64 100 128 1000 1024 2000 2048 3000 4096 5000 
do
    ./perfctr $cpu CPU_CLK_UNHALTED_CORE:FIXC1 ./matmult -n $i | grep "Runtime" | sed 's/|//g' | awk '{print $NF}'| awk -v d=" " '{s=(NR==1?s:s d)$0}END{print s}' | awk '{print "'$i'", $2*1000, $4*1000, "\n'$i'", $6*1000, $8*1000}' > output
    cat output | awk "NR==1" >> Resultados/runtime1
    cat output | awk "NR==2" >> Resultados/runtime2

done



