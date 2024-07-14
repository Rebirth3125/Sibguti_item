#!/bin/bash
VAR1=$1
VAR2="lab1"
VAR3="lab2"
VAR4="clean"

lab1_algs=(DiffiAlgorithm EuclidAlgorithm FastExpAlgorithm ShanksAlgorithm)
lab2_algs=(Elgamal RSA Shamir Vernam)

function assemble_lib() {
    cd libs
    make
    cd ..
}

function assemble_lab1() {
    cd lab1
    for var in ${lab1_algs[@]};
    do
    cd $var
    make
    rm ${var}.o
    cd ..
    done
    cd ..
}

function assemble_lab2() {
    cd lab2
    for var in ${lab2_algs[@]};
    do
    cd $var
    make
    rm ${var}.o
    cd ..
    done
    cd ..
}

function run_lab1() {
    cd lab1
    for var in ${lab1_algs[@]};
    do
    cd $var
    echo
    echo $var
    ./$var
    cd ..
    done
    cd ..
}

function run_lab2() {
    cd lab2
    for var in ${lab2_algs[@]};
    do
    cd $var
    echo
    echo $var
    ./$var
    cd ..
    done
    cd ..
}

function rm_exec_files() {
    cd libs/Lab1
    rm Lab1algs.o
    rm libbasicalgs.a
    cd ../..
    
    cd lab1
    for var in ${lab1_algs[@]};
    do
    cd $var
    rm $var
    cd ..
    done
    cd ..

    cd lab2
    for var in ${lab2_algs[@]};
    do
    cd $var
    rm $var
    cd ..
    done
    cd ..
}

if [ "$VAR1" = "$VAR2" ]; then
    assemble_lib
    assemble_lab1
    run_lab1
elif [ "$VAR1" = "$VAR3" ]; then
    assemble_lib
    assemble_lab2
    run_lab2
elif [ "$VAR1" = "$VAR4" ]; then
    rm_exec_files
fi