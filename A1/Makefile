CXX = u++ # compiler
CXXFLAGS = -g -Wall -Wno-unused-label -MMD -std=c++11 # compiler flags
MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}}# makefile name

OBJECTS01 = q1throwcatch.o		# optional build of given program
EXEC01 = throwcatch			# 0th executable name

OBJECTS1 = q1longjmp.o# object files forming 1st executable with prefix "q1"
EXEC1 = longjmp				# 1st executable name

OBJECTS02 = q2.o # optional build of given program
EXEC02 = resumption

OBJECTS2 = q2noresumption.o # object files forming 2nd executable with prefix "q2"
EXEC2 = noresumption # 2nd executable name

OBJECTS3 = q3.o # object files forming 3rd executable with prefix "q3"
EXEC3 = floatconstant			# 3rd executable name

OBJECTS = ${OBJECTS1} ${OBJECTS2} ${OBJECTS3}
DEPENDS = ${OBJECTS:.o=.d}
EXECS = ${EXEC1} ${EXEC2} ${EXEC3}

#############################################################

.PHONY : all clean

all : ${EXECS}				# build all executables

q1.o : q1.cpp				# change compiler 1st executable, ADJUST SUFFIX (.cc)
	g++-4.9 ${CXXFLAGS} -c $< -o $@

q1%.o : q1%.cpp				# change compiler 1st executable, ADJUST SUFFIX (.cc)
	g++-4.9 ${CXXFLAGS} -c $< -o $@

${EXEC01} : ${OBJECTS01}
	g++-4.9 ${CXXFLAGS} $^ -o $@

${EXEC1} : ${OBJECTS1}
	g++-4.9 ${CXXFLAGS} $^ -o $@

${OBJECTS02} : q2.cpp
	${CXX} ${CXXFLAGS} -c $< -o $@

${EXEC02} : ${OBJECTS02}
	${CXX} ${CXXFLAGS} $^ -o $@

q2.o : q2.cpp				# change compiler 2nd executable, ADJUST SUFFIX (.cc)
	g++-4.9 ${CXXFLAGS} -c $< -o $@

q2%.o : q2%.cpp				# change compiler 2nd executable, ADJUST SUFFIX (.cc)
	g++-4.9 ${CXXFLAGS} -c $< -o $@

q3.o : q3.cpp
	${CXX} ${CXXFLAGS} -c $< -o $@

${EXEC2} : ${OBJECTS2}
	g++-4.9 ${CXXFLAGS} $^ -o $@

${EXEC3} : ${OBJECTS3}
	${CXX} ${CXXFLAGS} $^ -o $@

#############################################################

${OBJECTS} : ${MAKEFILE_NAME}		# OPTIONAL : changes to this file => recompile

-include ${DEPENDS}			# include *.d files containing program dependences

clean :					# remove files that can be regenerated
	rm -f *.d *.o ${EXEC01} ${EXEC02} ${EXECS}
