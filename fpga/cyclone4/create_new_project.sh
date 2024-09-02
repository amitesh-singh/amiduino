#!/bin/bash

set -v
PROJECT="$1"
if [ -z "$PROJECT" ]; then
    echo "no argument provided"
    exit
fi
cp ./template -Rfv "$PROJECT"

mv $PROJECT/template.v $PROJECT/$PROJECT.v
mv $PROJECT/template.qsf $PROJECT/$PROJECT.qsf
mv $PROJECT/template.qpf $PROJECT/$PROJECT.qpf
sed -i "s/template/"$PROJECT"/g" "$PROJECT"/*

