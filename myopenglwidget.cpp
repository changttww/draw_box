#include "MyOpenGLWidget.h"
#include <QOpenGLShader>
#include <qdebug.h>
MyOpenGLWidget::MyOpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
    , m_shaderProgram(nullptr)
{
    // 定义一个简单的三角形顶点
    m_vertices[0] = 0.0f;
    m_vertices[1] = 0.5f;
    m_vertices[2] = 0.0f;
    m_vertices[3] = -0.5f;
    m_vertices[4] = -0.5f;
    m_vertices[5] = 0.0f;
    m_vertices[6] = 0.5f;
    m_vertices[7] = -0.5f;
    m_vertices[8] = 0.0f;
}

MyOpenGLWidget::~MyOpenGLWidget()
{
    delete m_shaderProgram;
}

void MyOpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // 设置背景颜色为黑色
    glEnable(GL_DEPTH_TEST);              // 启用深度测试

    // 创建着色器程序
    m_shaderProgram = new QOpenGLShaderProgram(this);

    // 顶点着色器
    if (!m_shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex,
                                                  ":/shaders/vertex_shader.glsl")) {
        qCritical() << "Failed to compile vertex shader:" << m_shaderProgram->log();
        return;
    }

    // 片段着色器
    if (!m_shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment,
                                                  ":/shaders/fragment_shader.glsl")) {
        qCritical() << "Failed to compile fragment shader:" << m_shaderProgram->log();
        return;
    }

    // 链接着色器程序
    if (!m_shaderProgram->link()) {
        qCritical() << "Shader program link failed:" << m_shaderProgram->log();
        return;
    }

    // 创建顶点缓冲对象（VBO）
    m_vbo.create();
    m_vbo.bind();
    m_vbo.allocate(m_vertices, sizeof(m_vertices)); // 分配内存
}

void MyOpenGLWidget::resizeGL(int w, int h)
{
    // 设置视口
    glViewport(0, 0, w, h);
}

void MyOpenGLWidget::paintGL()
{
    qDebug() << "paintGL called"; // 输出日志，检查是否进入该函数
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 清除颜色缓冲区和深度缓冲区

    // 激活着色器程序
    m_shaderProgram->bind();

    // 绑定顶点缓冲对象
    m_vbo.bind();

    // 绘制三角形
    glDrawArrays(GL_TRIANGLES, 0, 3); // 绘制 3 个顶点组成的三角形

    // 释放着色器程序
    m_shaderProgram->release();
}
