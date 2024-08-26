# Use bash shell with pipefail option enabled so that the return status of a
# piped command is the value of the last (rightmost) command to exit with a
# non-zero status. This lets us pipe output into tee but still exit on test
# failures.
SHELL = /bin/bash
.SHELLFLAGS = -o pipefail -c

all: test

test:
	cd /app && mkdir -p /build/tests/output && ln -s /build/tests/output tests/output && \
	python setup.py test | tee setup_py_test.stdout
	mkdir -p /build/reports && \
	if [ -d /build ]; then \
		mkdir -p /build/output; \
		cp /app/tests/output/*.vhd /build/output; \
        mv /app/setup_py_test.stdout /build; \
        mv /app/htmlcov /build/htmlcov; \
        cp /app/coverage.xml /build/coverage.xml; \
        mv /app/coverage.xml /build/reports/code-coverage.xml; \
    fi;

lint:
	pip3 install pylint2junit; \
	mkdir -p /build/reports; \
	echo "AB debug #1"; \
	cd /app && pylint ${PYLINT_OPTS} --output-format=parseable src/ska | tee /build/code_analysis.stdout; \
	echo "AB debug #2"; \
	cd /app && pylint ${PYLINT_OPTS} --output-format=pylint2junit.JunitReporter src/ska > /build/reports/linting.xml; \
	echo "AB debug #3"; 

convert:
	mkdir -p /build/vhdl; \
	translate --namespace ska --package CbfPsrHeader_pkg /app/src/ska/cbf_psr_interface/*.h /build/vhdl/cbfpsrheader_pkg.vhd;

.PHONY: all test
