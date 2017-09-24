CC = g++
#CFLAGS = -Wextra -g -O3 -std=c++11
#CFLAGS = -Wextra -O3 -std=c++11
CFLAGS =  -O3 -std=c++11


SRC = main.cpp rmsd_naive.cpp rmsd_pruned.cpp pruned_match_di.cpp pruned_match_FPT.cpp pruned_match_FPT2.cpp
OBJ = main.o rmsd_naive.o rmsd_pruned.o pruned_match_di.o pruned_match_FPT.o pruned_match_FPT2.cpp
####

main.o: main.cpp
	g++ $(CFLAGS) -c main.cpp 

rmsd.o: rmsd.cpp rmsd.hpp 
	g++ $(CFLAGS) -c rmsd.cpp 

#### rmsd1
naive_match.exe:  main.o rmsd.o rmsd_naive.cpp 
	g++ $(CFLAGS) -o naive_match.exe main.o rmsd.o rmsd_naive.cpp 

### 

pruned_match.exe:  main.o rmsd.o rmsd_pruned.cpp
	g++ $(CFLAGS) -o pruned_match.exe  main.o rmsd.o rmsd_pruned.cpp 

pruned_match_di.exe:  main.o rmsd.o rmsd_pruned_di.cpp
	g++ $(CFLAGS) -o pruned_match_di.exe  main.o rmsd.o rmsd_pruned_di.cpp 
###追加(FPT-like)

pruned_match_FPT.exe:  main.o rmsd.o rmsd_space.cpp
	g++ $(CFLAGS) -o pruned_match_FPT.exe  main.o rmsd.o rmsd_space.cpp 

###追加(OPT)

pruned_opt.exe:  main.o rmsd.o rmsd_opt.cpp
	g++ $(CFLAGS) -o pruned_opt.exe  main.o rmsd.o rmsd_opt.cpp

### 

datadist.exe:  datadist.o rmsd.o 
	g++ $(CFLAGS) -o datadist.exe  datadist.o rmsd.o 

datadist2.exe:  datadist2.o rmsd.o 
	g++ $(CFLAGS) -o datadist2.exe  datadist2.o rmsd.o 

randlines.exe:  randlines.cpp rmsd.o 
	g++ $(CFLAGS) -o randlines.exe  randlines.cpp rmsd.o 

###

# pmatch.exe:  main.cpp rmsd_pruned.cpp 
# 	g++ -o pmatch.exe main.cpp rmsd_pruned.cpp -O3

####
####
TOOLS=datadist2.exe  randlines.exe
TARGET = $(TOOLS) naive_match.exe pruned_match.exe pruned_match_di.exe pruned_match_FPT.exe
#DATFILE=data/testpdb1.txt 
DATFILE=_PDB.txt
PATFILE=_CA.txt
DATFILE0=_PDB0.txt
PATFILE0=_CA0.txt
CA=testdir/1jsm_CA.txt
PDB=testdir/1jsn_CA.txt

# CA=testdir/3khw_CA.txt
# PDB=testdir/3lkp_CA.txt
R=DBL_MAX

all: $(TARGET)

#exp: exp1 exp2 
P=10
T=100
#max T=3722

exp: exp3 exp3


exp4: ./pruned_match_FPT.exe
	#"\n####### FPT-like ####### "
	head -$(T) < $(PDB) > $(DATFILE)
	head -$(P) < $(CA) > $(PATFILE)
#	wc $(DATFILE)
#	wc $(PATFILE)
	./pruned_match_FPT.exe $(DATFILE) $(PATFILE) $(R)

exp3: 
	exp1run.sh -r 0.1 -n 100 3 | tee expR0.1N100K3.txt; grep "running time" expR0.1N100K3.txt;

expnaive: ./naive_match.exe 
#	#"\n####### Naive ####### "
	head -$(T) < $(PDB) > $(DATFILE)
	head -$(P) < $(CA) > $(PATFILE)
#	wc $(DATFILE)
#	wc $(PATFILE)
	./naive_match.exe $(DATFILE) $(PATFILE) $(R)

exppruned: ./pruned_match.exe
	#"\n####### Pruned ####### "
	head -$(T) < $(PDB) > $(DATFILE)
	head -$(P) < $(CA) > $(PATFILE)
#	wc $(DATFILE)
#	wc $(PATFILE)
	./pruned_match.exe $(DATFILE) $(PATFILE) $(R)

expopt: ./pruned_opt.exe
	#"\n####### Opt ####### "
	head -$(T) < $(PDB) > $(DATFILE)
	head -$(P) < $(CA) > $(PATFILE)
#	wc $(DATFILE)
#	wc $(PATFILE)
	./pruned_opt.exe $(DATFILE) $(PATFILE) $(R)

dist: ./datadist.exe
	#"\n####### Pruned ####### "
	head -$(T) < $(PDB) > $(DATFILE)
	wc $(DATFILE)
	./datadist.exe $(DATFILE)

dist2: ./datadist.exe
	datadist2.exe $(PDB) | sort -nk 2 | gawk -f cumcount.awk   



# dist: ./datadist.exe
# 	#"\n####### Pruned ####### "
# 	head -$(T) < $(PDB) > $(DATFILE)
# 	wc $(DATFILE)
# 	./datadist.exe $(DATFILE) 

clean:
	rm -f  *.o *~ #*

cleanall:
	rm -f *.o *.exe 

### EOF
