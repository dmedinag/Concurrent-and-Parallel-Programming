KIND:=STACK
TYPE:=MC

CXX =u++# compiler
CXXFLAGS = -g -std=c++1y -multi -nodebug -Wall -Wno-unused-label -MMD -O2 -DIMPLKIND_${KIND} -DIMPLTYPE_${TYPE}
MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}}	# makefile name

OBJECTS01 = q1.o # optional build of given program
EXEC01 = new

OBJECTS2 = q2tallyVotes${TYPE}.o # list of object files for question 1 prefixed with "q2"

EXEC2 = vote

OBJECTS = ${OBJECTS2} # all object files
DEPENDS = ${OBJECTS:.o=.d} # substitute ".o" with ".d"
EXECS = ${EXEC2} # all executables

#############################################################

.PHONY : all clean

all : ${EXECS}					# build all executables

-include ImplKind

q1new.o : q1new.cc				# add flags 1st executable, ADJUST SUFFIX (.cc)
	${CXX} ${CXXFLAGS} -nodebug -c $< -o $@

ifeq (${IMPLKIND},${KIND})			# same implementation type as last time ?
${EXEC01} : ${OBJECTS01}
	${CXX} ${CXXFLAGS} -nodebug $^ -o $@
else
ifeq (${KIND},)					# no implementation type specified ?
# set type to previous type
KIND=${IMPLKIND}
${EXEC01} : ${OBJECTS01}
	${CXX} ${CXXFLAGS} -nodebug $^ -o $@
else						# implementation type has changed
.PHONY : ${EXEC01}
${EXEC01} :
	rm -f ImplKind
	touch q1new.cc
	sleep 1
	${MAKE} ${EXEC01} KIND="${KIND}"
endif
endif

ImplKind :
	echo "IMPLKIND=${KIND}" > ImplKind
	sleep 1

#############################################################

-include ImplType

ifeq (${IMPLTYPE},${TYPE})			# same implementation type as last time ?
${EXEC2} : ${OBJECTS2}
	${CXX} ${CXXFLAGS} $^ -o $@
else
ifeq (${TYPE},)					# no implementation type specified ?
# set type to previous type
TYPE=${IMPLTYPE}
${EXEC2} : ${OBJECTS2}
	${CXX} ${CXXFLAGS} $^ -o $@
else						# implementation type has changed
.PHONY : ${EXEC2}
${EXEC2} :
	rm -f ImplType
	touch q2tallyVotes.h
	sleep 1
	${MAKE} ${EXEC2} TYPE="${TYPE}"
endif
endif

ImplType :
	echo "IMPLTYPE=${TYPE}" > ImplType
	sleep 1

#############################################################

${OBJECTS} : ${MAKEFILE_NAME}			# OPTIONAL : changes to this file => recompile

-include ${DEPENDS}				# include *.d files containing program dependences

clean :						# remove files that can be regenerated
	rm -f *.d *.o ${EXEC01} ${EXECS} ImplKind ImplType
