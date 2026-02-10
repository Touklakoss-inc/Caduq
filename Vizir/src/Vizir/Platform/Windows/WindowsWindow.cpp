#include "vzpch.h"
#include "WindowsWindow.h"

#include "Core/Core.h"

#include "EventSystem/ApplicationEvent.h"
#include "../OpenGL/OpenGLContext.h"

#include <glad/glad.h>

namespace Vizir
{
    static bool  s_GLFWInitialized = false;

    static void GLFWErrorCallback(int error, const char* description)
    {
        VZ_ERROR("GLFW Error ({0}): {1}", error, description);
    }

    Window* Window::Create(const WindowProps& props)
    {
        BOB_PROFILE_FUNC()

        return new WindowsWindow(props);
    }

    WindowsWindow::WindowsWindow(const WindowProps& props)
    {
        Init(props);
    }

    WindowsWindow::~WindowsWindow()
    {
        Shutdown();
        glfwTerminate();
    }

    void WindowsWindow::Init(const WindowProps& props)
    {
        BOB_PROFILE_FUNC()

        m_Data.Title = props.Title;
        m_Data.Width = props.Width;
        m_Data.Height = props.Height;

        VZ_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);


        if (!s_GLFWInitialized)
        {
            BOB_PROFILE_SCOPE("glfwInit")

            int success = glfwInit();
            VZ_ASSERT(success, "Could not initialize GLFW!");
            glfwSetErrorCallback(GLFWErrorCallback);
            s_GLFWInitialized = true;
        }

        {
            BOB_PROFILE_SCOPE("glfwCreateWindow")

            m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, props.Title.c_str(), nullptr, nullptr);
        }

        {
            BOB_PROFILE_SCOPE("Init")

            m_Context = CreateScope<OpenGLContext>(m_Window);
            m_Context->Init();
        }

        glfwSetWindowUserPointer(m_Window, &m_Data);
        SetVSync(true);

        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
        {
            WindowData& data = * (WindowData*) glfwGetWindowUserPointer(window);
            data.Width = width;
            data.Height = height;

            WindowResizedEvent event(width, height);
            data.EventCallback(event);
        });

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            WindowClosedEvent event;
            data.EventCallback(event);
        });

        glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int key)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            KeyTypedEvent event(key);
            data.EventCallback(event);

        });

        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            switch (action)
            {
                case GLFW_PRESS:
                {
                    KeyPressedEvent event(key, 0);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_REPEAT:
                {
                    KeyPressedEvent event(key, 1);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    KeyReleasedEvent event(key);
                    data.EventCallback(event);
                    break;
                }
            }
        });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            switch (action)
            {
                case GLFW_PRESS:
                {
                    MouseButtonPressedEvent event(button);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    MouseButtonReleasedEvent event(button);
                    data.EventCallback(event);
                    break;
                }
            }
        });

        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            
            MouseScrolledEvent event(xoffset, yoffset);
            data.EventCallback(event);
        });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            MouseMovedEvent event(xpos, ypos);
            data.EventCallback(event);
        });
    }


    void WindowsWindow::Shutdown()
    {
        BOB_PROFILE_FUNC()
        glfwDestroyWindow(m_Window);
    }

    void WindowsWindow::OnUpdate()
    {
        BOB_PROFILE_FUNC()

        m_Context->SwapBuffers();
        glfwPollEvents();
    }

    void WindowsWindow::SetVSync(bool value)
    {
        if (value)
        {
            glfwSwapInterval(1);
        }
        else
        {
            glfwSwapInterval(0);
        }

        m_Data.VSync = value;
    }

    bool WindowsWindow::IsVSync() const
    {
        return m_Data.VSync;
    }
}
