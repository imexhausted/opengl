#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

struct ShaderProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource parseShader(const std::string filepath){
    std::ifstream stream(filepath);

    if (!stream.is_open()){
        std::cout << "file didn't open";
    }

    enum class ShaderType { NONE = -1, VERTEX = 0, FRAGMENT = 1 };
    ShaderType type = ShaderType::NONE;

    std::string line;
    std::stringstream ss[2];
    
    while (getline(stream, line)){
        if (line.find("#shader") != std::string::npos){
            
            if (line.find("vertex") != std::string::npos){
                type = ShaderType::VERTEX;
            } else if (line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;
            }

        } 
        else {
            ss[(int)type] << line << "\n";
        }
    }

    return { ss[0].str(), ss[1].str()};
}

static unsigned int compileShader(unsigned int type, const std::string& source){

    unsigned int id = glCreateShader(type); 
    // creates an empty shader object and returns a non-zero value by which it can be referenced
    // A shader object is used to maintain the source code strings that define a shader. shaderType indicates the type of shader to be created

    const char* src = source.c_str();
    
    glShaderSource(id, 1, &src, nullptr);
    // Sets the source code in shader to the source code in the array of strings specified by string. 
    // Any source code previously stored in the shader object is completely replaced.
    
    glCompileShader(id);
    //  compiles the source code strings that have been stored in the shader object specified by shader.

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE){
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char *)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment" ) << " shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);

        return 0;
    }

    return id;
}

static unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader){
    
    unsigned int program = glCreateProgram(); 
    //creates an empty program object and returns a non-zero value by which it can be referenced
    // A program object is an object to which shader objects can be attached. 
    // This provides a mechanism to specify the shader objects that will be linked to create a program.
    
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader); //vs stands for verex shader for details see function declaration
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader); //fs stands for fragment shader for details see function declaration

    glAttachShader(program, fs); // attaches shader to the program (see glCreateProgram)
    glAttachShader(program, vs); // attaches shader to the program (see glCreateProgram)
    
    glLinkProgram(program); 
    // If any shader objects of type GL_VERTEX_SHADER / GL_FRAGMENT_SHADER are attached to program, they will be used 
    // to create an executable that will run on the programmable vertex / fragment processor

    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) return -1;

    float positions[6] = {-0.5f, -0.5f, 
                           0.0f,  0.5f, 
                           0.5f, -0.5f};

    unsigned int bufferId; // declaring a kind of ID of the buffer
    glGenBuffers(1, &bufferId); //generate buffer with reference to unassigned buffer ID variable
    glBindBuffer(GL_ARRAY_BUFFER, bufferId); // set openGL to use this actual buffer with this id
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW); // adding data to the buffer

    glEnableVertexAttribArray(0); // enabling attributes array
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0); //defining actual attributes of the buffer

    ShaderProgramSource source = parseShader("./res/shaders/basic.shader");

    unsigned int shader = createShader(source.VertexSource, source.FragmentSource);
    glUseProgram(shader);


    glBindBuffer(GL_ARRAY_BUFFER, 0);  //unbinding buffer

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}