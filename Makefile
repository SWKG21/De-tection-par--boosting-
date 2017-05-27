CXX = mpic++ 
FLAGS = -g

BASE = image.o rectangle.o
TESTVECTOR = $(BASE) testVectorProduct.o
DECRYPT = $(BASE) decrypt.o

# catch-all rule to compile object files
#   $@ refers to the rule name (.o) and %< to the first item in rule (.cpp)
%.o:    %.cpp %.h Makefile
	$(CXX) $(FLAGS) -c -o $@ $<

strategy0 : $(TESTVECTOR) strategy0.o
	$(CXX) $(FLAGS) $^ -o $@

strategy1 : $(TESTVECTOR) strategy1.o
	$(CXX) $(FLAGS) $^ -o $@

strategy2 : $(TESTVECTOR) strategy2.o
	$(CXX) $(FLAGS) $^ -o $@

strategy3 : $(TESTVECTOR) strategy3.o
	$(CXX) $(FLAGS) $^ -o $@

decrypt0 : $(DECRYPT) strategy0.o
	$(CXX) $(FLAGS) $^ -o $@

decrypt1 : $(DECRYPT) strategy1.o
	$(CXX) $(FLAGS) $^ -o $@

decrypt2 : $(DECRYPT) strategy2.o
	$(CXX) $(FLAGS) $^ -o $@

decrypt3 : $(DECRYPT) strategy3.o
	$(CXX) $(FLAGS) $^ -o $@

tmp : $(BASE) tmp.o
	$(CXX) $(FLAGS) $^ -o $@

