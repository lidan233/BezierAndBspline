//
// Created by lidan on 30/10/2020.
//

#ifndef BEZIERANDBSPLINE_UI_H
#define BEZIERANDBSPLINE_UI_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include "Eigen/Dense"
#include "Shader.h"
#include "camera.h"
#include "callback.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


#define WIDTH 800
#define HEIHGT 600

enum Flag_{
    FLAG_LINE ,
    FLAG_TRIANGLE
};

GLFWwindow *init_window() {
    // INITIALIZATION
    if (!glfwInit()) {
        std::cerr << "Failed to init GLFW" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    const char *glsl_version = "#version 330 core";

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

#ifndef _WIN32
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // No idea why, but this breaks everything on Windows 7
#endif

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    glfwWindowHint(GLFW_RESIZABLE, false);

    // Creates window
    GLFWwindow *window = glfwCreateWindow(WIDTH,HEIHGT, "Hello world!", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  // Disables cursor

    // GLAD initialize; we must do this before all OpenGL lib functions
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }

    // GL config; these don't change much
    glViewport(0, 0, WIDTH, HEIHGT);
    glEnable(GL_BLEND);  // Blend for alpha things
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return window;
}


void init_imgui(GLFWwindow *window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO(); (void)io;
//    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;  // Enable some nice imgui features
//    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
}


std::string read_shader(const char *path) {
    try {
        std::ifstream filestream(path);
        std::stringstream codestream;

        codestream << filestream.rdbuf();

        filestream.close();

        return codestream.str();
    } catch (std::exception e) {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void check_shader_error(GLuint shader, std::string extra_message) {
    int success;
    char err_message[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, err_message);
        std::cerr << extra_message << err_message << std::endl;
        exit(EXIT_FAILURE);
    }
}


void display(std::string vshaderf, std::string fshaderf, Eigen::Vector3d* vertices,std::string cvshaderf, std::string cfshaderf, Eigen::Vector3d* control_vertices, int size, int control_size, Flag_ flag)
{
    GLFWwindow *window = init_window();
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    init_imgui(window);

//    float vertices[] = {
//            // positions         // colors
//            0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
//            -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
//            0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top
//    };


    float* opengl_vertice= nullptr ;

    if(flag == FLAG_LINE)
    {
        opengl_vertice = new float[size*3*2] ;
        for(int i = 1 ; i < size ;i++)
        {
            opengl_vertice[6*i+0] = vertices[i-1][0] ;
            opengl_vertice[6*i+1] = vertices[i-1][1] ;
            opengl_vertice[6*i+2] = vertices[i-1][2] ;

            opengl_vertice[6*i+3] = vertices[i][0] ;
            opengl_vertice[6*i+4] = vertices[i][1] ;
            opengl_vertice[6*i+5] = vertices[i][2] ;
//        std::cout<<" line from"<<vertices[i-1]<<" to "<<vertices[i]<<std::endl ;
        }
    }else{
        opengl_vertice = new float[size*3] ;
        for(int i = 0 ; i < size; i++)
        {
            opengl_vertice[3*i+0] = vertices[i][0] ;
            opengl_vertice[3*i+1] = vertices[i][1] ;
            opengl_vertice[3*i+2] = vertices[i][2] ;

            std::cout<<vertices[i][0]<<" "<<vertices[i][1]<<" "<<vertices[i][2]<<std::endl ;
        }
    }


    float* control_points = new float[control_size*3] ;
    for(int i = 0 ; i < control_size ;i++)
    {
        control_points[i*3+0] = control_vertices[i][0] ;
        control_points[i*3+1] = control_vertices[i][1] ;
        control_points[i*3+2] = control_vertices[i][2] ;
    }



    // Set up buffers
    GLuint VBO, VBO1, VAO,VAO1;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);


    if(flag == FLAG_LINE)
        glBufferData(GL_ARRAY_BUFFER,size * 3 * 2 * sizeof(GLfloat), opengl_vertice, GL_STATIC_DRAW);
    else
        glBufferData(GL_ARRAY_BUFFER,size * 3  * sizeof(GLfloat), opengl_vertice, GL_STATIC_DRAW);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, (GLvoid *)0);
    glEnableVertexAttribArray(0);

    glGenVertexArrays(1, &VAO1);
    glBindVertexArray(VAO1);

    glGenBuffers(1, &VBO1) ;
    glBindBuffer(GL_ARRAY_BUFFER,VBO1) ;
    glBufferData(GL_ARRAY_BUFFER, control_size*3*sizeof(GLfloat), control_points, GL_STATIC_DRAW) ;
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, (GLvoid *)0);
    glEnableVertexAttribArray(0);


    glLineWidth(2.f) ;
    glPointSize(3.f) ;

    Shader Lineshader(vshaderf.c_str(),fshaderf.c_str()) ;
    Shader PointShader(cvshaderf.c_str(), cfshaderf.c_str()) ;

    ImVec4 clear_color = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
    bool show_demo_window = true;
    bool show_another_window = false;

    while (!glfwWindowShouldClose(window)) {
        processInput(window) ;
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);
        Lineshader.use() ;
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 model = glm::mat4(1.0f);

        Lineshader.setMat4("projection", projection);
        Lineshader.setMat4("view", view);
        Lineshader.setMat4("model", model);

        switch (flag) {
            case FLAG_LINE:
                glDrawArrays(GL_LINES, 0, size*2);
            case FLAG_TRIANGLE:
                glDrawArrays(GL_TRIANGLES,0,size);

        }

        glBindVertexArray(VAO1) ;
        PointShader.use() ;
        PointShader.setMat4("projection", projection);
        PointShader.setMat4("view", view);
        PointShader.setMat4("model", model);

        glDrawArrays(GL_POINTS,0,control_size) ;

        glBindVertexArray(0);

        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        if (show_another_window) {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
//        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
//        {
//            GLFWwindow *backup_current_context = glfwGetCurrentContext();
//            ImGui::UpdatePlatformWindows();
//            ImGui::RenderPlatformWindowsDefault();
//            glfwMakeContextCurrent(backup_current_context);
//        }

        glfwSwapBuffers(window);
    }

    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);

    glDeleteBuffers(1, &VBO1);
    glDeleteVertexArrays(1, &VAO1);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

}

#endif //BEZIERANDBSPLINE_UI_H
