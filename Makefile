all: style
.PHONE: style docs docs-pdf

style:
	cd src && find . -name '*.cc' -o -name '*.hh' | grep -v 'json.hh'  | grep -v ecalchannels_db.cc | xargs -n 1 clang-format --style=file -i

docs:
	cd src && doxygen Doxyfile

docs-pdf: docs
	cd src/docs/latex && pdflatex refman.tex && pdflatex refman.tex && cp -v refman.pdf ../../../DQMCpp-docs.pdf