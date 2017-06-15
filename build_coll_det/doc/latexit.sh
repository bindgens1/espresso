#!/bin/sh 
# Copyright (C) 2012,2013,2014,2015,2016 The ESPResSo project
# Copyright (C) 2010,2011,2012 Olaf Lenz
#  
# This file is part of ESPResSo.
#  
# ESPResSo is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#  
# ESPResSo is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#  
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>. 
#
SRCDIR=$1
BASEFILENAME=$2
TEXINPUTS=.:$SRCDIR:/Users/bindgens2/Documents/espresso_dev/espresso_rudolf/espresso/doc:
BIBINPUTS=.:$SRCDIR:
export TEXINPUTS BIBINPUTS
echo "TEXINPUTS=$TEXINPUTS"

ESPRESSO_VERSION=`cd /Users/bindgens2/Documents/espresso_dev/espresso_rudolf/espresso; sh config/genversion.sh -rd`
echo "ESPRESSO_VERSION=$ESPRESSO_VERSION"

PDFLATEX="/Library/TeX/texbin/pdflatex -halt-on-error -interaction=batchmode"
LATEXLINE="\\def\\esversion{$ESPRESSO_VERSION}\\input{$BASEFILENAME.tex}"

echo "Running LaTeX stage 1..."

$PDFLATEX $LATEXLINE
EC=$?
if test $EC -ne 0; then
    echo "ERROR: LaTeX stage 1 failed."
    echo "These are the last 20 lines of $BASEFILENAME.log:"
    echo "--------------------------------------------"
    tail -n 20 $BASEFILENAME.log
    echo "--------------------------------------------"
    test -f $BASEFILENAME.pdf && rm $BASEFILENAME.pdf
    exit $EC
fi

echo "Running bibtex..."
# Don't fail on bibtex error: it fails if no .bib-file is there!
/Library/TeX/texbin/bibtex $BASEFILENAME

echo "Running LaTeX stage 2..."
$PDFLATEX $LATEXLINE 
EC=$?
if test $EC -ne 0; then
    echo "ERROR: LaTeX stage 2 failed."
    echo "These are the last 20 lines of $BASEFILENAME.log:"
    echo "--------------------------------------------"
    tail -n 20 $BASEFILENAME.log
    echo "--------------------------------------------"
    test -f $BASEFILENAME.pdf && rm $BASEFILENAME.pdf
    exit $EC
fi

if test -e $BASEFILENAME.idx; then
    echo "Running makeindex..."
    /Library/TeX/texbin/makeindex $BASEFILENAME || exit $?
fi

echo "Running LaTeX stage 3..."
$PDFLATEX $LATEXLINE
EC=$?
if test $EC -ne 0; then
    echo "ERROR: LaTeX stage 3 failed."
    echo "These are the last 20 lines of $BASEFILENAME.log:"
    echo "--------------------------------------------"
    tail -n 20 $BASEFILENAME.log
    echo "--------------------------------------------"
    test -f $BASEFILENAME.pdf && rm $BASEFILENAME.pdf
    exit $EC
fi

echo "See LaTeX output in $BASEFILENAME.log."
