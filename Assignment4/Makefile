# cpp
CC = cc
C_FLAGS = -std=c99
SRC = src
INCLUDE = -I/include
EXE = assignment4

# ----------------------------------
# Rules
# ----------------------------------
.FAILED:
	@echo "********************************************"
	@echo "       Errors encountered (!)(!)            "
	@echo "********************************************"

.DONE:
	@echo ""
	@echo "        Make finished"
	@echo ""

# all: ${EXE}
#
# ${EXE}: main.o linkedList.o parser.o
# 	${CC} ${C_FLAGS} -o ${EXE} main.o linkedList.o parser.o
#
# main.o: main.c
# 	${CC} ${C_FLAGS} -o main.o main.c ${INCLUDES}
#
# linkedList.o: linkedList.c
# 	${CC} ${C_FLAGS} -o linkedList.o linkedList.c ${INCLUDES}
#
# parser.o: parser.c
# 	${CC} ${C_FLAGS} -o parser.o parser.c ${INCLUDES}

all: ${EXE}

${EXE}: ${SRC}/main.c # extra source files
	${CC} ${C_FLAGS} -o ${EXE} ${SRC}/main.c ${INCLUDE} #extra source files before includes

clean:
	-rm ${EXE}
