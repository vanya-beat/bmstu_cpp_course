#!/bin/bash

echo "Running tasks"
pwd
cd build/tasks
#find all exec files and run them
for f in $(find . -type f -executable -name "task_*"); do
    echo "Running $f"
    $f
done

for f in $(find . -type f -executable -name "bmstu_*"); do
    echo "Running $f"
    $f
done