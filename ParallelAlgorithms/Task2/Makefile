.PHONY: test clean

build:
	javac skel/Tema2.java skel/Document.java skel/Map.java skel/Reduce.java skel/Fragment.java skel/DocumentList.java skel/Rank.java skel/Fibonacci.java

test:
	java -cp skel Tema2 1 tests/in/test1.txt out

clean:
	rm skel/*.class
