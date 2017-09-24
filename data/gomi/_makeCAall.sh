#!/bin/bash
#usage: makeCA.sh pat1.txt pat2.txt ...
# => pat1.out pat2.out ...

TMPFILE=pat.tmp 
OUTFILE=pat.all

for name in $* 
do 
    body=${name%.txt}
    outfile="$body.out"
    echo "processing $name => $body => $outfile ..."
    grep -v "^REMARK" $name  > ${TMPFILE}
    cat ${TMPFILE}  | gawk ' ($3 == "CA") { print $0; }' > $outfile 
    \rm ${TMPFILE} 
done 
