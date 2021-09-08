all: style
.PHONE: style docs docs-pdf mkdocs

style:
	cd src && find . -name '*.cc' -o -name '*.hh' | grep -v 'json.hh'  | grep -v ecalchannels_db.cc | xargs -n 1 clang-format --style=file -i

docs:
	rm -rf doxygen
	cd src && doxygen Doxyfile

docs-pdf: docs
	cd doxygen/latex && pdflatex refman.tex && pdflatex refman.tex && cp -v refman.pdf ../../DQMCpp-docs.pdf

mkdocs: docs
	rm -rf mkdocs/docs/api
	rsync -avr --delete doxygen/html/ mkdocs/docs/api/

pages-deploy: mkdocs
	cd mkdocs; mkdocs gh-deploy