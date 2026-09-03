win:
	if not exist build mkdir build
	xcopy "asset" "build\asset" /E /Y /I
	gcc src/*.c -o build/main.exe -Iinclude -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

emcc-win:
	if not exist build mkdir build
	emcc $(wildcard src/*.c) -o build/index.html -Iinclude -lidbfs.js \
	-sUSE_SDL=2 -sUSE_SDL_TTF=2 -sUSE_SDL_IMAGE=2 -sSDL2_IMAGE_FORMATS='["png"]' -sMAX_WEBGL_VERSION=2 \
	-sEXPORTED_FUNCTIONS='["_main"]' --preload-file "asset@asset"

emcc:
	mkdir -p build
	emcc src/*.c -o build/index.html -Iinclude -lidbfs.js \
	-sUSE_SDL=2 -sUSE_SDL_TTF=2 -sUSE_SDL_IMAGE=2 -sSDL2_IMAGE_FORMATS='["png"]' -sMAX_WEBGL_VERSION=2 \
	-sEXPORTED_FUNCTIONS='["_main"]' --preload-file "asset@asset"

emcc-prod:
	mkdir -p build
	emcc src/*.c -o build/index.html -O2 -Iinclude -lidbfs.js \
	-sUSE_SDL=2 -sUSE_SDL_TTF=2 -sUSE_SDL_IMAGE=2 -sSDL2_IMAGE_FORMATS='["png"]' -sMAX_WEBGL_VERSION=2 \
	-sEXPORTED_FUNCTIONS='["_main"]' --preload-file "asset@asset"
