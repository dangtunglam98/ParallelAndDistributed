#!/bin/bash

strong_scaling() {
    echo $HOSTNAME > strong_scaling.txt
    echo | grep -m 1 'MHz' /proc/cpuinfo >> strong_scaling.txt
    echo "500 per dimension" >> strong_scaling.txt
    echo "Strong Scaling" >> strong_scaling.txt
    for thread in 1 2 4 8 
        do 
            echo "$(./lam_mmm -d 500 -r 10 -t $thread \n)" >> strong_scaling.txt
        done
    python3 plotting.py strong_scaling.txt
}

weak_scaling() {
    echo $HOSTNAME > weak_scaling.txt
    echo | grep -m 1 'MHz' /proc/cpuinfo >> weak_scaling.txt
    echo "num_threads* 100 per dim" >> weak_scaling.txt
    echo "Weak Scaling" >> weak_scaling.txt
    for thread in 1 2 4 8 
        do 
            echo $(./lam_mmm -d $(($thread*100)) -r 10 -t $thread) >> weak_scaling.txt
        done
    python3 plotting.py weak_scaling.txt
}
strong_scaling
weak_scaling