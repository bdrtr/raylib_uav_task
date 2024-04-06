

program_adi := $(name)
sonuc_adi = $(basename $(program_adi))

$(sonuc_adi): ${program_adi}
	cc -o $(sonuc_adi) $(program_adi) -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
