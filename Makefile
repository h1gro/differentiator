flags = -D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations \
-Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts \
-Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal \
-Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op \
-Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self \
-Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 \
-Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override \
-Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast \
-Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast \
-Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow \
-flto-odr-type-merging -fno-omit-frame-pointer -Wstack-usage=8192 -pie -fPIE -Werror=vla \
-fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

#-------------------------------------------------------------------------------------------------------#

all: differentiator clean

#-------------------------------------------------------------------------------------------------------#

differentiator: Main.o CreateNode.o Dump.o Utilits.o ScanFile.o ReadTree.o
	g++ Main.o CreateNode.o Dump.o Utilits.o ScanFile.o ReadTree.o $(flags) -o differentiator

Main.o: Main.cpp
	g++ -c $(flags) Main.cpp

CreateNode.o: CreateNode.cpp
	g++ -c $(flags) CreateNode.cpp

Dump.o: Dump.cpp
	g++ -c $(flags) Dump.cpp

Utilits.o: Utilits.cpp
	g++ -c $(flags) Utilits.cpp

ScanFile.o: ScanFile.cpp
	g++ -c $(flags) ScanFile.cpp

ReadTree.o: ReadTree.cpp
	g++ -c $(flags) ReadTree.cpp

#-------------------------------------------------------------------------------------------------------#

clean:
	rm -rf *.o *.exe *.exe.log *.exe.log.dmp

#-------------------------------------------------------------------------------------------------------#

run:
	./differentiator && rm -rf differentiator

#-------------------------------------------------------------------------------------------------------#

clean_graph:
	rm -r Graphs/*.png

#-------------------------------------------------------------------------------------------------------#
