/* glhexen2 Menu by Slayer366 (Andrew A. Kratz) */
#include "glh2menu.h"

// Function to handle SDL2 events
int handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_QUIT) {
            // Handle quit event (e.g., user closes the window)
            return 1; // Signal to exit the loop or program
        } else if (event.type == SDL_KEYDOWN) {
            // Handle keydown events
            switch (event.key.keysym.sym) {
                case SDLK_UP:
                    return SDLK_UP;
                case SDLK_DOWN:
                    return SDLK_DOWN;
                case SDLK_PAGEUP:
                    return SDLK_PAGEUP;
                case SDLK_PAGEDOWN:
                    return SDLK_PAGEDOWN;
                case SDLK_RETURN:
                    return SDLK_RETURN;
                case SDLK_ESCAPE:
                    return SDLK_ESCAPE;
            }
        } else if (event.type == SDL_CONTROLLERBUTTONDOWN) {
            // Handle controller events
            switch (event.cbutton.button) {
              //  case SDL_CONTROLLER_BUTTON_DPAD_UP:
              //      return SDLK_UP;
              //  case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
              //      return SDLK_DOWN;
                case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
                    return SDLK_PAGEUP;
                case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
                    return SDLK_PAGEDOWN;
                case SDL_CONTROLLER_BUTTON_A:
                case SDL_CONTROLLER_BUTTON_B:
                    return SDLK_RETURN;
                case SDL_CONTROLLER_BUTTON_BACK:
                    return SDLK_ESCAPE;
            }
    
        }
    }
    return 0;
}

// Function to display a menu with existing player classes
void displayClassMenu() {
    for (int i = 0; i < CHAR_CLASS; i++) {
        char buffer[100];
        snprintf(buffer, sizeof(buffer), "%s", charClass[i]);
    }
}

// Function to display a menu with existing skill levels
void displaySkillMenu() {
    for (int i = 0; i < SKILL_LEVEL; i++) {
        char buffer[100];
        snprintf(buffer, sizeof(buffer), "%s", skillLabels[i]);
    }
}

void renderText(SDL_Renderer *renderer, TTF_Font *font, const char *text, int x, int y, int r, int g, int b) {
    SDL_Color textColor = {r, g, b, 255};
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, text, textColor);

    if (textSurface) {
        textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_FreeSurface(textSurface);

        SDL_Rect dstRect = {x, y, textSurface->w, textSurface->h};
        SDL_RenderCopy(renderer, textTexture, NULL, &dstRect);
        SDL_DestroyTexture(textTexture);
    }
}

// Function to display a menu with existing files
void displayMenu(SDL_Renderer *renderer, const char **options, int numOptions, int selected, const char *title, int start) {
    // Clear the screen
    SDL_SetRenderDrawColor(renderer, 48, 8, 0, 255); // Dark Red
    SDL_RenderClear(renderer);

    // Render title
    renderText(renderer, font, title, 10, 10, 224, 210, 112); // Yellow

    int displayStart = (selected >= DISPLAY_HEIGHT) ? selected - DISPLAY_HEIGHT + 1 : 0;
    int displayEnd = displayStart + DISPLAY_HEIGHT;

    // Adjust the display end index to prevent going beyond the array bounds
    displayEnd = (displayEnd > numOptions) ? numOptions : displayEnd;

    // Render options
        for (int i = displayStart; i < displayEnd; i++) {
            if (i == selected) {
            // Highlight the selected option
            SDL_SetRenderDrawColor(renderer, 0, 0, 128, 255); // Dark Blue
            SDL_Rect rect = {10, 31 + (i - displayStart) * 20, screenw - 40, 20};
            SDL_RenderFillRect(renderer, &rect);
        }
        renderText(renderer, font, options[i], 20, 30 + (i - displayStart) * 20, 224, 224, 224); // Dim White
    }
    SDL_RenderPresent(renderer);
}

// Function to compare strings for qsort
// strcmp -> strcasecmp courtesy of @kloptops
int compareStrings(const void *a, const void *b) {
    return strcasecmp(*(const char **)a, *(const char **)b);
}

// Function to run the command
/*
 Originally it would allow for this application to exit immediately after launching child process,
 but this wasn't great because emulationstation would detect that this process had ended and then
 relaunched leaving the game running in the background while emulationstation was brought into focus.
 The idea was for the application to quit so a shell script would end gptokeyb so it wouldn't
 interfere with the controls in the game (which it absolutely will).  So we have instead decided to
 kill gptokeyb within this program directly which seems to do the job nicely.
*/
void runCommand(const char *command) {

    //system("pkill -f gptokeyb");
    //system("sudo kill -9 $(pidof gptokeyb)");
    //system("kill -9 $(pidof gptokeyb)");

    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Child process
        if (execlp("sh", "sh", "-c", command, NULL) == -1) {
            perror("execlp");
            SDL_Quit();
            exit(EXIT_FAILURE);
        }
    } else {
        // Parent process
        //SDL_Quit();
        //exit(0);
        int status;
        if (waitpid(pid, &status, 0) == -1) {
            perror("waitpid");
        }
    }
}

