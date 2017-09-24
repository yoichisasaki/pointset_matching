#!/bin/bash
#usage: makeCA.sh pat1.txt pat2.txt ...
# => pat1.out pat2.out ...

grep -v "^REMARK"  |  gawk ' ($3 == "CA") { print $0; }' 

### EOF


