target extended-remote :4242

define re
	make
	monitor jtag_reset
	load
	monitor jtag_reset
	run
end
