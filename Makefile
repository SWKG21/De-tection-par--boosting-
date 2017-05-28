MAIN = main

HFILES=$(wildcard *.h)

OBJECTS = image.o instances.o classifier.o boosting.o


CXX = mpic++ -O3

all : $(MAIN)

$(MAIN) : $(OBJECTS) $(MAIN).o
	$(CXX) $(INCLUDES) -o $@ $^ $(LIBS)

%o: %.cpp
	$(CXX) -c $< -o $@

.PHONY: clean

clean :
	rm -f $(MAIN)
	rm -f *.o *.out
