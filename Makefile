JAVA_INCLUDE_DIRECTORIES=$(shell find $(shell /usr/libexec/java_home)/include -type d)

run: build
	clojure --main main

build:
	$(MAKE) -C native-library
