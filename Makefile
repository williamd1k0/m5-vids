
flags: compile_flags.txt
compile_flags.txt: compile_flags.txt.in
	@sed "s|{HOME}|${HOME}|g" $< | tee $@
