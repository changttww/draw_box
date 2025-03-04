#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLWidget>

class OpenGLRectangle : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:

    OpenGLRectangle(QWidget *parent = nullptr) : QOpenGLWidget(parent), m_drawRectangle(false){}

    ~OpenGLRectangle() { cleanup(); }

    void setDrawRectangle(bool enabled) {
        m_drawRectangle = enabled;
        update();  // 触发重绘
    }

    /**
     * @brief 清空矩形数据
     */
    void clearRectangle() {
        m_drawRectangle = false;
        // makeCurrent();
        // vbo.destroy();
        // ibo.destroy();
        // vao.destroy();
        // shaderProgram.removeAllShaders();
        // doneCurrent();
        update();  // 触发重绘
    }

protected:
    /**
     * @brief 初始化 OpenGL 资源（如 VBO、VAO、IBO 和 Shader）
     */
    void initializeGL() override
    {
        // 初始化 OpenGL 函数
        initializeOpenGLFunctions();

        // 定义矩形的顶点数据 (x, y, z, r, g, b)
        GLfloat vertices[] = {
            // 位置坐标      // 颜色（RGB）
            -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // 左下角（红色）
            0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // 右下角（绿色）
            0.5f,  0.5f, 0.0f, 0.5f, 0.0f, 1.0f, // 右上角（蓝色）
            -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f  // 左上角（黄色）

        };

        // 定义索引数据，用于绘制两个三角形形成矩形
        GLuint indices[] = {
            0,
            1,
            2, // 第一个三角形（左下 → 右下 → 右上）
            2,
            3,
            0 // 第二个三角形（右上 → 左上 → 左下）
        };

        // 创建并绑定 VAO（顶点数组对象）
        vao.create();
        vao.bind();

        // 创建 VBO（顶点缓冲对象），存储顶点数据
        vbo.create();
        vbo.bind();
        vbo.allocate(vertices, sizeof(vertices));

        // 创建 IBO（索引缓冲对象），存储索引数据
        ibo.create();
        ibo.bind();
        ibo.allocate(indices, sizeof(indices));

        // 加载并编译着色器
        shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertex.glsl");
        shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragment.glsl");
        shaderProgram.link();
        shaderProgram.bind();

        // 启用并设置顶点属性指针
        shaderProgram.enableAttributeArray(0); // 位置属性
        shaderProgram.setAttributeBuffer(0, GL_FLOAT, 0, 3, 6 * sizeof(GLfloat));

        shaderProgram.enableAttributeArray(1); // 颜色属性
        shaderProgram.setAttributeBuffer(1, GL_FLOAT, 3 * sizeof(GLfloat), 3, 6 * sizeof(GLfloat));

        // 释放 VAO、VBO、IBO 和 Shader
        vao.release();
        vbo.release();
        ibo.release();
        shaderProgram.release();
    }





    void paintGL() override {
        if(m_drawRectangle)
        {
            // 清空颜色和深度缓冲区
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // 绑定着色器程序
            shaderProgram.bind();
            // 绑定 VAO
            vao.bind();
            // 通过索引缓冲区绘制矩形
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
            // 释放 VAO 和 Shader
            vao.release();
            shaderProgram.release();
        }
    }

    void resizeGL(int w, int h) override { glViewport(0, 0, w, h); }

private:
    QOpenGLShaderProgram shaderProgram;             // 着色器程序
    QOpenGLBuffer vbo{QOpenGLBuffer::VertexBuffer}; // 顶点缓冲对象
    QOpenGLBuffer ibo{QOpenGLBuffer::IndexBuffer};  // 索引缓冲对象
    QOpenGLVertexArrayObject vao;                   // 顶点数组对象

    bool m_drawRectangle;  // 控制是否绘制的标志

    /**
     * @brief 清理 OpenGL 资源
     */
    void cleanup()
    {
        makeCurrent(); // 设置 OpenGL 上下文为当前上下文
        vbo.destroy();
        ibo.destroy();
        vao.destroy();
        shaderProgram.removeAllShaders();
        doneCurrent(); // 释放 OpenGL 上下文
    }
};

#endif // RECTANGLE_H
