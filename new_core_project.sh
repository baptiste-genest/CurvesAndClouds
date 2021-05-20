#!/bin/bash

if [ -z "$1" ]
        then
                echo "you must give a project name!"
        else
                mkdir "apps/$1"
                cp templates/Cnc_Qt_Creator_Core_template "apps/$1/$1".pro
                cp templates/main_core_template.cpp "apps/$1/main.cpp"
fi

