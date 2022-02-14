#!/bin/bash
if [ -z "$1" ]
        then
                echo "you must give a project name!"
        else
                mkdir "apps/$1"
                cp templates/Cnc_Qt_Creator_template "apps/$1/$1".pro
                cp templates/main_template.cpp "apps/$1/main.cpp"
                if [ -z "$2" ]
                then
                      qtcreator "cnc_lib/cnc_lib.pro" "apps/$1/$1".pro &
                fi
fi

