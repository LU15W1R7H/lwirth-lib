#pragma once

#include "Standard.hpp"

#include <string>
#include "Vector.hpp"

struct GLFWwindow;

namespace lw
{
	class LWAPI Frame
	{
		friend class RenderWindow;

	private:
		static size_t s_frameCount;
		GLFWwindow* m_pFrame = nullptr;
		bool m_created = false;

	public:
		Frame();
		~Frame();

		virtual void create(std::string& title, u32 width, u32 height);
		virtual void create(std::string&& title, u32 width, u32 height);
		virtual void destroy();

		virtual void update();


		GLFWwindow* raw() const { return m_pFrame; }
		bool wasCreated() const { return m_created; }

		bool isOpen() const;
		void open() const;
		void close() const;
		u32 getWidth() const;
		u32 getHeight() const;
		void setWidth(u32 newWidth) const;
		void setHeight(u32 newHeight) const;
		void setSize(u32 newWidth, u32 newHeight) const;
		u32 getPosX() const;
		u32 getPosY() const;
		void setPosX(u32 newX) const;
		void setPosY(u32 newY) const;
		void setPos(u32 newX, u32 newY) const;
		void setTitle(std::string& newTitle) const;
		void setTitle(std::string&& newTitle) const;
		//void setIcon(lw::Image& image) const;
		void setMinimized(bool enable) const;
		void setVisibility(bool enable) const;
		void setFocus() const;


	private:
		virtual void resize(u32 width, u32 height) {}

		static void keyCallback(GLFWwindow* glfwWin, int key, int scancode, int action, int mods);
		static void mouseButtonCallback(GLFWwindow* glfwWin, int button, int action, int mods);
		static void cursorPositionCallback(GLFWwindow* glfwWin, double xpos, double ypos);
		static void resizeCallback(GLFWwindow* glfwWin, int width, int height);

	};

}



