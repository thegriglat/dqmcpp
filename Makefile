all: style
.PHONE: style docs docs-pdf

style:
	find src -name '*.cc' -o -name '*.hh' | grep -v 'json.hh' | xargs -n 1 clang-format --style=Chromium -i

docs:
	cd src && doxygen Doxyfile

docs-pdf: docs
	cd src/docs/latex && pdflatex refman.tex && pdflatex refman.tex && cp -v refman.pdf ../../../DQMCpp-docs.pdf