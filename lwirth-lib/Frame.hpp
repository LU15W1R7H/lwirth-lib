#pragma once

#include "Standard.hpp"

#include <string>
#include "Vector.hpp"

struct GLFWwindow;

namespace lw
{
	class API Frame
	{
		friend class RenderWindow;

	private:
		static Size s_frameCount;
		GLFWwindow* m_pFrame = nullptr;
		bool m_created = false;

	public:
		Frame();
		~Frame();

		virtual void create(std::string& title, U32 width, U32 height);
		virtual void create(std::string&& title, U32 width, U32 height);
		virtual void destroy();

		virtual void update();


		GLFWwindow* raw() const { return m_pFrame; }
		bool wasCreated() const { return m_created; }

		bool isOpen() const;
		void open() const;
		void close() const;
		U32 getWidth() const;
		U32 getHeight() const;
		void setWidth(U32 newWidth) const;
		void setHeight(U32 newHeight) const;
		void setSize(U32 newWidth, U32 newHeight) const;
		U32 getPosX() const;
		U32 getPosY() const;
		void setPosX(U32 newX) const;
		void setPosY(U32 newY) const;
		void setPos(U32 newX, U32 newY) const;
		void setTitle(std::string& newTitle) const;
		void setTitle(std::string&& newTitle) const;
		//void setIcon(lw::Image& image) const;
		void setMinimized(bool enable) const;
		void setVisibility(bool enable) const;
		void setFocus() const;


	private:
		virtual void resize(U32 width, U32 height) {}

		static void keyCallback(GLFWwindow* glfwWin, int key, int scancode, int action, int mods);
		static void mouseButtonCallback(GLFWwindow* glfwWin, int button, int action, int mods);
		static void cursorPositionCallback(GLFWwindow* glfwWin, double xpos, double ypos);
		static void resizeCallback(GLFWwindow* glfwWin, int width, int height);

	};

}



