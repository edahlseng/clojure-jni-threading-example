JAVA_INCLUDE_DIRECTORIES=$(shell find $(shell /usr/libexec/java_home)/include -type d)

build:
	mkdir -p targets/jni

	clojure -P
	javac --class-path ~/.m2/repository/com/github/ramanrajarathinam/native-utils/1.0.0/native-utils-1.0.0.jar -d targets/ sources/*.java -h targets/

	clang++ -std=c++17 -stdlib=libc++ $(JAVA_INCLUDE_DIRECTORIES:%=-I%) -I targets -shared sources/test.cpp -o targets/Test.so

	jar cf targets/jni/Test.jar -C targets Test.class -C targets TestFunction.class -C targets Test.so

clean:
	rm -rf targets
