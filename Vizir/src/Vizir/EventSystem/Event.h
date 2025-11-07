#pragma once

#include <string>
#include <functional>

#include "Vizir/Core/Core.h"

namespace Vizir
{
	enum class EventType {
		None = 0,
		WindowsClosed, WindowsResized,
		KeyPressed, KeyReleased, KeyTyped,
		MouseMoved, MouseScrolled, MouseDragged,
		MouseButtonPressed, MouseButtonReleased,
	};

	enum EventCategory {
		None = 0,
		WindowsEventCategory = 1 << 0,
		KeyboardEventCategory = 1 << 1,
		MouseEventCategory = 1 << 2,
		MouseButtonEventCategory = 1 << 3,
		InputEventCategory = 1 << 4
	};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	class VZ_API Event
	{
	public:
		bool handled = false;

		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }
#pragma warning(push)
#pragma warning(disable : 26812)
		inline bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & (int)category;
		}
#pragma warning(pop)
	};

	class VZ_API EventDispatcher
	{
		template <typename T>
		using EventFunc = std::function<bool(T&)>;
	public:

		EventDispatcher(Event& e)
			: m_Event(e)
		{
		}

		template<typename T>
		bool Dispatch(EventFunc<T> function)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.handled = function(*(T*)&m_Event);
				return true;
			}

			return false;
		}

	private:
		Event& m_Event;
	};
}
