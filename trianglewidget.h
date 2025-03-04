#ifndef TRIANGLEWIDGET_H
#define TRIANGLEWIDGET_H

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLWidget>

class TriangleWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit TriangleWidget(QWidget *parent = nullptr)
        : QOpenGLWidget(parent)
        , vao(nullptr)
        , vbo(nullptr)
        , ibo(nullptr)
    {}

    ~TriangleWidget()
    {
        // 在析构函数中清理 OpenGL 资源
        delete vao;
        delete vbo;
        delete ibo;
    }

protected:
    void initializeGL() override
    {
        initializeOpenGLFunctions();

        // 创建并编译着色器程序
        shaderProgram.addShaderFromSourceCode(QOpenGLShader::Vertex, //顶点着色器
                                              "#version 330 core\n"
                                              "layout(location = 0) in vec3 position;\n"
                                              "void main() {\n"
                                              "   gl_Position = vec4(position, 1.0);\n"
                                              "}");
        shaderProgram.addShaderFromSourceCode(QOpenGLShader::Fragment, //片段着色器
                                              "#version 330 core\n"
                                              "out vec4 FragColor;\n"
                                              "void main() {\n"
                                              "   FragColor = vec4(1.0, 0.0, 0.0, 1.0);\n"
                                              "}");

        shaderProgram.link();
        // 定义三角形的顶点数据
        GLfloat vertices[] = {
            0.0f,
            0.5f,
            0.0f, // 上
            -0.5f,
            -0.5f,
            0.0f, // 左
            0.5f,
            -0.5f,
            0.0f // 右
        };

        GLuint indices[] = {
            0, 1, 2 // 三角形的顶点索引
        };

        // 创建 VAO、VBO、IBO
        vao = new QOpenGLVertexArrayObject(this);
        vao->create();
        vao->bind();

        // 创建并填充 VBO
        vbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
        vbo->create();
        vbo->bind();
        vbo->allocate(vertices, sizeof(vertices));

        // 创建并填充 IBO
        ibo = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
        ibo->create();
        ibo->bind();
        ibo->allocate(indices, sizeof(indices));

        // 绑定 VBO 数据
        shaderProgram.enableAttributeArray(0);
        shaderProgram.setAttributeBuffer(0, GL_FLOAT, 0, 3);

        vao->release();
        vbo->release();
        ibo->release();
    }

    void resizeGL(int w, int h) override { glViewport(0, 0, w, h); }

    void paintGL() override
    {
        glClear(GL_COLOR_BUFFER_BIT);

        shaderProgram.bind();
        vao->bind();
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
        vao->release();
        shaderProgram.release();
    }

private:
    QOpenGLShaderProgram shaderProgram;
    QOpenGLVertexArrayObject *vao;
    QOpenGLBuffer *vbo;
    QOpenGLBuffer *ibo;
};

#endif // TRIANGLEWIDGET_H
