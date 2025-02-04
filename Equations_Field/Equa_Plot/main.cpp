#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "implot.h"
#include <stdio.h>
#include <algorithm>
#include <exprtk.hpp>
#include <string>

#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
#include <GL/gl3w.h> // Initialize with gl3wInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
#include <GL/glew.h> // Initialize with glewInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
#include <glad/glad.h> // Initialize with gladLoadGL()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD2)
#include <glad/gl.h> // Initialize with gladLoadGL(...) or gladLoaderLoadGL()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING2)
#define GLFW_INCLUDE_NONE      // GLFW including OpenGL headers causes ambiguity or multiple definition errors.
#include <glbinding/Binding.h> // Initialize with glbinding::Binding::initialize()
#include <glbinding/gl/gl.h>
using namespace gl;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING3)
#define GLFW_INCLUDE_NONE        // GLFW including OpenGL headers causes ambiguity or multiple definition errors.
#include <glbinding/glbinding.h> // Initialize with glbinding::initialize()
#include <glbinding/gl/gl.h>
using namespace gl;
#else
#include IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#endif

#include <GLFW/glfw3.h>
#pragma comment(lib, "legacy_stdio_definitions")

static void glfw_error_callback(int error, const char *description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

static ImVec2 window_size = {1280, 720};
static void glfw_window_size_callback(GLFWwindow *, int width, int height)
{
    window_size.x = width;
    window_size.y = height;
}

template <typename T>
const exprtk::expression<T> &parse_expr(const std::string &expr_str, T &x)
{
    typedef exprtk::symbol_table<T> symbol_table_t;
    typedef exprtk::expression<T> expression_t;
    typedef exprtk::parser<T> parser_t;


    symbol_table_t symbol_table;
    symbol_table.add_variable("x", x);
    symbol_table.add_constants();

    expression_t expression;
    expression.register_symbol_table(symbol_table);

    parser_t parser;
    parser.compile(expr_str, expression);

    return expression;
}

int main(int, char **)
{
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

        // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char *glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char *glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char *glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
    GLFWwindow *window = glfwCreateWindow(window_size.x, window_size.y, "ImMath", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync
    glfwSetWindowSizeCallback(window, glfw_window_size_callback);

    // Initialize OpenGL loader
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
    bool err = gl3wInit() != 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
    bool err = glewInit() != GLEW_OK;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
    bool err = gladLoadGL() == 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD2)
    bool err = gladLoadGL(glfwGetProcAddress) == 0; // glad2 recommend using the windowing library loader instead of the (optionally) bundled one.
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING2)
    bool err = false;
    glbinding::Binding::initialize();
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING3)
    bool err = false;
    glbinding::initialize([](const char *name)
                          { return (glbinding::ProcAddress)glfwGetProcAddress(name); });
#else
    bool err = false; // If you use IMGUI_IMPL_OPENGL_LOADER_CUSTOM, your loader is likely to requires some form of initialization.
#endif
    if (err)
    {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        return 1;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        {
            static float xmin = -7;
            static float xmax = 7;
            static float ymin = -4;
            static float ymax = 4;
            static int left_size = 200;
            static int vsplitter_width = 2;

            ImGui::SetNextWindowPos({0, 0});
            ImGui::SetNextWindowSize(window_size);
            ImGui::Begin("math", 0, ImGuiWindowFlags_NoDecoration);

            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 0});
            ImGui::SetNextWindowPos({0, 0});
            ImGui::BeginChild("Settings", {(float)left_size - vsplitter_width, window_size.y});

            float x;
            static char formula[128] = "clamp(-1.0,sin(2 * pi * x) + cos(x / 2 * pi),+1.0)";
            std::string expr_str(formula);
            static ImVec4 line_color = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);

            if (ImGui::CollapsingHeader("formula1"))
            {
                ImGui::PushItemWidth(left_size);
                ImGui::InputText("##formula", formula, IM_ARRAYSIZE(formula));
                ImGui::PopItemWidth();
                ImGui::ColorEdit3("line color", (float *)&line_color, ImGuiColorEditFlags_NoInputs);
            }

            typedef exprtk::symbol_table<float> symbol_table_t;
            typedef exprtk::expression<float> expression_t;
            typedef exprtk::parser<float> parser_t;

            symbol_table_t symbol_table;
            symbol_table.add_variable("x", x);
            symbol_table.add_constants();

            expression_t expression;
            expression.register_symbol_table(symbol_table);

            parser_t parser;
            parser.compile(expr_str, expression);

            static const float speed = 0.1;
            if (ImGui::CollapsingHeader("Axis", ImGuiTreeNodeFlags_None))
            {
                ImGui::DragFloat("xmin", &xmin, speed, -1000, xmax - speed);
                ImGui::Separator();
                ImGui::DragFloat("xmax", &xmax, speed, xmin + speed, 1000);
                ImGui::Separator();
                ImGui::DragFloat("ymin", &ymin, speed, -1000, ymax - speed);
                ImGui::Separator();
                ImGui::DragFloat("ymax", &ymax, speed, ymin + speed, 1000);
                ImGui::Separator();
            }

            ImGui::EndChild();

            ImVector<ImVec2> y;
            for (x = xmin; x < xmax; x += speed)
            {
                float res = expression.value();
                if (res >= ymin && res <= ymax)
                {
                    y.push_back({x, res});
                }
            }

            ImGui::SameLine();
            ImGui::Button("vsplitter", ImVec2(vsplitter_width, window_size.y));
            if (ImGui::IsItemActive())
                left_size += ImGui::GetIO().MouseDelta.x;

            ImGui::SetNextWindowPos({(float)left_size, 0});
            ImGui::BeginChild("Math Plot", {0, window_size.y});

            ImPlot::SetNextPlotLimits(xmin, xmax, ymin, ymax);
            ImPlot::PushStyleColor(ImPlotCol_Line, line_color);
            if (ImPlot::BeginPlot("##f(x)", NULL, NULL, {window_size.x - left_size, window_size.y}))
            {
                if (!y.empty())
                {
                    ImPlot::PlotLine("##(fx)", &y[0].x, &y[0].y, y.size(), 0, sizeof(ImVec2));
                }
                ImPlotLimits limits = ImPlot::GetPlotLimits();
                xmin = limits.X.Min;
                xmax = limits.X.Max;
                ymin = limits.Y.Min;
                ymax = limits.Y.Max;
                ImPlot::EndPlot();
            }
            ImPlot::PopStyleColor();

            ImGui::EndChild();
            ImGui::PopStyleVar();

            ImGui::End();
        }
        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    ImPlot::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
