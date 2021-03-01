all: style
.PHONE: style

style:
	find src -name '*.cc' -o -name '*.hh' | grep -v 'json.hh' | xargs astyle --style=kr