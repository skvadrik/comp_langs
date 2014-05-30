#!/bin/sh -e

pdflatex -shell-escape all.tex </dev/null > all.build_log
okular all.pdf
