#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

// Pencere boyutları
const GLuint WIDTH = 800, HEIGHT = 600;

int main() {
    // GLFW'yi başlat
    if (!glfwInit()) {
        fprintf(stderr, "GLFW başlatılamadı!\n");
        return -1;
    }

    // OpenGL versiyonunu ayarla
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Pencere oluştur
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL Test", NULL, NULL);
    if (!window) {
        fprintf(stderr, "Pencere oluşturulamadı!\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // GLEW'i başlat
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "GLEW başlatılamadı!\n");
        return -1;
    }

    // Viewport ayarla
    glViewport(0, 0, WIDTH, HEIGHT);

    // Vertex verisi
    GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f, // Sol alt
         0.5f, -0.5f, 0.0f, // Sağ alt
         0.0f,  0.5f, 0.0f  // Üst
    };

    // Vertex Buffer Object (VBO) ve Vertex Array Object (VAO) oluştur
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // VAO'yu bağla
    glBindVertexArray(VAO);

    // VBO'yu bağla ve veriyi gönder
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Vertexleri OpenGL'e tanıt
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Bağlantıları kaldır
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Basit bir vertex ve fragment shader
    const GLchar* vertexShaderSource = "#version 330 core\n"
                                       "layout (location = 0) in vec3 position;\n"
                                       "void main()\n"
                                       "{\n"
                                       "    gl_Position = vec4(position, 1.0);\n"
                                       "}\0";
    const GLchar* fragmentShaderSource = "#version 330 core\n"
                                         "out vec4 color;\n"
                                         "void main()\n"
                                         "{\n"
                                         "    color = vec4(1.0, 0.0, 0.0, 1.0);\n"
                                         "}\n\0";

    // Vertex Shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Fragment Shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Shader Program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Shader'ları temizle
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Render döngüsü
    while (!glfwWindowShouldClose(window)) {
        // Olayları kontrol et
        glfwPollEvents();

        // Ekranı temizle
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Shader programını kullan
        glUseProgram(shaderProgram);

        // Üçgeni çiz
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        // Bufferswap
        glfwSwapBuffers(window);
    }

    // Temizlik
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // GLFW'i kapat
    glfwTerminate();
    return 0;
}
