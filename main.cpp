#include <functional>
#include <iostream>
#include <vector>
using namespace std;

#include <emscripten.h>
#include <SDL.h>

std::function<void()> loop;
void main_loop() { loop(); }


int main()
{
    SDL_Window *window;
    SDL_Renderer* renderer;
    SDL_CreateWindowAndRenderer(640, 480, 0, &window, &renderer);

    SDL_Texture* texture = SDL_CreateTexture
        (
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        640, 480
        );

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    vector< unsigned char > pixels( 640 * 480 * 4, 0 );

    loop = [&]
    {
        const Uint64 start = SDL_GetPerformanceCounter();

        // move a vertex
        const uint32_t milliseconds_since_start = SDL_GetTicks();
        const uint32_t milliseconds_per_loop = 3000;


        SDL_SetRenderDrawColor( renderer, 0, 0, 0, SDL_ALPHA_OPAQUE );
        SDL_RenderClear( renderer );

        for( unsigned int i = 0; i < 1000; i++ )
        {
            const unsigned int x = rand() % 640;
            const unsigned int y = rand() % 480;

            const unsigned int offset = ( 640 * 4 * y ) + x * 4;
            pixels[ offset + 0 ] = rand() % 256;        // b
            pixels[ offset + 1 ] = rand() % 256;        // g
            pixels[ offset + 2 ] = rand() % 256;        // r
            pixels[ offset + 3 ] = SDL_ALPHA_OPAQUE;    // a
        }

        SDL_UpdateTexture
            (
            texture,
            NULL,
            &pixels[0],
            640 * 4
            );

        SDL_RenderCopy( renderer, texture, NULL, NULL );
        SDL_RenderPresent( renderer );


        const Uint64 end = SDL_GetPerformanceCounter();
        const static Uint64 freq = SDL_GetPerformanceFrequency();
        const double seconds = ( end - start ) / static_cast< double >( freq );
        cout << "Start: " << start
             << " end: "  << end
             << " Frame time: " << seconds * 1000.0 << "ms" << endl;
        
        // Draw a triangle from the 3 vertices
        // glDrawArrays(GL_TRIANGLES, 0, 3);

        SDL_GL_SwapWindow(window);
    };

    emscripten_set_main_loop(main_loop, 0, true);

    return EXIT_SUCCESS;
}
