#!/bin/bash

umlet -action=convert -format=pdf -filename=layer_architecture.uxf
pdflatex requirements_core.tex
