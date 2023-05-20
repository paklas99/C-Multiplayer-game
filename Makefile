executable = Kalabalik
sources_directory = ./components/sources/
build_directory = ./build/
sources = $(wildcard $(sources_directory)*.c)
objects = $(patsubst $(sources_directory)%.c,$(build_directory)%.o,$(sources))
flags_mac = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_net -lSDL2_mixer -lm
flags_pc = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_net -lSDL2_mixer -Wall -lm 

mac: $(objects) $(build_directory)server.o
	gcc -o $(executable) $(objects) $(flags_mac)
	gcc -o Server $(build_directory)server.o $(build_directory)collision_detection.o $(build_directory)game.o $(build_directory)initialization.o $(build_directory)interactable_items.o $(build_directory)map.o $(build_directory)menu.o $(build_directory)network_UDP.o $(build_directory)network_TCP.o $(build_directory)object.o $(build_directory)player.o $(build_directory)points.o $(build_directory)text.o $(build_directory)textures.o $(flags_mac)

pc: $(objects) $(build_directory)server.o
	gcc -o $(executable) $(objects) $(flags_pc)
	gcc -o Server $(build_directory)server.o $(build_directory)collision_detection.o $(build_directory)game.o $(build_directory)initialization.o $(build_directory)interactable_items.o $(build_directory)map.o $(build_directory)menu.o $(build_directory)network_UDP.o $(build_directory)network_TCP.o $(build_directory)object.o $(build_directory)player.o $(build_directory)points.o $(build_directory)text.o $(build_directory)textures.o $(flags_pc)

$(build_directory)%.o:$(sources_directory)%.c
	gcc -c $< -o $@

# Server
$(build_directory)server.o: ./components/server/server.c
	gcc -c ./components/server/server.c -o $(build_directory)server.o 

.PHONY: clean_pc
.PHONY: clean_mac
clean_pc:
	powershell "(Remove-Item -ErrorAction Ignore $(build_directory)*.o)"
	powershell "(Remove-Item -ErrorAction Ignore *.exe)"
clean_mac:
	rm $(build_directory)*.o
	rm $(executable)
	rm ./Server