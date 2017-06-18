all :
	cd lib; \
	make; \
	cd ../src; \
	make

debug:
	cd lib; \
	make; \
	cd ../src; \
	make debug

clean :
	cd lib; \
	make clean; \
	cd ../src; \
	make clean

