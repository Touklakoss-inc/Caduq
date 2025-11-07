#pragma once

#include "vzpch.h"
#include "Event.h"
#include "Vizir/Core/Core.h"

namespace Vizir
{
	/*
	* ----- Windows Events -----
	*/

	class 
		VZ_API WindowClosedEvent : public Event
	{
	public:
		WindowClosedEvent() {}

		EVENT_CLASS_TYPE(WindowsClosed)
		EVENT_CLASS_CATEGORY(WindowsEventCategory)
	};

	class VZ_API WindowResizedEvent : public Event
	{
	public:
		WindowResizedEvent(int width, int height) 
			: m_Width(width), m_Height(height) { }

		inline int GetWidth() const { return m_Width; }
		inline int GetHeight() const { return m_Height; }

		EVENT_CLASS_TYPE(WindowsResized)
		EVENT_CLASS_CATEGORY(WindowsEventCategory)

	private:
		int m_Width, m_Height;
	};

	/*
	* ----- Mouse Events -----
	*/

	class VZ_API MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(const double x, const double y)
			: m_X(x), m_Y(y) { }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMoved: (" << m_X << ',' << m_Y << ")";
			return ss.str();
		}

		double GetX() const { return m_X; }
		double GetY() const { return m_Y; }

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(InputEventCategory | MouseEventCategory)

	private:
		const double m_X, m_Y;
	};

	class VZ_API MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(const double xOffset, const double yOffset)
			: m_XOffset(xOffset), m_YOffset(yOffset) { }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolled: " << "(" << m_XOffset << "," << m_YOffset << ")";
			return ss.str();
		}

		double GetXOffset() const { return m_XOffset; }
		double GetYOffset() const { return m_YOffset; }

		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(InputEventCategory | MouseEventCategory)

	private:
		const double m_XOffset;
		const double m_YOffset;
	};

	class VZ_API MouseDraggedEvent : public Event
	{
	public:
		MouseDraggedEvent(const double xOffset, const double yOffset)
			: m_XOffset(xOffset), m_YOffset(yOffset) { }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseDragged: (" << m_XOffset << ',' << m_YOffset << ")";
			return ss.str();
		}

		double GetXOffset() const { return m_XOffset; }
		double GetYOffset() const { return m_YOffset; }

		EVENT_CLASS_TYPE(MouseDragged)
		EVENT_CLASS_CATEGORY(MouseEventCategory)

	private:
		const double m_XOffset, m_YOffset;
	};

	/*
	* ----- MouseButton Events -----
	*/

	class VZ_API MouseButtonEvent : public Event
	{
	public:
		MouseButtonEvent(const int button)
			: m_Button(button) { }

		inline int GetMouseButton() const { return m_Button; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonEvent: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_CATEGORY(InputEventCategory | MouseEventCategory | MouseButtonEventCategory)

	private:
		const int m_Button;
	};

	class VZ_API MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(const int button)
			: MouseButtonEvent(button) { }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << MouseButtonEvent::ToString() << ", MouseButtonPressedEvent";
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class VZ_API MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(const int button)
			: MouseButtonEvent(button) { }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << MouseButtonEvent::ToString() << ", MouseButtonReleasedEvent";
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};

	/*
	* ----- Keyboard Events -----
	*/

	class VZ_API KeyEvent : public Event
	{
	public:
		KeyEvent(const char keyCode)
			: m_KeyCode(keyCode) {}

		inline char GetKeyCode() { return m_KeyCode; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_CATEGORY(InputEventCategory | KeyboardEventCategory)

	private:
		char m_KeyCode;
	};

	class VZ_API KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(const char keyCode, const int repeatCount)
			: KeyEvent(keyCode), m_RepeatCount(repeatCount) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << KeyEvent::ToString() << ", KeyPressedEvent, repeat count: " << m_RepeatCount;
			return ss.str();
		}

		inline int GetRepeatCount() { return m_RepeatCount; }

		EVENT_CLASS_TYPE(KeyPressed)

	private:
		int m_RepeatCount;
	};

	class VZ_API KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(const char keyCode)
			: KeyEvent(keyCode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << KeyEvent::ToString() << ", KeyReleasedEvent";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	class VZ_API KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(const char keyCode)
			: KeyEvent(keyCode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << KeyEvent::ToString() << ", KeyTypedEvent";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};
}
