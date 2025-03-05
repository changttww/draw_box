#ifndef TEXTURED_RECTANGLE_H
#define TEXTURED_RECTANGLE_H

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLWidget>
#include <QOpenGLTexture>
#include <QMatrix4x4>
#include <QTimer>
#include <QKeyEvent>

class TexturedRectangle: public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT;
public:

    TexturedRectangle(QWidget *parent = nullptr) : QOpenGLWidget(parent), m_drawRectangle(false)
    {

        setFocusPolicy(Qt::StrongFocus);

        //旋转定时器
        QTimer *timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, QOverload<>::of(&QOpenGLWidget::update));
        timer->start(16); // 16ms ≈ 60FPS

        // 摄像机移动定时器，响应按键移动
        movementTimer = new QTimer(this);
        connect(movementTimer, &QTimer::timeout, this, &TexturedRectangle::updateCameraPosition);
        movementTimer->start(16); // 16ms 响应按键（更灵敏）
    }

    ~TexturedRectangle() { cleanup(); }

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

    void keyPressEvent(QKeyEvent *event) override {
        keysPressed.insert(event->key());  // 记录按下的按键
    }

    void keyReleaseEvent(QKeyEvent *event) override {
        keysPressed.remove(event->key());  // 移除松开的按键
    }

    void updateCameraPosition() {
        float deltaMove = 0.05f; // 移动速度

        QVector3D moveDirection(0.0f, 0.0f, 0.0f);

        if (keysPressed.contains(Qt::Key_W)) {
            moveDirection += cameraFront;  // 前进
        }
        if (keysPressed.contains(Qt::Key_S)) {
            moveDirection -= cameraFront;  // 后退
        }
        if (keysPressed.contains(Qt::Key_A)) {
            moveDirection -= QVector3D::crossProduct(cameraFront, cameraUp).normalized(); // 左移
        }
        if (keysPressed.contains(Qt::Key_D)) {
            moveDirection += QVector3D::crossProduct(cameraFront, cameraUp).normalized(); // 右移
        }

        // 归一化后再缩放，防止对角线方向比单向快
        if (!moveDirection.isNull()) {
            cameraPos += moveDirection.normalized() * deltaMove;
        }

        update(); // 触发重绘
    }

    /**
     * @brief 初始化 OpenGL 资源（如 VBO、VAO、IBO 和 Shader）
     */
    void initializeGL() override
    {
        // 初始化 OpenGL 函数
        initializeOpenGLFunctions();

        GLfloat vertices[] = {
            // 位置            // 颜色           // 纹理坐标
            // 前面
            -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, // 左下
            0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f, // 右下
            0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f, // 右上
            -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f, // 左上

            // 后面
            -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,  0.0f, 0.0f, // 左下
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  1.0f, 0.0f, // 右下
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f, // 右上
            -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f, // 左上

            // 左面
            -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,

            // 右面
            0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,  0.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,

            // 顶面
            -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,

            // 底面
            -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,  0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  1.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f
        };


        // 立方体的 6 个面，每个面 2 个三角形
        GLuint indices[] = {
            0, 1, 2, 2, 3, 0,   // 前面
            4, 5, 6, 6, 7, 4,   // 后面
            8, 9,10,10,11, 8,   // 左面
            12,13,14,14,15,12,  // 右面
            16,17,18,18,19,16,  // 顶面
            20,21,22,22,23,20   // 底面
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
        shaderProgram.setAttributeBuffer(0, GL_FLOAT, 0, 3, 8 * sizeof(GLfloat));

        shaderProgram.enableAttributeArray(1); // 颜色属性
        shaderProgram.setAttributeBuffer(1, GL_FLOAT, 3 * sizeof(GLfloat), 3, 8 * sizeof(GLfloat));

        // 纹理坐标属性
        shaderProgram.enableAttributeArray(2);
        shaderProgram.setAttributeBuffer(2, GL_FLOAT, 6 * sizeof(GLfloat), 2, 8 * sizeof(GLfloat));

        // 加载纹理
        texture = new QOpenGLTexture(QImage(":/textures/bb.png").mirrored());
        texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
        texture->setMagnificationFilter(QOpenGLTexture::Linear);
        texture->setWrapMode(QOpenGLTexture::Repeat);



        // 释放 VAO、VBO、IBO 和 Shader
        vao.release();
        vbo.release();
        ibo.release();
        shaderProgram.release();
        texture->release();
    }

    void paintGL() override {
        if(m_drawRectangle)
        {
            // 清空颜色和深度缓冲区
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            //矩阵计算
            QMatrix4x4 model, view, projection;
            model.rotate(rotation, 1.0f, 1.0f, 0.0f);


            //view.lookAt(QVector3D(0.0f, 0.0f, 3.0f), QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f));
            view.lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

            projection.perspective(45.0f, float(width()) / height(), 0.01f, 500.0f);

            shaderProgram.bind();
            shaderProgram.setUniformValue("model", model);
            shaderProgram.setUniformValue("view", view);
            shaderProgram.setUniformValue("projection", projection);


            // 绑定着色器程序
            shaderProgram.bind();
            // 绑定 VAO
            vao.bind();
            texture->bind();
            // 通过索引缓冲区绘制矩形
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
            // 释放 VAO 和 Shader
            vao.release();
            texture->release();
            shaderProgram.release();
            rotation += 1.0f;
        }
    }

    void resizeGL(int w, int h) override { glViewport(0, 0, w, h); }

private:
    QOpenGLShaderProgram shaderProgram;             // 着色器程序
    QOpenGLBuffer vbo{QOpenGLBuffer::VertexBuffer}; // 顶点缓冲对象
    QOpenGLBuffer ibo{QOpenGLBuffer::IndexBuffer};  // 索引缓冲对象
    QOpenGLVertexArrayObject vao;                   // 顶点数组对象
    QOpenGLTexture *texture;
    float rotation = 0.0f;

    bool m_drawRectangle;  // 控制是否绘制的标志

    float yaw = -90.0f; // 初始偏航角，朝向 -Z 轴
    float pitch = 0.0f;  // 俯仰角
    float cameraSpeed = 2.5f; // 摄像机移动速度
    QVector3D cameraPos = QVector3D(0.0f, 0.0f, 3.0f);  // 摄像机位置
    QVector3D cameraFront = QVector3D(0.0f, 0.0f, -1.0f); // 摄像机朝向
    QVector3D cameraUp = QVector3D(0.0f, 1.0f, 0.0f);  // 摄像机的上向量


    QSet<int> keysPressed;  // 存储当前被按下的按键
    QTimer *movementTimer;  // 用于定期更新摄像机位置

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
        delete texture;
        doneCurrent(); // 释放 OpenGL 上下文
    }
};


#endif // TEXTURED_RECTANGLE_H
