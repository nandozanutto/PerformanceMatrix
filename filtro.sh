#!/bin/bash
echo "calculando Memory bandwidth"
cpu=3
# for i in {1..4};
# do
#     ./perfctr $cpu L3 ./matmult -n 64 | grep "L3 bandwidth" | awk '{print "64", $6}' | awk "NR==${i}" > Resultados/bandwidth$i
#     ./perfctr $cpu L3 ./matmult -n 100 | grep "L3 bandwidth" | awk '{print "100", $6}' | awk "NR==${i}" >> Resultados/bandwidth$i
#     ./perfctr $cpu L3 ./matmult -n 128 | grep "L3 bandwidth" | awk '{print "128", $6}' | awk "NR==${i}" >> Resultados/bandwidth$i
#     ./perfctr $cpu L3 ./matmult -n 2000 | grep "L3 bandwidth" | awk '{print "2000", $6}' | awk "NR==${i}" >> Resultados/bandwidth$i
#     ./perfctr $cpu L3 ./matmult -n 2048 | grep "L3 bandwidth" | awk '{print "2048", $6}' | awk "NR==${i}" >> Resultados/bandwidth$i
# done
# echo "calculando data cache miss ratio"
# for i in {1..4};
# do
#     ./perfctr $cpu L2CACHE ./matmult -n 64 | grep "L2 miss ratio" | awk '{print "64", $6}' | awk "NR==${i}" > Resultados/cacheMiss$i
#     ./perfctr $cpu L2CACHE ./matmult -n 100 | grep "L2 miss ratio" | awk '{print "100", $6}' | awk "NR==${i}" >> Resultados/cacheMiss$i
#     ./perfctr $cpu L2CACHE ./matmult -n 128 | grep "L2 miss ratio" | awk '{print "128", $6}' | awk "NR==${i}" >> Resultados/cacheMiss$i
#     ./perfctr $cpu L2CACHE ./matmult -n 2000 | grep "L2 miss ratio" | awk '{print "2000", $6}' | awk "NR==${i}" >> Resultados/cacheMiss$i
#     ./perfctr $cpu L2CACHE ./matmult -n 2048 | grep "L2 miss ratio" | awk '{print "2048", $6}' | awk "NR==${i}" >> Resultados/cacheMiss$i
# done
echo "calculando flops_dp e flops_avx"
aux=0
for i in 2 5 8 11;
do
    aux=$((aux+1))
    ./perfctr $cpu FLOPS_DP ./matmult -n 64 | grep "DP" | awk 'NR=='$i' {print "64", $5} NR=='$i+1' {print $6}' | awk '{printf("%s ", $0)}' > Resultados/flops$aux   
    echo "" >> Resultados/flops$aux
    ./perfctr $cpu FLOPS_DP ./matmult -n 100 | grep "DP" | awk 'NR=='$i' {print "100", $5} NR=='$i+1' {print $6}' | awk '{printf("%s ", $0)}' >> Resultados/flops$aux
    echo "" >> Resultados/flops$aux
    ./perfctr $cpu FLOPS_DP ./matmult -n 128 | grep "DP" | awk 'NR=='$i' {print "128", $5} NR=='$i+1' {print $6}' | awk '{printf("%s ", $0)}' >> Resultados/flops$aux
    echo "" >> Resultados/flops$aux
    ./perfctr $cpu FLOPS_DP ./matmult -n 2000 | grep "DP" | awk 'NR=='$i' {print "2000", $5} NR=='$i+1' {print $6}' | awk '{printf("%s ", $0)}' >> Resultados/flops$aux
    echo "" >> Resultados/flops$aux
    ./perfctr $cpu FLOPS_DP ./matmult -n 2048 | grep "DP" | awk 'NR=='$i' {print "2048", $5} NR=='$i+1' {print $6}' | awk '{printf("%s ", $0)}' >> Resultados/flops$aux
done
# echo "calculando runtime"
# aux=0
# for i in 2 4 6 8;
# do
#     aux=$((aux+1))
#     ./perfctr $cpu CPU_CLK_UNHALTED_CORE:FIXC1 ./matmult -n 64 | grep "Runtime" | awk 'NR=='$i' {print 64, $8, "x 1000"}' > Resultados/time$aux
#     ./perfctr $cpu CPU_CLK_UNHALTED_CORE:FIXC1 ./matmult -n 100 | grep "Runtime" | awk 'NR=='$i' {print 100, $8, "x 1000"}' >> Resultados/time$aux
#     ./perfctr $cpu CPU_CLK_UNHALTED_CORE:FIXC1 ./matmult -n 128 | grep "Runtime" | awk 'NR=='$i' {print 128, $8, "x 1000"}' >> Resultados/time$aux
#     ./perfctr $cpu CPU_CLK_UNHALTED_CORE:FIXC1 ./matmult -n 2000 | grep "Runtime" | awk 'NR=='$i' {print 2000, $8, "x 1000"}' >> Resultados/time$aux
#     ./perfctr $cpu CPU_CLK_UNHALTED_CORE:FIXC1 ./matmult -n 2048 | grep "Runtime" | awk 'NR=='$i' {print 2048, $8, "x 1000"}' >> Resultados/time$aux
# done