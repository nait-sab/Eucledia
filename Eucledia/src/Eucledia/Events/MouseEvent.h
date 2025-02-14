#pragma once

#include "Event.h"

namespace Eucledia
{
	class EUCLEDIA_API MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(float x, float y) : _mouseX(x), _mouseY(y) {}

		inline float getX() const { return _mouseX; }
		inline float getY() const { return _mouseY; }

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: " << getX() << ", " << getY();
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

	private:
		float _mouseX, _mouseY;
	};

	class EUCLEDIA_API MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(float xOffset, float yOffset) 
			: _xOffset(xOffset), _yOffset(yOffset) {}

		inline float getXOffset() const { return _xOffset; }
		inline float getYOffset() const { return _yOffset; }

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << getXOffset() << ", " << getYOffset();
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

	private:
		float _xOffset, _yOffset;
	};

	class EUCLEDIA_API MouseButtonEvent : public Event
	{
	public:
		inline int getMouseButtn() const { return _button; }

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput | EventCategoryMouseButton)

	protected:
		MouseButtonEvent(int button) : _button(button) {}

		int _button;
	};

	class EUCLEDIA_API MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int button) : MouseButtonEvent(button) {}

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << _button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class EUCLEDIA_API MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int button) : MouseButtonEvent(button) {}

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << _button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};
}