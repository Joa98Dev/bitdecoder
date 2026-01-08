#include <GL/gl.h>
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include "../includes/TranslatorLogic.h"
#include <cstdio>
#include <cstring>
#include <string>

// -------------------- Buffers --------------------
// Static character array to store the text entered by the user
// ImGui requires these raw char arrays rather than std::string for input fields
static char textBuffer[4096] = "";
static char binaryBuffer[8192] = "";

// -------------------- Helpers --------------------
// Fills the buffer with null terminators to erase the content
void ClearBuffer(char* buffer, size_t size) {
    memset(buffer, 0, size);
}

// Uses ImGui's built-in utility to put text into the OS clipboard
void CopyToClipboard(const char* text) {
    ImGui::SetClipboardText(text);
}

// Safely copies a std::string into a fixed size char array
void SafeCopy(char* dst, size_t dstSize, const std::string& src) {
    if (dstSize == 0) return;
    strncpy(dst, src.c_str(), dstSize - 1);
    dst[dstSize - 1] = '\0';
}

// -------------------- Main --------------------
int main() {

    // Initialize GLFW library
    if (!glfwInit())
        return -1;

    // Window configuration
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // Disable resizing
    GLFWwindow* window = glfwCreateWindow(800, 600, "BitDecoder", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable VSync

    // Setup ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Apply the dark visual theme
    ImGui::StyleColorsDark();

    // Initialize ImGui backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    // -------------------- Loop --------------------
    while (!glfwWindowShouldClose(window)) {
	
	// Process OS events (clicks, key presses)
        glfwPollEvents();

	// Start a new ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Make the ImGui window cover the entire GLFW window area
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

	// Define window flags to make it look like a static app interface
        ImGui::Begin("BitDecoder",
            nullptr,
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoTitleBar
        );

        // -------------------- TEXT LAYOUT --------------------

	ImGui::Text("Text:");

	// Multiline input for standard text
        ImGui::InputTextMultiline("##text", textBuffer, sizeof(textBuffer), ImVec2(-1, 170));

        if (ImGui::Button("Text to Binary")) {
	    
            // Call the logic function store result in the text buffer
            std::string bin = TextToBinary(textBuffer);
            SafeCopy(binaryBuffer, sizeof(binaryBuffer), bin);
        }

        ImGui::SameLine();

        if (ImGui::Button("Copy to clipboard##text")) {
            CopyToClipboard(textBuffer);
        }

        ImGui::SameLine();

        if (ImGui::Button("Clean##text")) {
            ClearBuffer(textBuffer, sizeof(textBuffer));
        }

        ImGui::Separator();

        // -------------------- BINARY LOGIC --------------------
        ImGui::Text("Binary:");
        ImGui::InputTextMultiline("##binary", binaryBuffer, sizeof(binaryBuffer), ImVec2(-1, 170));

        if (ImGui::Button("Binary to Tex")) {
            std::string txt = BinaryToText(binaryBuffer);
            SafeCopy(textBuffer, sizeof(textBuffer), txt);
        }

        ImGui::SameLine();

        if (ImGui::Button("Copy to clipboard##binary")) {
            CopyToClipboard(binaryBuffer);
        }

        ImGui::SameLine();

        if (ImGui::Button("Clean##binary")) {
            ClearBuffer(binaryBuffer, sizeof(binaryBuffer));
        }

	
	// -------------------- FOOTER --------------------
        ImGui::Spacing();
        ImGui::Separator();
        
        // Calculate vertical position to stick to the bottom
        // window height - padding - text height
        float footerHeight = ImGui::GetTextLineHeightWithSpacing();
        ImGui::SetCursorPosY(ImGui::GetWindowHeight() - footerHeight - 10); 

        // Center the text horizontally (optional)
        std::string footerText = "Developed by Joa98 | Version 1.0.0 | 2026";
        float windowWidth = ImGui::GetWindowSize().x;
        float textWidth = ImGui::CalcTextSize(footerText.c_str()).x;
        
        ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
        
        // Render the footer text in a subtle gray color
        ImGui::TextDisabled("%s", footerText.c_str());	

        ImGui::End();

        // -------------------- Render --------------------
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);

	// Background color (Dark gray)
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

	// Draw the ImGui data to the screen
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// Swap the front and back buffers to display the frame
        glfwSwapBuffers(window);
    }

    // -------------------- Cleanup --------------------

    // Properly shut down the libraries and free memory
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

