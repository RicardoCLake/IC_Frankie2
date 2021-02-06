function value = ConvFloat(msb, lsb)
	signal = floor(msb/ 32768);
	expoent = floor((msb-signal*32768)/128) - 127;
	mantissa = (mod(msb, 128)*65536 + lsb) / (128*65536) + 1;
	value = mantissa*2^expoent;
	if(signal)
		value = value * -1;		
	endif
endfunction