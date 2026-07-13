#include <SDL2/SDL.h>

#ifdef __EMSCRIPTEN__
#include <GLES3/gl3.h> // Provides WebGL/GLES headers
#include <emscripten.h>
#else
#include <glad/gl.h>
#endif

#include <stdio.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

SDL_Window* window = NULL;
SDL_GLContext glContext = NULL;
int running = 1;

// OpenGL Buffer handles
unsigned int VAO, VBO;
unsigned int shaderProgram;

// Vertex Shader Source (OpenGL ES 3.0 / WebGL 2)
const char* vertexShaderSource = 
    #ifdef __EMSCRIPTEN__
    "#version 300 es\n"
    #else
    "#version 330 core\n"
    #endif
    "layout (location = 0) in vec2 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "out vec3 vertexColor;\n"
    "void main() {\n"
    "   gl_Position = vec4(aPos, 0.0, 1.0);\n"
    "   vertexColor = aColor;\n"
    "}\0";

// Fragment Shader Source
const char* fragmentShaderSource =
    #ifdef __EMSCRIPTEN__
    "#version 300 es\n"
    "precision mediump float;\n"
    #else
    "#version 330 core\n"
    #endif
    "in vec3 vertexColor;\n"
    "out vec4 FragColor;\n"
    "void main() {\n"
    "   FragColor = vec4(vertexColor, 1.0);\n"
    "}\0";

void init_shaders() {
    // Compile Vertex Shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Compile Fragment Shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Link Shaders into Program
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Delete intermediate shader stages
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void init_geometry() {
    // Interleaved data: Position (X, Y) and Color (R, G, B)
    // Values normalized between -1.0 and 1.0 for clip-space coordinates
    float vertices[] = {
         0.0f,  0.5f,  1.0f, 0.0f, 0.0f,  // Top: Red
         0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  // Bottom Right: Green
        -0.5f, -0.5f,  0.0f, 0.0f, 1.0f   // Bottom Left: Blue
    };

    // Generate and bind VAO & VBO arrays
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Attribute 0: Position data (2 floats)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Attribute 1: Color data (3 floats)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 
}

void cleanup() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    if (glContext) SDL_GL_DeleteContext(glContext);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}

void main_loop() {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            #ifdef __EMSCRIPTEN__
            emscripten_cancel_main_loop();
            #else
            running = 0;
            #endif
            cleanup();
            return;
        }
    }

    // Set Clear state (Dark Grey)
    glClearColor(0.12f, 0.12f, 0.12f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Activate WebGL Shader program
    glUseProgram(shaderProgram);
    
    // Draw the explicit elements
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // Swap backbuffer into view display
    SDL_GL_SwapWindow(window);
}

int main(int argc, char** argv) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return 1;

    #ifdef __EMSCRIPTEN__
    // Force SDL to request WebGL 2.0 context profiles
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    #else
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    #endif

    window = SDL_CreateWindow("SDL2 WebGL Triangle", 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    glContext = SDL_GL_CreateContext(window);
    if (!glContext) {

        return 1;
    }
    #ifndef __EMSCRIPTEN__
    if (!gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress)) {
        printf("Failed to load GLAD.");
    }
    #endif

    // Compile pipelines and populate buffers
    init_shaders();
    init_geometry();

    #ifdef __EMSCRIPTEN__
    // Start browser context execution loop
    emscripten_set_main_loop(main_loop, 0, 1);
    #else
    while (running) {
        main_loop();
    }
    #endif
    printf("Ended successfully\n");
    return 0;
}
