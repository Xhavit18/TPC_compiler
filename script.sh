#!/bin/bash

make

echo > rapport_tests.txt

test() {
    files=($1)
    n=0
    passed=0

    for f in ${files[@]}; do
        let n++
        printf "(%02d/%02d) - %s\n" $n ${#files[@]} "Testing $f..."  | tee -a rapport_tests.txt

        bin/./tpcc < $f | cat | sed -r "s/[[:cntrl:]]\[[0-9]{1,3}m//g" >> rapport_tests.txt
        feedback=$PIPESTATUS

        echo $feedback >> rapport_tests.txt
        echo >> rapport_tests.txt

        if [ "$feedback" = "$2" ]; then
            let passed++
        fi
    done

    if [ $passed -eq ${#files[@]} ]; then
        echo -e "\033[0;32m$passed tests out of ${#files[@]} passed! \033[0;37m"
    elif [ $passed = "0" ]; then
        echo -e "\033[0;31m$passed tests out of ${#files[@]} passed! \033[0;37m"
    else
        echo -e "\033[0;33m$passed tests out of ${#files[@]} passed! \033[0;37m"
    fi
}

echo "=============== Tests corrects ===============" | tee -a rapport_tests.txt
test "test/good/*.tpc" 0

echo "============== Tests incorrects syntaxiquement ==============" | tee -a rapport_tests.txt
test "test/syn-err/*.tpc" 1

echo "============== Tests incorrects sémantiquement ==============" | tee -a rapport_tests.txt
test "test/sem-err/*.tpc" 2

echo "============== Tests provoquant des warnings ==============" | tee -a rapport_tests.txt
test "test/warn/*.tpc" 0

echo "Done."
