HELP = LifeSensor Documents

TARGETS_all += build

GRAPHVIZ += esp32_overview.svg

.PHONY: build-graphviz
TARGETS_build += build-graphviz
HELP_build-graphviz = run graphviz to generate images 
build-graphviz: $(GRAPHVIZ)

.PHONY: clean-graphviz
TARGETS_clean += clean-graphviz
HELP_clean-graphviz = remove images produced by graphviz
clean-graphviz:
	rm -f $(GRAPHVIZ)

%.svg : %.neato.gv
	neato -Tsvg $^ > $@

include $(shell git rev-parse --show-toplevel)/.make/common.mk