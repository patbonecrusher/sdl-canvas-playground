
# Instructions

-------------

1. Install Emscripten:

    http://emscripten.org

2. Clone this repo:

    ```git clone https://github.com/timhutton/opengl-canvas-wasm.git```

    ```cd sdl-canvas-playground```

3. Build index.js and index.wasm:

    ```emcc main.cpp -std=c++11 -s WASM=1 -s USE_SDL=2 -O3 -o index.js```

4. Open index.html in a web browser. You should see random static pixel.