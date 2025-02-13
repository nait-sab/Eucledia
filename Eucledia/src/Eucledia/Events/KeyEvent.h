#pragma once

#include "Event.h"

namespace Eucledia
{
	class EUCLEDIA_API KeyEvent : public Event
	{
	public:
		inline int getKeyCode() const { return _keyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
			
	protected:
		KeyEvent(int keyCode) : _keyCode(keyCode) {}

		int _keyCode;
	};

	class EUCLEDIA_API KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keyCode, int repeatCount) 
			: KeyEvent(keyCode), _repeatCount(repeatCount) {}

		inline int getRepeatCount() const { return _repeatCount; }

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << _keyCode << " (" << _repeatCount << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)

	private:
		int _repeatCount;
	};

	class EUCLEDIA_API KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keyCode) : KeyEvent(keyCode) {}

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << _keyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};
}