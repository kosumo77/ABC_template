#!/bin/bash
readonly arg1=$1
readonly arg2=$2
readonly arg3=$3
readonly alpha=("a" "b" "c" "d" "e")
readonly ALPHA=("A" "B" "C" "D" "E")
if [ "$arg1" = "make" ];then
    if [ "$arg2" ];then
        mkdir -p "$arg2"
        cp ~/program/template.cpp "$arg2"/main.cpp
        cp ./set.sh.template "$arg2"/set.sh
        chmod +x "$arg2"/set.sh
    else 
        echo "No arg"
    fi
elif [ "$arg1" = "test" ];then
    echo "${alpha[@]}"
elif [ "$arg1" = "all" ];then
    if [ "$arg2" ] && [ "$arg3" ];then
         contest_num=""
         dir_name=""

        # Check if arg2 starts with 'abc'
        if [[ "$arg2" =~ ^abc ]]; then
            dir_name="$arg2"
            contest_num=$(echo "$arg2" | sed -E 's/[^0-9]*([0-9]+).*/\1/')
        else
            dir_name="abc$arg2"
            contest_num="$arg2"
        fi
        
        # Create the main contest directory (e.g., "abc444")
        mkdir -p "$dir_name"
        cp ./set.sh.template "$dir_name"/set.sh
        chmod +x "$dir_name"/set.sh

        idx=0
        # 小文字
        if [[ "${alpha[*]}" =~ $arg3 ]]; then
            while [ "$idx" -lt "${#alpha[@]}" ]; do
                cp ~/program/template.cpp "$dir_name/${alpha[$idx]}".cpp
                if [ "${alpha[$idx]}" = "$arg3" ]; then
                    break
                fi
                ((idx++))
            done
        # 大文字
        elif [[ "${ALPHA[*]}" =~ $arg3 ]]; then
            while [ "$idx" -lt "${#ALPHA[@]}" ]; do
                cp ~/program/template.cpp "$dir_name/${ALPHA[$idx]}".cpp
                if [ "${ALPHA[$idx]}" = "$arg3" ]; then
                    break
                fi
                ((idx++))
            done
        fi
    fi
elif [ "$arg1" = "comp" ];then
    if [ "$arg2" ];then
        if [ -e "$arg2"/main.cpp ];then
            g++ "$arg2"/main.cpp -o "$arg2"/a.out -I"$HOME/program/ac-library"
        else
            echo "No file in this directry"
        fi
    else
        echo "No arg"
    fi
elif [ "$arg1" = "comprun" ];then
    if [ "$arg2" ];then
        if [ -e "$arg2"/main.cpp ];then
            g++ "$arg2"/main.cpp -o "$arg2"/a.out -I"$HOME/program/ac-library"
            "$arg2"/a.out
        else
            echo "No file in this directry"
        fi
    else
        echo "No arg"
    fi
else 
    echo "use command"
fi