// The main program loop
int main() {

    const char *deviceArch = getenv("DEVICE_ARCH");
    if (!deviceArch) {
        fprintf(stderr, "Error: DEVICE_ARCH environment variable not set.\nExample:\nexport DEVICE_ARCH=aarch64 \n");
        return 1;
    }

    const char *displayWidth = getenv("DISPLAY_WIDTH");
    if (!displayWidth) {
        fprintf(stderr, "Error: DISPLAY_WIDTH environment variable not set.\nExample:\nexport DISPLAY_WIDTH=640 \n");
        return 1;
    }

    const char *displayHeight = getenv("DISPLAY_HEIGHT");
    if (!displayHeight) {
        fprintf(stderr, "Error: DISPLAY_HEIGHT environment variable not set.\nExample:\nexport DISPLAY_HEIGHT=480 \n");
        return 1;
    }

    const char *addlParams = getenv("ADDLPARAMS");
    if (!addlParams) {
        fprintf(stderr, "Error: ADDLPARAMS environment variable not set.\nExample:\nexport ADDLPARAMS=\"+showfps 0 +r_waterwarp 0\" \n");
        return 1;
    }

    const char *runMod = getenv("RUNMOD");
    if (!runMod) {
        fprintf(stderr, "Error: RUNMOD environment variable not set.\nExamples:\nexport RUNMOD=\"-game mpbyrino +map rinoxxx\" \nexport RUNMOD=\"-game fo4d +map hexn1\" \n");
        return 1;
    }


    // Initialize SDL2
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0) {
        fprintf(stderr, "SDL2 initialization failed: %s\n", SDL_GetError());
        return 1;
    }

    // Initialize SDL2_ttf for text rendering
    if (TTF_Init() < 0) {
        fprintf(stderr, "SDL2_ttf initialization failed: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    // Create a window and renderer
    SDL_Window *window = SDL_CreateWindow("Hexen 2 Selection Menu", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenw, screenh, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderSetLogicalSize(renderer, screenw, screenh);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    // Load font
    font = TTF_OpenFont("DooM.ttf", 15);
    if (!font) {
        fprintf(stderr, "Error loading font: %s\n", TTF_GetError());
        return 1;
    }

    // Open the first available game controller
    SDL_GameController *controller = NULL;
    for (int i = 0; i < SDL_NumJoysticks(); ++i) {
        if (SDL_IsGameController(i)) {
            controller = SDL_GameControllerOpen(i);
            if (controller) {
                printf("Game controller connected: %s\n", SDL_GameControllerName(controller));
                break;
            }
        }
    }

    if (!controller) {
        printf("Warning: No game controller found.\n");
    }

int playerclassSelected = 0;
int skillSelected = 0;

/* Improved event handling, removed redundancies, added boolean operator - courtesy of @kloptops */

bool in_loop = true;

// Player Class selection loop
displayClassMenu();

int numclass = CHAR_CLASS;

in_loop = true;
while(in_loop) { 
    int key;

    while (1) {
        key = handleEvents();
        if (key == 0) {
            // No more events
            break;
        }

        if (key == SDLK_RETURN) {
            in_loop = false;
            break; // Exit loop on Enter key
        }

        switch (key) {
            case SDLK_UP:
                    VAR_DEC(playerclassSelected, 1, CHAR_CLASS);
                break;
            case SDLK_DOWN:
                    VAR_INC(playerclassSelected, 1, CHAR_CLASS);
                break;
            case SDLK_ESCAPE:
                SDL_Quit();
                exit(0);
                break;
        }
    }
    displayMenu(renderer, charClass, numclass, playerclassSelected, "Choose your player class:", 0);
    SDL_Delay(35); // Moved displayMenu function call and added SDL_Delay to reduce unnecessary CPU usage - courtesy of @kloptops
}


// Skill/Difficulty selection loop
displaySkillMenu();

int numSkills = SKILL_LEVEL;

in_loop = true;
while(in_loop) {
    int key;

    while (1) {
        key = handleEvents();
        if (key == 0) {
            // No more events
            break;
        }

        if (key == SDLK_RETURN) {
            in_loop = false;
            break; // Exit loop on Enter key
        }

        switch (key) {
            case SDLK_UP:
                    VAR_DEC(skillSelected, 1, SKILL_LEVEL);
                break;
            case SDLK_DOWN:
                    VAR_INC(skillSelected, 1, SKILL_LEVEL);
                break;
            case SDLK_ESCAPE:
                SDL_Quit();
                exit(0);
                break;
        }
    }
    displayMenu(renderer, skillLabels, numSkills, skillSelected, "Select difficulty:", 0);
    SDL_Delay(35); // Moved displayMenu function call and added SDL_Delay to reduce unnecessary CPU usage - courtesy of @kloptops
}

playerclassSelected++;  // Adjust playerclassSelected value to range from 1 to 5

// Display the command based on the selected source port
// strstr -> strcasestr courtesy of @kloptops
if (playerclassSelected < numclass + 1) {

        // Run the source port
            printf("./glhexen2.%s -width %s -height %s %s -basedir ./ +playerclass %d +skill %d %s \n", deviceArch, displayWidth, displayHeight, addlParams, playerclassSelected, skillSelected, runMod);

    printf("\n");
}

// Run the command
if (playerclassSelected < numclass + 1) {
    char command[4096];     // Adjust the size based on your needs

          // Run the source port
            snprintf(command, sizeof(command), "./glhexen2.%s -width %s -height %s %s -basedir ./ +playerclass %d +skill %d %s ", deviceArch, displayWidth, displayHeight, addlParams, playerclassSelected, skillSelected, runMod);

    // Run the command
        runCommand(command);

    // Cleanup and exit the program
        TTF_CloseFont(font);
        TTF_Quit();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(0);
}


return 0;
}
